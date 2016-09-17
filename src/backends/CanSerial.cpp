#include <iostream>
#include <functional>
#include <boost/log/trivial.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include <Wt/WServer>
#include "CanSerial.hpp"

namespace heinz
{

namespace
{
	std::unique_ptr<CanSerialBackend> backendInstance(new CanSerialBackend);
	PluginRegistrar<BackendPlugin> registrar("can", backendInstance.get());
}

CanSerialBackend::CanSerialBackend()
:io(),
thread(boost::bind(&boost::asio::io_service::run, &io)),
work(io)
{}

void CanSerialBackend::backendConfig(ptree &pt)
{
	BOOST_FOREACH( ptree::value_type &v, pt.get_child("buses") )
	{
		string name = v.first.data();
		BOOST_LOG_TRIVIAL(info) << "initializing CAN bus '" << name <<"'";
		shared_ptr<SerialCanDumpPort> port(new SerialCanDumpPort(io, v.second.get<string>("serial_port"), v.second.get<unsigned int>("baud_rate")));
		port->onReceive().connect([name, this](shared_ptr<const ExtendedCanFrame> frame)
		{
			handleCanMessage(name, frame);
		});
		messageLinks[name]; // initialize with empty map
		ports.insert(std::make_pair(name, port));
	}
}

shared_ptr<Endpoint> CanSerialBackend::createEndpoint(shared_ptr<Config> config, ptree &pt)
{
	shared_ptr<CanSerialEndpoint> tmp(new CanSerialEndpoint(pt));
	if( (tmp->getIsInput()) && (tmp->getPollingMode() == EnCanPollingMode::always) )// ondemand-polling is handled elsewhere
	{
		shared_ptr<PollingObject> po=tmp;
		config->pollingObjects.push_back(po);
		tmp->setScriptIfAvailable(pt);
	}
	return tmp;
}

void CanSerialBackend::handleCanMessage(const string port, shared_ptr<const ExtendedCanFrame> frame)
{
	auto portAssignment = getMessageLinks(port);
	auto handlerIt = portAssignment.find(frame->id);
	if(handlerIt == portAssignment.end())
		std::cout << "CAN frame received with UNKNOWN identifier: " << frame->id;
	else
	{
		std::cout << "CAN frame received with identifier: " << frame->id;
		handlerIt->second(frame);
	}
}

shared_ptr<SerialCanDumpPort> CanSerialBackend::getPort(const string &name)
{
	auto it = ports.find(name);
	if(it == ports.end())
		BOOST_THROW_EXCEPTION(ConfigException() << ExErrorMessage(("port '" +name+ "' not found")));
	return it->second;
}

std::map<uint32_t, CanEndpointMessageHandler>& CanSerialBackend::getMessageLinks(const string& name)
{
	std::map<string, std::map<uint32_t, CanEndpointMessageHandler> >::iterator it = messageLinks.find(name);
	if(it == messageLinks.end())
		BOOST_THROW_EXCEPTION(ConfigException() << ExErrorMessage(("port '" +name+ "' not found")));
	return it->second;

}



CanSerialEndpoint::CanSerialEndpoint(ptree &pt)
:HardwareEndpoint(pt),
port(backendInstance->getPort(pt.get<string>("port", ""))),
pollingMode(configCanPollingModes.at(pt.get<string>("poll", "never"))),
hasState(pt.get<bool>("has_state"))
{
	BOOST_FOREACH( ptree::value_type &v, pt.get_child("messages") )
	{
		string message=v.first.data();
		auto it = configCanMessageNames.find(message);
		if(it == configCanMessageNames.end())
		{
			BOOST_LOG_TRIVIAL(warning) << ": ignoring unknown can command: " << message;
			continue;
		}
		EnCanMessageType type = it->second;
		uint32_t can_id = boost::lexical_cast<uint32_t>(v.second.data());
		switch(type)
		{
			case EnCanMessageType::set:
			{
				if(!hasState)
					BOOST_THROW_EXCEPTION(ConfigException() << ExErrorMessage(("stateless nodes can't have 'set' command")));
				setCommand.reset(new uint32_t);
				*setCommand = can_id;
			}
			break;
			case EnCanMessageType::update:
			{
				backendInstance->getMessageLinks(pt.get<string>("port", "")).insert(std::make_pair(can_id, std::bind(&CanSerialEndpoint::handleBusUpdate, this, std::placeholders::_1)));
			}
			break;
			default:
				assert(false);
		}
	}
}

void CanSerialEndpoint::handleBusUpdate(shared_ptr<const ExtendedCanFrame> frame)
{
	{
		boost::unique_lock<CanSerialEndpoint> guard(*this);
		if(hasState)
		{
			switch(rangeType)
			{
				case RANGE_U1:
					this->cachedValue = frame->getData_uint8();
				break;
				case RANGE_U8:
					this->cachedValue = frame->getData_uint8();
				break;
				case RANGE_U16:
					this->cachedValue = frame->getData_uint16();
				break;
				default:
					;// TODO
			}
		}
		else // no state, only relative changes
		{
			switch(rangeType)
			{
				case RANGE_U1:
				break;
				case RANGE_U8:
					this->cachedValue = frame->getData_int8();
				break;
				case RANGE_U16:
					this->cachedValue = frame->getData_int16();
				break;
				default:
					;// TODO
			}
		}
	}
	postUpdates();
}

void CanSerialEndpoint::setValue(int64_t value)
{
	checkValueForValidity(value);
	if(setCommand)
	{
		boost::unique_lock<CanSerialEndpoint> guard(*this);
		this->cachedValue=value;

		switch(rangeType)
		{
			case RANGE_U1:
				port->send(ExtendedCanFrame(*setCommand, (uint8_t) value));
				break;
			case RANGE_U8:
				port->send(ExtendedCanFrame(*setCommand, (uint8_t) value));
				break;
			case RANGE_U16:
				port->send(ExtendedCanFrame(*setCommand, (uint16_t) value));
				break;
			default:
				throw HeinzException("unimplemented range");
		}
	}
	else
		throw HeinzException("setting value for can endpoint without 'set' command");
	triggerUpdates();
}

bool CanSerialEndpoint::isValid()
{
	return hasState;
}

EnCanPollingMode CanSerialEndpoint::getPollingMode()
{
	return pollingMode;
}

int64_t CanSerialEndpoint::getValue()
{
	if(pollingMode == EnCanPollingMode::ondemand)
		throw HeinzException("reading ondemand endpoints is not supported yet");
	boost::shared_lock<ScalarEndpoint> guard(*this);
	return cachedValue;
}

bool CanSerialEndpoint::updatesAvailable()
{
	if(pollingMode == EnCanPollingMode::always)
	{
		// TODO initiate poll, but don't wait for result
	}
	return false;
}
void CanSerialEndpoint::postUpdates()
{
	triggerUpdates();
	executeScript();
}

const std::map<string, EnCanMessageType> configCanMessageNames
{
	{"set", EnCanMessageType::set},
	//{"modify", EnCanMessageType::modify},
	{"update", EnCanMessageType::update}
};

const std::map<string, EnCanPollingMode> configCanPollingModes
{
	{"never", EnCanPollingMode::never},
	{"ondemand", EnCanPollingMode::ondemand},
	{"always", EnCanPollingMode::always}
};

}


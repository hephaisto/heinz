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

void CanSerialBackend::backendConfig(ptree &pt)
{
	BOOST_FOREACH( ptree::value_type &v, pt.get_child("buses") )
	{
		string name = v.first.data();
		shared_ptr<SerialCanDumpPort> port(new SerialCanDumpPort(io, v.second.get<string>("serial_port"), v.second.get<unsigned int>("baud_rate")));
		ports.insert(std::make_pair(name, port));
	}
	boost::thread t(boost::bind(&boost::asio::io_service::run, &io));
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
				backendInstance->getMessageLinks()[pt.get<string>("port")].insert(std::make_pair(can_id, boost::bind(&CanSerialEndpoint::handleBusUpdate, this, boost::placeholders::_1)));
			}
			break;
			default:
				assert(false);
		}
	}
}

void CanSerialEndpoint::handleBusUpdate(shared_ptr<ExtendedCanFrame> frame)
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

}

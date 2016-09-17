#ifndef CAN_SERIAL_ENDPOINT_H
#define CAN_SERIAL_ENDPOINT_H

#include <serial_can_dump.hpp>

#include "../HardwareEndpoint.hpp"
#include "../config/config_loader.hpp"
#include "BackendPlugin.hpp"

/// CURRENT STATE OF IMPLEMENTATION
// missing: ondemand reading

namespace heinz
{

typedef std::function<void (shared_ptr<const ExtendedCanFrame>)> CanEndpointMessageHandler;

class CanSerialBackend : public BackendPlugin
{
public:
	CanSerialBackend();
	virtual void backendConfig(ptree &pt);
	virtual shared_ptr<Endpoint> createEndpoint(shared_ptr<Config> config, ptree &pt);
	shared_ptr<SerialCanDumpPort> getPort(const string &name);
	std::map<uint32_t, CanEndpointMessageHandler>& getMessageLinks(const string &port);
private:
	boost::asio::io_service io;
	boost::thread thread;
	boost::asio::io_service::work work;
	std::map<string, shared_ptr<SerialCanDumpPort> > ports;
	void handleCanMessage(const string port, shared_ptr<const ExtendedCanFrame> frame);
	std::map<string, std::map<uint32_t, CanEndpointMessageHandler> > messageLinks;
};

enum class EnCanMessageType
{
	set,
	//modify, // does not concern us at the moment
	update
	//hardware // not implemented yet (will probably stay that way?)
};

enum class EnCanPollingMode
{
	never,
	ondemand,
	always
};

extern const std::map<string, EnCanMessageType> configCanMessageNames;
extern const std::map<string, EnCanPollingMode> configCanPollingModes;

/**
An endpoint connected via serial-can-dump
**/
class CanSerialEndpoint : public HardwareEndpoint, public PollingObject
{
public:
	CanSerialEndpoint(ptree &pt);
	virtual bool isValid();
	virtual void setValue(int64_t value);
	virtual int64_t getValue(); //< value for endpoints without state will return last relative value
	// inherited from PollingObject
	virtual bool updatesAvailable();
	virtual void postUpdates();

	static shared_ptr<Endpoint> create(shared_ptr<Config> config, ptree &pt);
	void handleBusUpdate(shared_ptr<const ExtendedCanFrame> frame);

	EnCanPollingMode getPollingMode();
private:
	shared_ptr<SerialCanDumpPort> port;
	EnCanPollingMode pollingMode;
	std::unique_ptr<uint32_t> setCommand;
	bool hasState;
};

}

#endif

#pragma once 
#include <functional>
#include <boost/uuid/uuid.hpp>
#include <string_view>
class DuServer;
struct Callbacks
{
	std::function<bool(boost::uuids::uuid)> _doesExistUuid;
	std::function<bool(boost::uuids::uuid)> _setMasterUuid;
	std::function<void(std::string_view)> _sendPingCommand;
	std::function<void()> _sendStopPingCommand;
	static Callbacks& Instance(DuServer* server);
private:
	Callbacks(DuServer* serv);
};
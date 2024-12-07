#pragma once 
#include <functional>
#include <boost/uuid/uuid.hpp>
class DuServer;
struct Callbacks
{
	std::function<bool(boost::uuids::uuid)> _doesExistUuid;
	std::function<bool(boost::uuids::uuid)> _setMasterUuid;
	static Callbacks& Instance(DuServer* server);
private:
	Callbacks(DuServer* serv);
};
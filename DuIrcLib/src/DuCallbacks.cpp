#include "../include/DuCallbacks.hpp"
#include "../include/DuServer.h"
Callbacks& Callbacks::Instance(DuServer* server)
{
	static Callbacks cb(server);
	return cb;
}

Callbacks::Callbacks(DuServer* serv)
{
	_doesExistUuid = std::bind(&DuServer::doesExistUuid, serv, std::placeholders::_1);
}
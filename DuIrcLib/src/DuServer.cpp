#include "../include/DuServer.h"
#include "../include/DuCallbacks.hpp"

DuServer::DuServer() : m_acceptor(m_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT))
{
	Callbacks::Instance(this);
}
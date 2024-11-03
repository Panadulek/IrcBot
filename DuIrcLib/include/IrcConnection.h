#pragma once
#include <memory>
#include <boost/asio.hpp>
#include "IReader.h"
#include <string>
#include <string_view>
class Connection final
{

	using Socket = boost::asio::ip::tcp::socket;
	using Resolver = boost::asio::ip::tcp::resolver;
	Socket m_socket;
	Resolver m_resolver;
	enum 
	{
		CONNECTED,
		DISCONNECTED,
	}m_state;
	std::string m_connectionMessage;
	Connection(boost::asio::io_context& ioContext);
	
public:
	static std::unique_ptr<Connection> newConnection(boost::asio::io_context& ioContext);
	bool connect(std::string ip, std::string port);
	std::string_view connectionMessage() { return m_connectionMessage; }
	auto state() { return m_state; }
	Socket& refSocket() { return m_socket; }
	~Connection() = default;

};

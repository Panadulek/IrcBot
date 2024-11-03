#include "../include/IrcConnection.h"
#include "../include/IReader.h"
#include <string_view>
#include <memory>
#include <iostream>
Connection::Connection(boost::asio::io_context& ioContext)
	:  m_state(Connection::DISCONNECTED), m_socket(ioContext), m_resolver(ioContext)
{
}

std::unique_ptr<Connection> Connection::newConnection(boost::asio::io_context& ioContext)
{
	return std::unique_ptr<Connection>(new Connection(ioContext));
}


bool Connection::connect( std::string ip, std::string port) //probably done
{
	boost::asio::ip::tcp::resolver::query query(ip.data(), port);
	Resolver::results_type endpoints = m_resolver.resolve(ip, port);
	auto iter = endpoints;
	boost::asio::ip::tcp::resolver::iterator end;
	while (iter != end)
	{
		boost::asio::ip::tcp::endpoint endpoint = *iter++;
		std::cout << endpoint << std::endl;
	}
	boost::asio::async_connect(m_socket, endpoints, [this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint endpoint) {
		        if (!ec)
				{
					m_state = CONNECTED;
					m_connectionMessage = ec.message();
                } else {
					m_state = DISCONNECTED;
					m_connectionMessage = ec.message();
                }
	});

	return m_state == CONNECTED;
}
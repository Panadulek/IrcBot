#pragma once
#include <memory>
#include <boost/asio.hpp>
#include "IReader.h"
#include <string>
#include <string_view>
#include <format>
#include <variant>
#include <vector>
#include <optional>
class Connection final
{

	using Socket = boost::asio::ip::tcp::socket;
	using IcmpSocket = boost::asio::ip::icmp::socket;
	using TcpResolver = boost::asio::ip::tcp::resolver;
	using IcmpResolver = boost::asio::ip::icmp::resolver;
	using Resolver = TcpResolver;
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

	bool connect(const std::string& ip, const std::string& port);
	std::string_view connectionMessage() { return m_connectionMessage; }
	auto state() { return m_state; }
	bool isConnected() { return state() == CONNECTED; }
	Socket& refSocket() { return m_socket; }
	void closeConnection()
	{
		m_socket.cancel();
		boost::system::error_code ec;
		m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
		if (ec)
		{
			std::cerr << std::format("Shutdown failed {}", ec.message()) << std::endl;
		}
		m_socket.close(ec);
		if(ec)
		{
			std::cerr << std::format("Close failed {}", ec.message()) << std::endl;
		}
	}
	~Connection()
	{
		closeConnection();
	}
};

#include "IrcConnection.h"
#include "IWriter.h"
#include<string_view>
#include "IrcProtocol.h"
#include <thread>
#include <deque>
class IrcClient final
{
	std::unique_ptr<Connection> m_connection;
	std::deque<std::shared_ptr<IWriter>> m_writer;
	std::shared_ptr<IReader> m_reader;
	std::string_view m_ip;
	std::string_view m_port;
	boost::asio::io_context m_context;
	std::unique_ptr<std::thread> m_loopThread;
	static void _run(boost::asio::io_context& ctx)
	{
		ctx.run();
	}
	void _SendData();
public:
	IrcClient(std::string ip, std::string port) : m_ip(ip), m_port(port), m_context()
	{
		m_connection = Connection::newConnection(m_context);
		m_connection->connect(ip, port);
	}
	void setReader(std::shared_ptr<IReader> iReader)
	{
		m_reader = iReader;
	}
	void RegisterReader(std::shared_ptr<IReader> iReader);
	void SendData(std::shared_ptr<IWriter> iReader);
	void run()
	{
		m_loopThread = std::make_unique<std::thread>(_run, std::ref(m_context));
	}
	~IrcClient()
	{
	
	}
};
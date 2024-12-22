#include "IrcConnection.h"
#include "IWriter.h"
#include<string_view>
#include "IrcProtocol.h"
#include <thread>
#include <deque>
#include <atomic>
#include "Pinger.h"
class IrcClient final
{
	std::unique_ptr<Connection> m_connection;
	std::shared_ptr<IReader> m_reader;
	std::string_view m_ip;
	std::string_view m_port;
	boost::asio::io_context m_context;
	std::unique_ptr<std::thread> m_loopThread;
	std::unique_ptr<Pinger> m_pinger;
	IrcProtocol m_protocol;
	std::atomic_flag m_uiidSet;
	static void _run(boost::asio::io_context& ctx)
	{
		while (1)
		{
			ctx.run();
		}
	}
	void _SendData(std::shared_ptr<IWriter> writer);
	void process(std::shared_ptr<IReader> writer);
	void RegisterReadHeader();
	void RegisterReader();
	void initIcmpProtocol();
public:
	IrcClient(std::string ip, std::string port, std::shared_ptr<IReader> reader) : m_ip(ip), m_port(port), m_context(), m_reader(reader)
	{
		m_connection = Connection::newConnection(m_context);
		m_connection->connect(ip, port);
		m_uiidSet.clear();
	}
	void SendData(std::shared_ptr<IWriter> iReader);
	void run()
	{
		m_loopThread = std::make_unique<std::thread>(_run, std::ref(m_context));
		while (!m_connection->isConnected())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		RegisterReadHeader();
		while (!m_uiidSet.test())
			Sleep(1);
	}
	~IrcClient()
	{
	
	}
};
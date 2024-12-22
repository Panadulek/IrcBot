#pragma once

#include "DuSession.h"
#include <vector>
#include <functional>
#include <ranges>
class DuServer
{

private:
	boost::asio::io_context m_context;
	using  Acceptor = boost::asio::ip::tcp::acceptor;
	using Sessions = std::vector<std::shared_ptr<DuSession>>;
	Acceptor m_acceptor;
	std::shared_ptr<Sessions> m_sessions;
	std::optional<boost::uuids::uuid> m_masterId;

	bool doesExistUuid(boost::uuids::uuid id)
	{
		auto it = std::find_if(m_sessions->begin(), m_sessions->end(), [id](std::shared_ptr<DuSession>session) {
			return session->hasUUID() && session->getUUID().value() == id;
			});
		return it != m_sessions->end();
	}
	
	bool setMasterUuid(boost::uuids::uuid id)
	{
		bool hadMaster = false;
		if (!m_masterId)
		{
			m_masterId = id;
			hadMaster = true;
		}
		return hadMaster;
	}

	void sendPingCommand(std::string_view ip)
	{
		std::ranges::for_each(*m_sessions, [this, ip](std::shared_ptr<DuSession> client)
			{
				if (!client->hasUUID())
					return;
				client->sendPingCommand(ip);
			});
	}

public:

	

	static constexpr uint16_t PORT = 6666;
	DuServer();
	void listen()
	{
		m_acceptor.async_accept([this](boost::system::error_code ec, DuSession::TcpSocket socket)
			{
				if (!ec)
				{
					if (!m_sessions)
						m_sessions = std::make_shared<Sessions>();
					m_sessions->emplace_back(std::make_shared<DuSession>(std::move(socket)));
					m_sessions->back()->read();
					std::shared_ptr<IWriter> writer = std::make_shared<DuSession::Writer>(IWriter::Header(IWriter::Header::MESSAGE_TYPE::EXPECT_UUID, 0));
					m_sessions->back()->write(writer);
				}
				listen();
			}
		);

	}
	void run()
	{
		m_context.run();
	}

	friend struct Callbacks;
}
;
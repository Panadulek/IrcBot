#pragma once

#include "DuSession.h"
#include <vector>
#include <functional>
class DuServer
{

private:
	boost::asio::io_context m_context;
	using  Acceptor = boost::asio::ip::tcp::acceptor;
	using Sessions = std::vector<std::shared_ptr<DuSession>>;
	Acceptor m_acceptor;
	std::shared_ptr<Sessions> m_sessions;


	bool doesExistUuid(boost::uuids::uuid id)
	{
		auto it = std::find_if(m_sessions->begin(), m_sessions->end(), [id](std::shared_ptr<DuSession>session) {
			return session->hasUUID() && session->getUUID().value() == id;
			});
		return it != m_sessions->end();
	}


public:

	struct Callbacks
	{
		std::function<bool(boost::uuids::uuid)> _doesExistUuid;
		static Callbacks& Instance(DuServer* server)
		{
			Callbacks cb(server);
			return cb;
		}
	private:
		Callbacks(DuServer* serv)
		{
			_doesExistUuid = std::bind(&DuServer::doesExistUuid, serv, std::placeholders::_1);
		}
	};


	static constexpr uint16_t PORT = 6666;
	DuServer() : m_acceptor(m_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT)) 
	{
		Callbacks::Instance(this);
	}
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
				}
				listen();
			}
		);

	}
	void run()
	{
		m_context.run();
	}

};
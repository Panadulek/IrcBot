#include "../include/IrcClient.h"
#include <iostream>

void IrcClient::RegisterReader(std::shared_ptr<IReader> iReader)
{
	auto buff = m_reader->OutputBuffer();
	m_connection->refSocket().async_read_some(boost::asio::buffer(m_reader->OutputBuffer(), 4096), [&](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				auto reader = m_reader->operator()(length);
				RegisterReader(iReader);
				if (reader)
					SendData(reader);
			}
			else
			{
				std::cout << ec.message() << std::endl;
				assert(0 && "async_read ex != 0");
			}
		}
	);
}

void IrcClient::_SendData()
{
	std::shared_ptr<IWriter> writer = m_writer.front();
	auto& buffer = writer->InputBuffer();
	m_connection->refSocket().async_write_some(boost::asio::buffer(&buffer[0], buffer.size()), [&](boost::system::error_code ec, std::size_t lengthWritten)
		{
			if (!ec)
			{
				bool sendDataAgain = m_writer.front()->operator()(lengthWritten);
				if (sendDataAgain)
					_SendData();
				else
					m_writer.pop_front();
			}
			else
			{
				std::cout << ec.message() << std::endl;
				assert(0 && "async_read ex != 0");
			}
		});
}


void IrcClient::SendData(std::shared_ptr<IWriter> iWriter)
{
	auto& buffer = iWriter->InputBuffer();
	boost::asio::post(m_context, [this, iWriter]() {
		m_writer.push_back(iWriter);
		_SendData();
	}
	);
}
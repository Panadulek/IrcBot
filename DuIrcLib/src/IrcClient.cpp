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


void IrcClient::SendData(std::shared_ptr<IWriter> iWriter)
{

	auto& buffer = iWriter->InputBuffer();
	m_connection->refSocket().async_write_some(boost::asio::buffer(&buffer[0], buffer.size()), [this, iWriter](boost::system::error_code ec, std::size_t lengthWritten)
		{
			if (!ec)
			{
				bool sendDataAgain = iWriter->operator()(lengthWritten);
				if (sendDataAgain)
					SendData(iWriter);
			}
			else
			{
				std::cout << ec.message() << std::endl;
				assert(0 && "async_read ex != 0");
			}
		});
}
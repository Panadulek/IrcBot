#include "../include/IrcClient.h"
#include <iostream>

void IrcClient::RegisterReader()
{
	auto& buff = m_reader->OutputBuffer();
	m_connection->refSocket().async_read_some(boost::asio::buffer(buff, buff.size()), [&](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				auto dataToWrite = m_reader->operator()(length);
				RegisterReader();
				if (dataToWrite)
					SendData(dataToWrite);
			}
			else
			{
				std::cout << ec.message() << std::endl;
				assert(0 && "async_read ex != 0");
			}
		}
	);
}

void IrcClient::_SendData(std::shared_ptr<IWriter> writer)
{
	
	m_connection->refSocket().async_write_some(boost::asio::buffer(&writer->InputBuffer()[0], writer->getHeader().MessageSize),
		[this, writer](boost::system::error_code ec, std::size_t lengthWritten)
		{
			if (!ec)
			{
				if (writer->operator()(lengthWritten))
					_SendData(writer);
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
	std::size_t headerSize = 0;
	uint8_t* buffer =  iWriter->getHeaderAsBytes(headerSize);
	m_connection->refSocket().async_write_some(boost::asio::buffer((char*)buffer, headerSize), [this, iWriter](boost::system::error_code ec, std::size_t lengthWritten)
		{
			if (!ec)
			{
				bool sendDataAgain = iWriter->operator()(lengthWritten);
				if (iWriter->getHeader().MessageSize > 0)
				{
					_SendData(iWriter);
				}
			}
			else
			{
				std::cout << ec.message() << std::endl;
				assert(0 && "async_read ex != 0");
			}
		});
}
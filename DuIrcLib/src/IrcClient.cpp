#include "../include/IrcClient.h"
#include <iostream>
#include "../include/Pinger.h"

void IrcClient::RegisterReadHeader()
{
	std::size_t len;
	auto* buff = m_reader->getHeaderAsBytes(len);
	m_connection->refSocket().async_read_some(boost::asio::buffer(buff, sizeof(IReader::Header)), [&](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				auto dataToWrite = m_reader->operator()(length);
				if (m_reader->getHeader().Type == IrcProtocol::Header::MESSAGE_TYPE::UUID_RESPONSE_OK)
					m_uiidSet.test_and_set();
				if (dataToWrite)
					SendData(dataToWrite);
				RegisterReader();
			}
			else
			{
				std::cout << ec.message() << std::endl;
				assert(0 && "async_read ex != 0");
			}
		}
	);
}

void IrcClient::process(std::shared_ptr<IReader> reader)
{
	if (reader->getHeader().Type == IrcProtocol::Header::MESSAGE_TYPE::PING)
	{
		std::string view = reinterpret_cast<char*>(&reader->OutputBuffer()[0]);
		m_pinger = std::make_unique<Pinger>(view.data());
	}
	else if (reader->getHeader().Type == IrcProtocol::Header::MESSAGE_TYPE::PING_STOP && m_pinger)
	{
		m_pinger->stopPing();
		m_pinger.reset();
	}
}
void IrcClient::RegisterReader()
{
	if (!m_reader->hasBytesToRead())
	{
		RegisterReadHeader();
		return;
	}
	auto& buff = m_reader->OutputBuffer();
	m_connection->refSocket().async_read_some(boost::asio::buffer(buff, buff.size()), [&](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				auto dataToWrite = m_reader->operator()(length);
				m_reader->OutputBuffer()[length] = 0;
				if (dataToWrite)
					SendData(dataToWrite);
				else
					process(m_reader);

				if (m_reader->hasBytesToRead())
					RegisterReader();
				else
					RegisterReadHeader();
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


void IrcClient::initIcmpProtocol()
{

}

void IrcClient::SendData(std::shared_ptr<IWriter> iWriter)
{
	if (!iWriter->isCallback())
	{
		initIcmpProtocol();
		return;
	}
	std::size_t headerSize = 0;
	uint8_t* buffer =  iWriter->getHeaderAsBytes(headerSize);
	m_connection->refSocket().async_write_some(boost::asio::buffer((char*)buffer, headerSize), [this, iWriter](boost::system::error_code ec, std::size_t lengthWritten)
		{
			if (!ec)
			{
				if (iWriter->operator()(lengthWritten))
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
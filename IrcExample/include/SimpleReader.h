#pragma once
#include <IReader.h>
#include "SimpleWriter.h"
#include "IrcProtocol.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <Pinger.h>
class Reader : public IReader
{
	using Header = IrcProtocol::Header;
	ReadContainer m_readContainer;
	Header m_headerBuffer;
public:
	Reader()
	{
	}
	virtual std::shared_ptr<IWriter> operator ()(std::size_t length) override
	{
		m_headerBuffer.MessageSize -= length;
		switch (m_headerBuffer.Type)
		{
		case Header::MESSAGE_TYPE::EXPECT_UUID:
			{
				boost::uuids::random_generator gen;
				boost::uuids::uuid id = gen();
				std::string idStr = boost::uuids::to_string(id);
				return std::make_shared<Writer<false>>(IWriter::Header(IWriter::Header::MESSAGE_TYPE::SET_UUID, idStr.size()), idStr);
			}
		case Header::MESSAGE_TYPE::MASTER_RESPONSE_OK:
			std::cout << "MASTER_OK" << std::endl;
			break;
		case Header::MESSAGE_TYPE::MASTER_RESPONSE_ERR:
			std::cout << "MASTER_ERR" << std::endl;
			break;
		case Header::MESSAGE_TYPE::PING:
			std::cout << "PING COMAND" << std::endl;
			break;
		default:
			break;
		}
		return nullptr;
	}
	virtual ReadContainer& OutputBuffer()
	{
		return  m_readContainer;

	}
	virtual Header& getHeader()
	{
		return m_headerBuffer;
	}
	virtual uint8_t* getHeaderAsBytes(std::size_t& size)
	{
		size = sizeof(Header);
		return (uint8_t*)&m_headerBuffer;
	}
	bool hasBytesToRead() override
	{
		return m_headerBuffer.MessageSize > 0;
	}

};
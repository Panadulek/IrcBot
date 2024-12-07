#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <array>
#include <cstdint>
#include <boost/uuid/uuid_generators.hpp>
#include "IrcProtocol.h"
#include <boost/lexical_cast.hpp>
#include "IReader.h"
#include <iostream>
#include "DuCallbacks.hpp"


#define CALLBACKS Callbacks::Instance(nullptr)
class DuSession : public std::enable_shared_from_this<DuSession> {
public:
	using TcpSocket = boost::asio::ip::tcp::socket;
	using UUID = boost::uuids::uuid;
private:
	TcpSocket m_socket;
	static constexpr std::size_t BUFFER_SIZE = 4096;
	std::array<uint8_t, BUFFER_SIZE> m_data;
	int readerCounter = 0;
	class SessionReader : public IReader
	{
		ReadContainer buffer;
		Header header;
	public:
		SessionReader() : IReader() 
		{
			std::memset(&header, 0, sizeof(Header));
		}
		virtual std::shared_ptr<IWriter> operator ()(std::size_t length) override
		{
			header.MessageSize -= length;
			return nullptr;
		}
		virtual ReadContainer& OutputBuffer() override
		{
			return buffer;
		}
		virtual Header& getHeader() override
		{
			return header;
		}
		virtual uint8_t* getHeaderAsBytes(std::size_t& size) override
		{
			size = sizeof(Header);
			return (uint8_t*)&header;
		}
		bool hasBytesToRead() override
		{
			return header.MessageSize > 0;
		}
	};



	class SessionWriter : public IWriter
	{
		QueueData m_data;
		Header m_header;
	public:
		SessionWriter(Header header) : m_header(header), IWriter() {}
		virtual bool operator ()(std::size_t writtenDataSize)
		{
			bool bSendData = m_header.MessageSize > writtenDataSize;
			m_header.MessageSize -= writtenDataSize;
			return bSendData;
		}
		virtual QueueData& InputBuffer() 
		{
			return m_data;
		}

		virtual uint8_t* getHeaderAsBytes(std::size_t& size) override
		{
			size = sizeof(Header);
			return reinterpret_cast<uint8_t*>(&m_header);
		}
		virtual Header getHeader()
		{
			return m_header;
		}

	};


	struct {
		UUID _id;
		bool _isSettedUUID : 1;
	} m_settings;

	using MainHeader = IrcProtocol::Header;
	void sendExpectUUID()
	{
		auto self(shared_from_this());
		MainHeader(MainHeader::MESSAGE_TYPE::EXPECT_UUID, 0);
		
	}

	void _readData(std::shared_ptr<IReader> reader)
	{
		if (!reader->hasBytesToRead())
		{
			read();
			return;
		}
		auto self(shared_from_this());
		std::size_t size = reader->OutputBuffer().size();
		uint8_t* begin = &reader->OutputBuffer().front();
		m_socket.async_read_some(boost::asio::buffer(begin, reader->getHeader().MessageSize),
			[this, self, reader, begin](boost::system::error_code ec, std::size_t length)
			{
				
				if (!ec)
				{
					std::shared_ptr<IWriter> writer = reader->operator()(length);
					if (!hasUUID())
					{
						if (reader->getHeader().Type == IReader::Header::MESSAGE_TYPE::SET_UUID)
						{
							boost::uuids::string_generator gen;
							std::string uuidStr((char*)begin, (char*)begin +length);
							
							auto id = gen(uuidStr);
							if (CALLBACKS._doesExistUuid(id) )
							{
								std::shared_ptr<IWriter> writer = std::make_shared<SessionWriter>(IWriter::Header(IWriter::Header::MESSAGE_TYPE::EXPECT_UUID, 0));
								_writeHeader(writer);
							}
							else
							{
								m_settings._isSettedUUID = true;
								m_settings._id = id;
							}
						}
						else
						{
							IWriter::Header header(IWriter::Header::MESSAGE_TYPE::EXPECT_UUID, 0);
							writer = std::make_shared<SessionWriter>(header);
							std::cout << "NOT OK";
						}
					}
					else
					{

					}
					_readData(reader);
				}
			});

	}

	void _readHeader()
	{
		auto self(shared_from_this());
		std::shared_ptr<IReader> reader = std::make_shared<SessionReader>();
		std::size_t headerSize = sizeof(IrcProtocol::Header);
		m_socket.async_receive(boost::asio::buffer(reader->getHeaderAsBytes(headerSize), sizeof(IrcProtocol::Header)),
			[this, self, reader, headerSize](boost::system::error_code ec, std::size_t length)
			{
				if (!ec)
				{
					reader->operator()(length);
					_readData(reader);
				}
				else
				{
					std::cerr << "Nie odczytano naglowka" << std::endl;
				}
				std::cout << "Reader counter: " << reader.use_count() << std::endl;
			});
			
	}

	void _writeHeader(std::shared_ptr<IWriter> writer)
	{
		auto self(shared_from_this());
		std::size_t headerSize = 0 ;
		m_socket.async_write_some(boost::asio::buffer(writer->getHeaderAsBytes(headerSize), sizeof(IrcProtocol::Header)),
			[this, self, writer](boost::system::error_code ec, std::size_t length)
			{
				if (!ec)
				{
					if (writer->operator()(length))
					{
						_write(writer);
					}						
				}
			});
	}

	void _write(std::shared_ptr<IWriter> writer)
	{
	
	}
	
public:
	using Writer = SessionWriter;
	DuSession(TcpSocket socket) : m_socket(std::move(socket))
	{
		m_settings._isSettedUUID = false;
	}
	void read() 
	{
		readerCounter++;
		_readHeader();
	}
	void write(std::shared_ptr<IWriter> writer)
	{
		_writeHeader(writer);
	}
	bool hasUUID()
	{
		return m_settings._isSettedUUID;
	}

	std::optional<UUID> getUUID() const
	{
		if (!m_settings._isSettedUUID)
			return std::nullopt;
		return m_settings._id;
	}
	const bool operator ==(const DuSession& other)
	{
		return *getUUID() == *other.getUUID();
	}
};

#undef CALLBACKS
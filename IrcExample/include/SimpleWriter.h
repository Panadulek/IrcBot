#pragma once

#include <IWriter.h>

class Writer : public IWriter
{
	QueueData m_bufferToSend;

	bool sendHeader;
public:
	Header m_header;
	Writer(Header h, const std::string& str) : m_header(h), sendHeader(true), m_bufferToSend(str.begin(), str.end())
	{
		m_header.MessageSize += 1;
		InputBuffer().emplace_back('\0');
	}
	Writer(Header h) : m_header(h), sendHeader(true)
	{
		assert(h.MessageSize == 0);
	}
	virtual QueueData& InputBuffer() override
	{
		return m_bufferToSend;
	}
	virtual bool operator ()(std::size_t writtenDataSize) override //return true if some data have not been send
	{

		return writtenDataSize < m_header.MessageSize;
	}
	virtual uint8_t* getHeaderAsBytes(std::size_t& size) override
	{
		size = sizeof(Header);
		return reinterpret_cast<uint8_t*>(&m_header);
	}
	virtual Header getHeader() {
		return m_header;
	}
};
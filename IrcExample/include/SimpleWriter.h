#pragma once

#include <IWriter.h>

class Writer : public IWriter
{
	QueueData m_bufferToSend;
public:
	Writer(std::string str) : m_bufferToSend(str.begin(), str.end())
	{}

	virtual QueueData& InputBuffer() override
	{
		return m_bufferToSend;
	}
	virtual bool operator ()(std::size_t writtenDataSize) //return true if some data have not been send
	{
		m_bufferToSend.erase(m_bufferToSend.begin(), m_bufferToSend.begin() + writtenDataSize);
		return !m_bufferToSend.empty();
	}
};
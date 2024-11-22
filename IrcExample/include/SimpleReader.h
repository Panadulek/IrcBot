#pragma once
#include <IReader.h>
#include "SimpleWriter.h"
#include "IrcProtocol.h"
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
		return nullptr;
	}
	virtual ReadContainer& OutputBuffer()
	{
		return  m_readContainer;

	}
	virtual Header getHeader()
	{
		return m_headerBuffer;
	}
	virtual uint8_t* getHeaderAsBytes(std::size_t& size)
	{
		return nullptr;
	}

};
#pragma once
#include <IReader.h>
#include "SimpleWriter.h"
class Reader : public IReader
{
	ReadContainer m_readContainer;
public:
	Reader()
	{
	}
	virtual std::shared_ptr<IWriter> operator ()(std::size_t length) override
	{
		std::string output(m_readContainer.begin(), m_readContainer.begin() + length);
		std::size_t pos = output.find("PING", 0, strlen("PING"));
		std::cout << output << std::endl;
		if(pos != std::string::npos)
		{
			std::string replay = "PONG :";
			replay += output.substr(6) + "\r\n";
			return std::make_shared<Writer>(replay);
		}
		return nullptr;
	}
	virtual ReadContainer& OutputBuffer()
	{
		return  m_readContainer;

	}
};
#pragma once
#include <string_view>
#include <queue>
#include "IrcProtocol.h"

#include <vector>
struct IWriter
{
	using QueueData = std::vector<uint8_t>;
	using Header = IrcProtocol::Header;
	virtual bool operator ()(std::size_t writtenDataSize) = 0; //return true if some data have not been send 

	virtual QueueData& InputBuffer() = 0;
	virtual Header getHeader() = 0;
	virtual uint8_t* getHeaderAsBytes(std::size_t&) = 0;
protected:
	IWriter() {}
};
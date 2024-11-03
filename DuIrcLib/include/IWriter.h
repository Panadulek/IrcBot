#pragma once
#include <string_view>
#include <queue>
struct IWriter
{
	using QueueData = std::deque<uint8_t>;

	virtual bool operator ()(std::size_t writtenDataSize) = 0; //return true if some data have not been send 

	virtual QueueData& InputBuffer() = 0;
protected:
	IWriter() {}
};
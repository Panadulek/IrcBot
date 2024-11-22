#pragma once
#include <boost/system.hpp>
#include <array>
#include <cstdint>
#include <span>
#include <cassert>
#include <array>
#include "IWriter.h"
#include "IrcProtocol.h"
struct IReader 
{
	using Header = IrcProtocol::Header;
	using ReadContainer = std::array<uint8_t, 4096>;
	virtual std::shared_ptr<IWriter> operator ()(std::size_t length) = 0; //return potential replay.
	virtual ReadContainer& OutputBuffer() = 0;
	virtual Header getHeader() = 0;
	virtual uint8_t* getHeaderAsBytes(std::size_t& size) = 0;
};
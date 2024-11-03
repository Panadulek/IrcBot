#pragma once
#include <boost/system.hpp>
#include <array>
#include <cstdint>
#include <span>
#include <cassert>
#include <array>
#include "IWriter.h"
struct IReader 
{
	using ReadContainer = std::array<uint8_t, 4096>;
	virtual std::shared_ptr<IWriter> operator ()(std::size_t length) = 0; //return potential replay.
	virtual ReadContainer& OutputBuffer() = 0;
};
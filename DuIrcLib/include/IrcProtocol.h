#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include <format>
#include <array>
#include <boost/uuid/uuid.hpp>
#define COMMAND_METHOD static std::string



class IrcProtocol final
{



public:
	struct  Header
	{
		enum class MESSAGE_TYPE : uint8_t
		{
			SET_UUID,
			EXPECT_UUID,
		};

		MESSAGE_TYPE Type;
		uint64_t MessageSize;
		Header(const MESSAGE_TYPE type, const uint64_t ms) : Type(type), MessageSize(ms) {}
		Header() = default;
	};
private:

	Header getUuidMessage(std::string_view id)
	{
		return Header(Header::MESSAGE_TYPE::SET_UUID, id.size());
	}

	

};
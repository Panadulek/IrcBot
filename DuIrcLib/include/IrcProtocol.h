#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include <format>
#include <array>
#include <variant>
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
			UUID_RESPONSE_OK,

			MASTER,
			MASTER_RESPONSE_OK,
			MASTER_RESPONSE_ERR,
			
			PING,
			PING_RESPONSE_OK,
			PING_RESPONSE_ERR,
		
			PING_STOP,
			PING_STOP_RESPONSE_OK,
			PING_STOP_RESPONSE_ERR,

			NULL_TYPE,
		};

		MESSAGE_TYPE Type;
		uint64_t MessageSize;
		uint8_t isFromClient;
		Header(const MESSAGE_TYPE type, const uint64_t ms) : Type(type), MessageSize(ms + sizeof(Header))
		{
#ifdef CLIENT_DU
			isFromClient = 1;
#else
			isFromClient = 0;
#endif
		}
		Header() = default;
	};

};



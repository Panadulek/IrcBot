#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include <format>
class IrcProtocol final
{
	enum COMMAND_ID : uint8_t
	{
		HELLO = 0,
		NICK,
		USER,
		PASSWORD,
		LAST_COMMAND,
	};
	static constexpr std::size_t MAX_COMMAND_LEN = 64;
	const char command[LAST_COMMAND][MAX_COMMAND_LEN] = {
		"HELLO",
		"NICK",
		"USER",
		"PASSWORD",
	};

	std::string buildSimpleCommand(COMMAND_ID id, std::string_view val) { return std::format("{} {}\r\n", command[id], val); }
public:
	std::string createLoginMessage();
	std::string createLoginMessage(std::string_view user);
	std::string createNickMessage(std::string_view nick);
	std::string createPassMessage(std::string_view pass);

};
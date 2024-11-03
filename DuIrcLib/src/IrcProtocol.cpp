#include "../include/IrcProtocol.h"
#include <string_view>
#include <format>
std::string IrcProtocol::createLoginMessage() { return std::format("{}", command[HELLO]); }

std::string IrcProtocol::createLoginMessage(std::string_view user) { return user.size() > 0 ? buildSimpleCommand(USER, user) : ""; }

std::string IrcProtocol::createNickMessage(std::string_view nick) { return nick.size() > 0 ? buildSimpleCommand(NICK, nick) : ""; }

std::string IrcProtocol::createPassMessage(std::string_view pass) { return pass.size() > 0 ? buildSimpleCommand(PASSWORD, pass) : ""; }



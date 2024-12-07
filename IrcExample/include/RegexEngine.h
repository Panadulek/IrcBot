#pragma once
#include <map>
#include "IrcProtocol.h"
#include <boost/regex.hpp>
#include <string_view>
class RegexEngine
{
public:
	using RegexId = IrcProtocol::Header::MESSAGE_TYPE;
	using FoundRegex = std::pair<bool, RegexId>;
	RegexEngine()
	{
		m_regexes.insert({ RegexId::PING, boost::regex(R"(ping\s+ipv4\s+(\d{1,3}\.){3}\d{1,3})") });
	}
	FoundRegex findRegex(std::string_view line)
	{
		for (auto it : m_regexes)
		{
			if (boost::regex_match(line.data(), it.second))
			{
				return { true, it.first };
			}
		}
		return { false, RegexId::NULL_TYPE };
	}
private:
	std::map<RegexId, boost::regex> m_regexes;
};


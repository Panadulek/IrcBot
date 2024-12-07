#include <iostream>
#include <IrcClient.h>
#include <cassert>
#include "../include/SimpleReader.h"
#include "../include/SimpleWriter.h"
#include <IrcProtocol.h>
#include "../include/DuSession.h"
#include <boost/uuid/uuid.hpp>            
#include <boost/uuid/uuid_generators.hpp> 
#include <boost/uuid/uuid_io.hpp>      
#include "../include/RegexEngine.h"
#define CLIENT_DU

std::string_view getMessage(IWriter::Header::MESSAGE_TYPE mt, std::string_view line)
{
	switch (mt)
	{
	case IWriter::Header::MESSAGE_TYPE::PING:
	{
		std::size_t currPos = 0;
		std::size_t prevPos = currPos;
		while (true)
		{
			prevPos = currPos;
			currPos = line.find(" ", currPos);
			if (currPos == std::string_view::npos)
				break;
			currPos++;
		}
		return line.substr(prevPos);
	}
	
	default:
		return "";
	}
}

int main(int argc, char** argv)
{
	try
	{
		RegexEngine regexEngine;
		IrcClient client("127.0.0.1", "6666", std::make_shared<Reader>());
		client.run();
		std::string line = argc > 1 ? argv[1] : "master";
		if(!line.empty())
			client.SendData(std::make_shared<Writer>(IWriter::Header(IWriter::Header::MESSAGE_TYPE::MASTER, line.size()), line));
		while (std::getline(std::cin, line)) 
		{
			auto [isFound, messageType] = regexEngine.findRegex(line);
			if (isFound)
			{
				auto message = getMessage(messageType, line);
				client.SendData(std::make_shared<Writer>(IWriter::Header(messageType, line.size()), line));
			}
			else
			{
				std::cout << "b³êdna komenda" << std::endl;
			}
		}
	}
	catch (const boost::system::system_error& e) {
		std::cerr << "Error resolving host: " << e.what() << "\n";
		return -1;
	}


	return 0;
}
#include <iostream>
#include <IrcClient.h>
#include <cassert>
#include "../include/SimpleReader.h"
#include "../include/SimpleWriter.h"
#include <IrcProtocol.h>
int main()
{
	try
	{
		IrcClient client("irc.chat4all.org", "6668");
		std::shared_ptr<IReader> reader = std::make_shared<Reader>();

		IrcProtocol ip;
		std::string nickname = "EXAMPLE";
		client.setReader(reader);
		client.RegisterReader(reader);
		client.run();
		
		client.SendData(std::make_shared<Writer>(ip.createNickMessage(nickname)));
		client.SendData(std::make_shared<Writer>(std::format("USER {} 0 * :{}\r\n", nickname, nickname)));

		std::string line;
		while (std::getline(std::cin, line)) {
			std::shared_ptr<IWriter> messageItem = std::make_shared<Writer>(line);
			client.SendData(messageItem);
		}
	}
	catch (const boost::system::system_error& e) {
		std::cerr << "Error resolving host: " << e.what() << "\n";
		return -1;
	}


	return 0;
}
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
#define CLIENT_DU
int main()
{
	try
	{
		IrcClient client("127.0.0.1", "6666", std::make_shared<Reader>());
		client.run();
		std::string line;
		while (std::getline(std::cin, line)) {
	
		}
	}
	catch (const boost::system::system_error& e) {
		std::cerr << "Error resolving host: " << e.what() << "\n";
		return -1;
	}


	return 0;
}
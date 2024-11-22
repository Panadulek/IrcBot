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
int main()
{
	try
	{
		IrcClient client("127.0.0.1", "6666", std::make_shared<Reader>());
		client.run();
		std::string line;
		boost::uuids::random_generator gen;

		boost::uuids::uuid id = gen();
		std::string idStr = boost::uuids::to_string(id);

		std::shared_ptr<IWriter> uuidMsg = std::make_shared<Writer>(IWriter::Header(IWriter::Header::MESSAGE_TYPE::SET_UUID, idStr.size()), idStr);
		client.SendData(uuidMsg);
		while (std::getline(std::cin, line)) {
	
		}
	}
	catch (const boost::system::system_error& e) {
		std::cerr << "Error resolving host: " << e.what() << "\n";
		return -1;
	}


	return 0;
}
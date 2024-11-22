#include <DuServer.h>



int main()
{
	DuServer server;
	server.listen();
	server.run();
	return 0;
}
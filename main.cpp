#include "Server.h"

int main(void)
{
	Server	*server = new Server(8080);
	server->start();

	delete server;
	
	return (0);
}
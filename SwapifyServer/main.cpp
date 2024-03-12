#include "TCPServer.h"

int main(void) {
	TCPServer server = TCPServer(27000);
	server.start();
	return 1;
}
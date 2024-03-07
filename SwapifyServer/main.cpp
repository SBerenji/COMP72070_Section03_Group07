#include "TCPServer.h"
#include "SQLiteDatabase.h"
int main(void) {

	TCPServer server = TCPServer(2750);
	server.start();
	return 1;
}
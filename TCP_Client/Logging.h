
#ifndef REQUESTLOGGER_H
#define REQUESTLOGGER_H
#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <string>
#include <ctime>
#include "Packet.h"
#include "PacketWrapper.h"

class Logging {
public:
	Logging(const std::string& filename);

	void logPacket(char*** TxBuffer);

	void logListingSend(Listing* ptrToList);

private:
	std::string filename;
};

#endif // REQUESTLOGGER_H

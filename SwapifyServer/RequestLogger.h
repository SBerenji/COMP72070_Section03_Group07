
#ifndef REQUESTLOGGER_H
#define REQUESTLOGGER_H

#include <fstream>
#include <string>
#include <ctime>
#include "Packet.h"

class RequestLogger {
public:
    RequestLogger(const std::string& filename);

    void logPacket(Packet& packet);

private:
    std::string filename;
};

#endif // REQUESTLOGGER_H
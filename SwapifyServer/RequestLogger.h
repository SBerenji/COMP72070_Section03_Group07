
#ifndef REQUESTLOGGER_H
#define REQUESTLOGGER_H
#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <string>
#include <ctime>
#include "Packet.h"

class RequestLogger {
public:
    RequestLogger(const std::string& filename);

    void logPacketSend(Packet& packet);


    void logPacketRecv(Packet& packet);
    void logResponse();


    void logListingSend();

    void logImageSend();



private:
    std::string filename;
};

#endif // REQUESTLOGGER_H
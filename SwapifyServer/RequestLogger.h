
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

    void logPacketSend(Packet** pkt);


    void logPacketRecv(Packet** pkt);
    void logResponse(const char* responseMsg);


    void logListingSend(Listing* ptrToList);

    void logImageSend(char** imageBuff, int& imageSize);



private:
    std::string filename;
};

#endif // REQUESTLOGGER_H
#ifndef REQUESTLOGGER_H
#define REQUESTLOGGER_H

#include <fstream>
#include <string>
#include "SQLiteDatabase.h"
class RequestLogger {
public:
    RequestLogger(const std::string& filename) : filename(filename) {}

    void logPacket(Packet& packet) {
        std::ofstream outFile(filename, std::ios::app); // Open in append mode
        if (outFile.is_open()) {
            auto head = packet.GetHead();
            auto body = packet.GetBody();
            auto tail = packet.GetTail();

            outFile << std::dec;
            outFile << "Source:  " << head->Source << std::endl;
            outFile << "Destination: " << head->Destination << std::endl;
            outFile << "Route:  " << head->Route << std::endl;
            outFile << "Authorization:  " << head->Authorization << std::endl;
            outFile << "Length:  " << head->Length << std::endl;

            outFile << "User:  " << body->User << std::endl;
            outFile << "Listing:  " << *(body->Listing) << std::endl;
            outFile << "Message:  " << *(body->Message) << std::endl;

            outFile << "Checksum  " << std::hex << tail->Checksum << std::endl;

            outFile.close();
        }
        else {
            std::cerr << "Error opening log file." << std::endl;
        }
    }

private:
    std::string filename;
};

#endif // REQUESTLOGGER_H
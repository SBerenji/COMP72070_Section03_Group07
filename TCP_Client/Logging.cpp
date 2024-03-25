#include "Logging.h"




Logging::Logging(const std::string& filename) : filename(filename) {}

void  Logging::logPacket(char* TxBuffer) {
    std::ofstream outFile(filename, std::ios::app); // Open in append mode
    if (outFile.is_open()) {
        Packet packet;
        Deserialization(&packet, TxBuffer);

        auto head = packet.GetHead();

        outFile << std::dec;

        time_t currentTime = time(0); //determines the current dat and time on system
        char* timeString = ctime(&currentTime); //converts time to string 

        outFile << "Time: " << timeString << " Source: " << head->Source << " Destination: " << head->Destination << " Route: " << head->Route << std::endl;

        outFile.close();
    }
    else {
        std::cerr << "Error opening log file." << std::endl;
    }
}

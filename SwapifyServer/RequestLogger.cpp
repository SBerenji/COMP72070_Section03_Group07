#include "RequestLogger.h"

    RequestLogger::RequestLogger(const std::string& filename) : filename(filename) {}

    void  RequestLogger::logPacket(Packet& packet) {
        std::ofstream outFile(filename, std::ios::app); // Open in append mode
        if (outFile.is_open()) {
            auto head = packet.GetHead();

            outFile << std::dec;
			
			time_t currentTime = time(0); //determines the current dat and time on system
		
			outFile << "Time: " << currentTime << " Source: " << head->Source << " Destination: " << head->Destination << " Route: " << head->Route << std::endl;

            outFile.close();
        }
        else {
            std::cerr << "Error opening log file." << std::endl;
        }
    }



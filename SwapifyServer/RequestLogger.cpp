#include "RequestLogger.h"

    RequestLogger::RequestLogger(const std::string& filename) : filename(filename) {}

    void  RequestLogger::logPacket(Packet& packet) {
        std::ofstream outFile(filename, std::ios::app); // Open in append mode
        if (outFile.is_open()) {
            auto head = packet.GetHead();

            outFile << std::dec;
			
			time_t currentTime = time(0); //determines the current dat and time on system
			char* timeString = ctime(&currentTime); //converts time to string 
            
            outFile << "Time: " << timeString << " Source: " << "Client" << " Destination: " << "Server" << " Route: " << head->Route << std::endl;

            outFile.close();
        }
        else {
            std::cerr << "Error opening log file." << std::endl;
        }
    }



    void  RequestLogger::logListingSend() {
        std::ofstream outFile(filename, std::ios::app); // Open in append mode
        if (outFile.is_open()) {

            time_t currentTime = time(0); //determines the current dat and time on system
            char* timeString = ctime(&currentTime); //converts time to string 

            outFile << "Time: " << timeString << " Source: " << "Server" << " Destination: " << "Client" << " Route: " << "Listing" << std::endl;

            outFile.close();
        }
        else {
            std::cerr << "Error opening log file." << std::endl;
        }
    }

    void  RequestLogger::logImageSend() {
        std::ofstream outFile(filename, std::ios::app); // Open in append mode
        if (outFile.is_open()) {

            time_t currentTime = time(0); //determines the current dat and time on system
            char* timeString = ctime(&currentTime); //converts time to string 

            outFile << "Time: " << timeString << " Source: " << "Server" << " Destination: " << "Client" << " Route: " << "Image" << std::endl;

            outFile.close();
        }
        else {
            std::cerr << "Error opening log file." << std::endl;
        }
    }

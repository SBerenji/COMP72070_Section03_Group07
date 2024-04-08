#include "RequestLogger.h"

    RequestLogger::RequestLogger(const std::string& filename) : filename(filename) {}

    void  RequestLogger::logPacketSend(Packet** pkt) {
        std::ofstream outFile(filename, std::ios::app); // Open in append mode
        if (outFile.is_open()) {

            outFile << std::dec;
			
			time_t currentTime = time(0); //determines the current dat and time on system
			char* timeString = ctime(&currentTime); //converts time to string 
            
            outFile << "Time: " << timeString << std::endl << "Source: " << (*pkt)->GetHead()->Source << std::endl << "Destination: " << (*pkt)->GetHead()->Destination << std::endl << "Route: " << (*pkt)->GetHead()->Route << std::endl << "Authorization: " << (*pkt)->GetHead()->Authorization << std::endl << "Body Length: " << (*pkt)->GetHead()->Length << std::endl << "User ID: " << (*pkt)->GetBody()->User << std::endl << std::endl;


            /*outFile << "Time: " << timeString << "; Source: " << "Server" << "; Destination: " << "Client" << "; Route: " << head->Route << std::endl;*/

            outFile.close();
        }
        else {
            std::cerr << "Error opening log file." << std::endl;
        }
    }


    void  RequestLogger::logPacketRecv(Packet** pkt) {
        std::ofstream outFile(filename, std::ios::app); // Open in append mode
        if (outFile.is_open()) {

            outFile << std::dec;

            time_t currentTime = time(0); //determines the current dat and time on system
            char* timeString = ctime(&currentTime); //converts time to string 


            outFile << "Time: " << timeString << std::endl << "Source: " << (*pkt)->GetHead()->Source << std::endl << "Destination: " << (*pkt)->GetHead()->Destination << std::endl << "Route: " << (*pkt)->GetHead()->Route << std::endl << "Authorization: " << (*pkt)->GetHead()->Authorization << std::endl << "Body Length: " << (*pkt)->GetHead()->Length << std::endl << "User ID: " << (*pkt)->GetBody()->User << std::endl << std::endl;

            //outFile << "Time: " << timeString << "; Source: " << "Client" << "; Destination: " << "Server" << "; Route: " << head->Route << std::endl;

            outFile.close();
        }
        else {
            std::cerr << "Error opening log file." << std::endl;
        }
    }

    void  RequestLogger::logResponse(const char* responseMsg) {
        std::ofstream outFile(filename, std::ios::app); // Open in append mode
        if (outFile.is_open()) {

            outFile << std::dec;

            time_t currentTime = time(0); //determines the current dat and time on system
            char* timeString = ctime(&currentTime); //converts time to string 

            outFile << "Time: " << timeString << "Response From Client: " << responseMsg << std::endl << std::endl;

            outFile.close();
        }
        else {
            std::cerr << "Error opening log file." << std::endl;
        }
    }


    void  RequestLogger::logListingSend(Listing* ptrToList) {
        std::ofstream outFile(filename, std::ios::app); // Open in append mode
        if (outFile.is_open()) {

            time_t currentTime = time(0); //determines the current dat and time on system
            char* timeString = ctime(&currentTime); //converts time to string 

            outFile << "Time: " << timeString << std::endl << "Title: " << ptrToList->Title << std::endl << "Location: " << ptrToList->Location << std::endl << "Condition: " << ptrToList->Condition << std::endl << "Estimated Worth: " << ptrToList->EstimatedWorth << std::endl << "Delivery: " << ptrToList->Delivery << std::endl << "Swap Idea (Looking For)" << ptrToList->LookingFor << std::endl << std::endl;

            outFile.close();
        }
        else {
            std::cerr << "Error opening log file." << std::endl;
        }
    }

    void  RequestLogger::logImageSend(char** imageBuff, int& imageSize) {
        //std::ofstream outFile(filename, std::ios::app); // Open in append mode

        std::ofstream outFile(filename, std::ios::binary | std::ios::app);

        if (outFile.is_open()) {

            time_t currentTime = time(0); //determines the current dat and time on system
            char* timeString = ctime(&currentTime); //converts time to string .

            std::string time_heading = "Time: ";

            outFile.write(time_heading.c_str(), time_heading.length());

            outFile.write(timeString, sizeof(currentTime));

            outFile << std::endl;

            std::string image_heading = "Image Data: ";

            outFile.write(image_heading.c_str(), image_heading.length());

            outFile.write(*imageBuff, imageSize);

            outFile << std::endl << std::endl;


            //outFile << "Time: " << timeString << " Source: " << "Server" << " Destination: " << "Client" << " Route: " << "Image" << std::endl;

            outFile.close();
        }
        else {
            std::cerr << "Error opening log file." << std::endl;
        }
    }

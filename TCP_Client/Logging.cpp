#include "Logging.h"




Logging::Logging(const std::string& filename) : filename(filename) {}

void Logging::logPacket(char*** TxBuffer) {
    std::ofstream outFile(filename, std::ios::app); // Open in append mode
    if (outFile.is_open()) {
        Packet* pkt = CreatePacket();

        DeserializeHeader(pkt, &TxBuffer);

        /*Deserialization(&packet, TxBuffer);*/

        /*auto head = packet.GetHead();*/

        outFile << std::dec;

        time_t currentTime = time(0); //determines the current date and time on system
        char* timeString = ctime(&currentTime); //converts time to string 

        outFile << "Time: " << timeString << std::endl << "Source: " << pkt->GetHead()->Source << std::endl << "Destination: " << pkt->GetHead()->Destination << std::endl << "Route: " << pkt->GetHead()->Route << std::endl << "Authorization: " << pkt->GetHead()->Authorization << std::endl << "Body Length: " << pkt->GetHead()->Length << std::endl << std::endl;

        outFile.close();


        DestroyPacket(pkt);
    }
    else {
        std::cerr << "Error opening log file." << std::endl;
    }
}






void Logging::logListingSend(Listing* ptrToList) {
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

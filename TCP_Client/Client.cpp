
#include "Packet.h"
//#include "PacketWrapper.h"


//extern "C" __declspec(dllexport) int setupConnection();
extern "C" __declspec(dllexport) SOCKET setupConnection2(WSAStartupFunc wsaStartup = WSAStartup, socketFunc socketfunc = socket, connectFunc connectfunc = connect, WSACleanupFunc wsacleanup = WSACleanup);
extern "C" __declspec(dllexport) int sendData(SOCKET ClientSocket, const char* TxBuffer, int totalSize, SendFunction sendFunc = send);
extern "C" __declspec(dllexport) int CloseSocket(SOCKET ClientSocket, closesocketFunc closeFunc = closesocket, WSACleanupFunc wsacleanupfunc = WSACleanup);


//extern "C" {
//
//    __declspec(dllexport) Packet* CreatePacket();
//}

// extern "C" __declspec(dllexport) int sendData(SOCKET ClientSocket);
//extern "C" __declspec(dllexport) int main(int argc, char* argv[]);

//void Print() {  
//    std::cout << "Hello World!!" << std::endl;
//}
//
//void Print2() {
//    std::cout << "Hello World!!" << std::endl;
//}
//
//int CloseSocket(SOCKET ClientSocket) {
//    closesocket(ClientSocket);
//    // Cleaning up the socket
//
//    std::cout << "Client Socket Closed" << std::endl;
//
//    WSACleanup();
//    // Cleaning up the winsock library
//
//    std::cout << "WSA Closed" << std::endl;
//
//    return 1;
//}



///// CHANGED THE CLOSESOCKET FUNCTION FOR TESTING PURPOSES
int CloseSocket(SOCKET ClientSocket, closesocketFunc closeFunc, WSACleanupFunc wsacleanupfunc) {

    // Cleaning up the socket
    if (closeFunc(ClientSocket) == SOCKET_ERROR)
    {
        std::cerr << "Error closing the socket" << std::endl;
        return 0;
    }

    std::cout << "Client Socket Closed" << std::endl;


    // Cleaning up the winsock library
    if (wsacleanupfunc() == SOCKET_ERROR)
    {
        std::cerr << "Error cleaning WSACleanup" << std::endl;
        return 2;
    }

    std::cout << "WSA Closed" << std::endl;

    return 1;
}


//int sendData(SOCKET ClientSocket, char* TxBuffer, int totalSize) {
//    // The order matters, therefore the send is before receive on Client side.
//    // Send
//
//    //char sendBuffer[20] = "Hello World!";
//    // buffer to store address of the string
//
//    int sendSize = send(ClientSocket, TxBuffer, totalSize, 0);
//    // here we are passing the clientSocket, the address of the string and the size of the string that we want the server to receive
//    // +1 becuase of NULL terminator
//
//    if (sendSize < 0)
//    {
//        std::cout << "Sending Failed" << std::endl;
//
//        closesocket(ClientSocket);
//
//        WSACleanup();
//
//        return 0;
//    }
//
//    else
//    {
//
//        std::cout << "Message Succesfully sent: " << TxBuffer << std::endl;
//    }
//
//    return 1;
//    // returning 1 upon successful completion.
//}


//int sendData(SOCKET ClientSocket, char* TxBuffer, int totalSize) {
//
//
//    int sendSize = send(ClientSocket, TxBuffer, totalSize, 0);
//
//    if (sendSize < 0)
//    {
//        std::cout << "Sending Failed" << std::endl;
//
//        closesocket(ClientSocket);
//
//        WSACleanup();
//
//        return 0;
//    }
//
//    else
//    {
//
//        std::cout << "Message Succesfully sent: " << TxBuffer << std::endl;
//    }
//
//    return 1;
//    // returning 1 upon successful completion.
//}


int sendData(SOCKET ClientSocket, const char* TxBuffer, int totalSize, SendFunction sendFunc){

    int sendSize = sendFunc(ClientSocket, TxBuffer, totalSize, 0);

    if (sendSize < 0) {
        std::cout << "Sending Failed" << std::endl;

        CloseSocket(ClientSocket);
     
        return 0;
    }
    else {
        std::cout << "Message Successfully sent: " << TxBuffer << std::endl;
    }

    return 1;
    // returning 1 upon successful completion.
}




//int setupConnection() {
//    // starting up and configuring the Winsock dynamically linked library
//
//    WSADATA wsaData;
//
//    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//        // providing the address of the object (&wsaData) into the library's startup function
//    {
//        std::cout << "ERROR: Failed to start WSA" << std::endl;
//
//        return 0;
//    }
//
//    else {
//        std::cout << "WSA Started" << std::endl;
//    }
//
//    // Create a Socket
//    // This is to make the call to the server
//
//    SOCKET ClientSocket;
//    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    // here we are using TCP protocol
//
//    if (ClientSocket == INVALID_SOCKET)
//    {
//        WSACleanup();
//
//        std::cout << "ERROR: Failed to create Client Socket" << std::endl;
//
//        return 0;
//    }
//
//    else {
//        std::cout << "Client Socket Created" << std::endl;
//    }
//
//    // Connect
//    // starting up the three way handshake process
//    // The server should have binded and configured a socket for Listening by now.
//
//    sockaddr_in SvrAddr;
//    // This object contains necessary information needed by the library to setup the TCP/IP headers correctly.
//
//    SvrAddr.sin_family = AF_INET;
//    // The family which specifies the protocol that will be used.
//
//    SvrAddr.sin_port = htons(27000);
//    // The client is using the port 27500.
//
//    SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//    // The IPV4 address the server is located on. Since different network we are gonna send the request to the NAT using localhost.
//
//    if ((connect(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR)
//    {
//        closesocket(ClientSocket);
//        WSACleanup();
//
//        std::cout << "ERROR: Connection attempted Failed" << std::endl;
//
//        return 0;
//    }
//
//    else {
//        std::cout << "Connection Established" << std::endl;
//    }
//
//    return 1;
//}
//




SOCKET setupConnection2(WSAStartupFunc wsaStartup, socketFunc socketfunc, connectFunc connectfunc, WSACleanupFunc wsacleanupfunc) {
    // starting up and configuring the Winsock dynamically linked library

    WSADATA wsaData;

    if (wsaStartup(MAKEWORD(2, 2), &wsaData) != 0)
        // providing the address of the object (&wsaData) into the library's startup function
    {
        std::cout << "ERROR: Failed to start WSA" << std::endl;

        return 0;
    }

    else {
        std::cout << "WSA Started" << std::endl;
    }

    // Create a Socket
    // This is to make the call to the server

    SOCKET ClientSocket;
    ClientSocket = socketfunc(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // here we are using TCP protocol

    if (ClientSocket == INVALID_SOCKET)
    {
        wsacleanupfunc();
       
        std::cout << "ERROR: Failed to create ServerSocket" << std::endl;

        return 0;
    }

    else {
        std::cout << "Client Socket Created" << std::endl;
    }

    // Connect
    // starting up the three way handshake process
    // The server should have binded and configured a socket for Listening by now.

    sockaddr_in SvrAddr;
    // This object contains necessary information needed by the library to setup the TCP/IP headers correctly.

    SvrAddr.sin_family = AF_INET;
    // The family which specifies the protocol that will be used.

    SvrAddr.sin_port = htons(27000);
    // The client is using the port 27500.

    SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // The IPV4 address the server is located on. Since different network we are gonna send the request to the NAT using localhost.

    if ((connectfunc(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR)
    {
        CloseSocket(ClientSocket);

        std::cout << "ERROR: Connection attempted Failed" << std::endl;

        return 0;
    }

    else {
        std::cout << "Connection Established" << std::endl;
    }


    return ClientSocket;
}





//int main(int argc, char* argv[]) {
//    
//
//    //// Receive
//    //char recvBuffer[17];
//    //// creating a receive buffer
//
//    //int recvSize = recv(ClientSocket, recvBuffer, sizeof(recvBuffer), 0);
//    //// using the recv to get the message from the server.
//
//    //if (recvSize < 0)
//    //{
//    //    std::cout << "Receiving Failed";
//
//    //    closesocket(ClientSocket);
//
//    //    WSACleanup();
//
//    //    return 0;
//    //}
//
//    //else
//    //{
//    //    std::cout << "Message Succesfully Received: " << recvBuffer << std::endl;
//    //}
//
//    //closesocket(ClientSocket);
//    //// Cleaning up the socket
//
//    //std::cout << "Client Socket Closed" << std::endl;
//
//    //WSACleanup();
//    //// Cleaning up the winsock library
//
//    //std::cout << "WSA Closed" << std::endl;
//
//    //return 1;
//    // returning 1 upon successful completion.
//}





// **********************************************************88





#ifdef __cplusplus
extern "C" {
#endif

    __declspec(dllexport) Packet* CreatePacket() {
        Packet* Pkt = new Packet();

        Pkt->GetBody()->Data = nullptr;


        memset(Pkt->GetHead(), 0, sizeof(*(Pkt->GetHead())));
        memset(&(Pkt->GetBody()->User), 0, sizeof(Pkt->GetBody()->User));
        memset(&(Pkt->GetTail()->Checksum), 0, sizeof(Pkt->GetTail()->Checksum));


        return Pkt;
    }

    __declspec(dllexport) void DestroyPacket(Packet*& Pkt) {
        delete Pkt;
        Pkt = nullptr;   // nullifying the pointer
    }


    __declspec(dllexport) void FreeBuffer(char*& Buffer) {
        delete Buffer;
        Buffer = nullptr;   // nullifying the pointer
    }


    __declspec(dllexport) char* AllocateHeaderPtr() {
        Packet p;

        char* ptr = new char[sizeof(p.Head)];

        memset(ptr, 0, sizeof(p.Head));

        return ptr;
    }


    __declspec(dllexport) void Display(Packet* Pkt, std::ostream& os)
    {
        os << std::dec;
        os << "Source:  " << Pkt->GetHead()->Source << std::endl;
        os << "Destination: " << Pkt->GetHead()->Destination << std::endl;
        os << "Route:  " << Pkt->GetHead()->Route << std::endl;
        os << "Authorization:  " << Pkt->GetHead()->Authorization << std::endl;
        os << "Length:  " << Pkt->GetHead()->Length << std::endl;

        os << "User:  " << Pkt->GetBody()->User << std::endl;
        os << "Data:  " << Pkt->GetBody()->Data << std::endl;

        os << "Checksum  " << std::hex << Pkt->GetTail()->Checksum << std::endl;
    }

    __declspec(dllexport) void Deserialization(Packet* Pkt, char* src) {
        memcpy(Pkt->GetHead(), src, sizeof(*(Pkt->GetHead())));

        Pkt->GetBody()->Data = new char[Pkt->GetHead()->Length];

        memcpy(&(Pkt->GetBody()->User), src + sizeof(*(Pkt->GetHead())), sizeof(Pkt->GetBody()->User));
        memcpy(Pkt->GetBody()->Data, src + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User), Pkt->GetHead()->Length);

        memcpy(Pkt->GetTail(), src + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User) + Pkt->GetHead()->Length, sizeof(*(Pkt->GetTail())));
    }


    __declspec(dllexport) void SetHeader(Packet* Pkt, void* Head) {
        memcpy(Pkt->GetHead(), Head, sizeof(*(Pkt->GetHead())));
    }


    __declspec(dllexport) void SetBody(Packet* Pkt, unsigned char User, char* Data, int DataSize) {
        Pkt->GetBody()->User = User;


        DataSize++;

        Pkt->GetBody()->Data = new char[DataSize];
        Pkt->GetBody()->Data[DataSize - 1] = '\0';


        memcpy(Pkt->GetBody()->Data, Data, DataSize);

        Pkt->GetHead()->Length = DataSize;
    }


    __declspec(dllexport) unsigned int CalculateChecksum() {
        unsigned int checksum;

        memset(&(checksum), 0, sizeof(checksum));

        checksum = 0xFF00FF00;

        return checksum;
    }


    __declspec(dllexport) char* SerializeData(Packet* Pkt, int& TotalSize) {
        if (Pkt->GetTxBuffer()) {
            delete[] Pkt->GetTxBuffer();
        }

        Pkt->GetTail()->Checksum = CalculateChecksum();

        TotalSize = sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User) + Pkt->GetHead()->Length + sizeof(*(Pkt->GetTail()));

        Pkt->GetTxBuffer() = new char[TotalSize];


        memcpy(Pkt->GetTxBuffer(), Pkt->GetHead(), sizeof(*(Pkt->GetHead())));

        memcpy(Pkt->GetTxBuffer() + sizeof(*(Pkt->GetHead())), &(Pkt->GetBody()->User), sizeof(Pkt->GetBody()->User));
        memcpy(Pkt->GetTxBuffer() + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User), Pkt->GetBody()->Data, Pkt->GetHead()->Length);


        memcpy(Pkt->GetTxBuffer() + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User) + Pkt->GetHead()->Length, Pkt->GetTail(), sizeof(*(Pkt->GetTail())));


        return Pkt->GetTxBuffer();
    }




#ifdef __cplusplus
}
#endif
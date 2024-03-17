#include "Packet.h"
#include "PacketWrapper.h"


Packet* CreatePacket() {
    Packet* Pkt = new Packet();

    Pkt->GetBody()->Data = nullptr;


    memset(Pkt->GetHead(), 0, sizeof(*(Pkt->GetHead())));
    memset(&(Pkt->GetBody()->User), 0, sizeof(Pkt->GetBody()->User));
    memset(&(Pkt->GetTail()->Checksum), 0, sizeof(Pkt->GetTail()->Checksum));


    return Pkt;
}

void DestroyPacket(Packet*& Pkt) {
    delete Pkt;
    Pkt = nullptr;   // nullifying the pointer
}


void FreeBuffer(char*& Buffer) {
    delete Buffer;
    Buffer = nullptr;   // nullifying the pointer
}

char* AllocateHeaderPtr() {
    Packet p;

    char* ptr = new char[sizeof(p.Head)];

    memset(ptr, 0, sizeof(p.Head));

    return ptr;
}


void Display(Packet* Pkt, std::ostream& os)
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

void Deserialization(Packet* Pkt, char* src) {
    memcpy(Pkt->GetHead(), src, sizeof(*(Pkt->GetHead())));

    Pkt->GetBody()->Data = new char[Pkt->GetHead()->Length];

    memcpy(&(Pkt->GetBody()->User), src + sizeof(*(Pkt->GetHead())), sizeof(Pkt->GetBody()->User));
    memcpy(Pkt->GetBody()->Data, src + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User), Pkt->GetHead()->Length);

    memcpy(Pkt->GetTail(), src + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User) + Pkt->GetHead()->Length, sizeof(*(Pkt->GetTail())));
}


void SetHeader(Packet* Pkt, void* Head) {
    memcpy(Pkt->GetHead(), Head, sizeof(*(Pkt->GetHead())));
}


void SetBody(Packet* Pkt, unsigned char User, char* Data, int DataSize) {
    Pkt->GetBody()->User = User;


    DataSize++;

    Pkt->GetBody()->Data = new char[DataSize];
    Pkt->GetBody()->Data[DataSize - 1] = '\0';


    memcpy(Pkt->GetBody()->Data, Data, DataSize);

    Pkt->GetHead()->Length = DataSize;
}


unsigned int CalculateChecksum() {
    unsigned int checksum;

    memset(&(checksum), 0, sizeof(checksum));

    checksum = 0xFF00FF00;

    return checksum;
}


 char* SerializeData(Packet* Pkt, int& TotalSize) {
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


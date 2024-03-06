#pragma once

#include "Packet.h"

#include <memory>
#include <iostream>
#include <fstream>


#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport) Packet* CreatePacket() {
		Packet* Pkt = new Packet();
		
		Pkt->GetBody()->Listing = nullptr;
		Pkt->GetBody()->Message = nullptr;


		memset(Pkt->GetHead(), 0, sizeof(*(Pkt->GetHead())));
		memset(&(Pkt->GetBody()->User), 0, sizeof(Pkt->GetBody()->User));
		memset(&(Pkt->GetTail()->Checksum), 0, sizeof(Pkt->GetTail()->Checksum));


		return Pkt;
	}

	__declspec(dllexport) void DestroyPacket(Packet* Pkt) {
		delete Pkt;
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
		os << "Listing:  " << *(Pkt->GetBody()->Listing) << std::endl;
		os << "Message:  " << *(Pkt->GetBody()->Message) << std::endl;

		os << "Checksum  " << std::hex << Pkt->GetTail()->Checksum << std::endl;
	}

	__declspec(dllexport) void Deserialization(Packet* Pkt, char* src) {
		memcpy(Pkt->GetHead(), src, sizeof(*(Pkt->GetHead())));

		Pkt->GetBody()->Listing = new char[(unsigned int)(Pkt->GetHead()->Length / 2)];
		Pkt->GetBody()->Message = new char[(unsigned int)(Pkt->GetHead()->Length / 2)];

		memcpy(&(Pkt->GetBody()->User), src + sizeof(*(Pkt->GetHead())), sizeof(Pkt->GetBody()->User));
		memcpy(Pkt->GetBody()->Listing, src + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User), (unsigned int)(Pkt->GetHead()->Length / 2));
		memcpy(Pkt->GetBody()->Message, src + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User) + (unsigned int)(Pkt->GetHead()->Length / 2), (unsigned int)(Pkt->GetHead()->Length / 2));

		memcpy(Pkt->GetTail(), src + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User) + Pkt->GetHead()->Length, sizeof(*(Pkt->GetTail())));
	}


	__declspec(dllexport) void SetData(Packet* Pkt, char* ListingsData, char* MessageData, int ListingSize, int MessageSize) {
		ListingSize++;
		MessageSize++;

		Pkt->GetBody()->Listing = new char[ListingSize];
		Pkt->GetBody()->Listing[ListingSize - 1] = '\0';

		Pkt->GetBody()->Message = new char[MessageSize];
		Pkt->GetBody()->Message[MessageSize - 1] = '\0';


		memcpy(Pkt->GetBody()->Listing, ListingsData, ListingSize);
		memcpy(Pkt->GetBody()->Message, MessageData, MessageSize);

		Pkt->GetHead()->Length = MessageSize + ListingSize;
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


		TotalSize = sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User) + Pkt->GetHead()->Length + sizeof(*(Pkt->GetTail()));

		Pkt->GetTxBuffer() = new char[TotalSize];


		memcpy(Pkt->GetTxBuffer(), Pkt->GetHead(), sizeof(*(Pkt->GetHead())));

		memcpy(Pkt->GetTxBuffer() + sizeof(*(Pkt->GetHead())), &(Pkt->GetBody()->User), sizeof(Pkt->GetBody()->User));
		memcpy(Pkt->GetTxBuffer() + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User), Pkt->GetBody()->Listing, (unsigned int)(Pkt->GetHead()->Length) / 2);
		memcpy(Pkt->GetTxBuffer() + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User) + (unsigned int)((Pkt->GetHead()->Length) / 2), Pkt->GetBody()->Message, (unsigned int)(Pkt->GetHead()->Length) / 2);


		Pkt->GetTail()->Checksum = CalculateChecksum();

		memcpy(Pkt->GetTxBuffer() + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User) + Pkt->GetHead()->Length, Pkt->GetTail(), sizeof(*(Pkt->GetTail())));


		return Pkt->GetTxBuffer();
	}


	

#ifdef __cplusplus
}
#endif
#pragma once
#include <memory>
#include <iostream>
#include <fstream>
#include "Packet.h"

#ifdef __cplusplus
extern "C" {
#endif


	__declspec(dllexport) Packet* CreatePacket();

	__declspec(dllexport) void DestroyPacket(Packet* Pkt);


	__declspec(dllexport) void Display(Packet* Pkt, std::ostream& os);

	__declspec(dllexport) void Deserialization(Packet* Pkt, char* src);


	__declspec(dllexport) void SetData(Packet* Pkt, char* ListingsData, char* MessageData, int ListingSize, int MessageSize);


	__declspec(dllexport) unsigned int CalculateChecksum();


	__declspec(dllexport) char* SerializeData(Packet* Pkt, int& TotalSize);
#ifdef __cplusplus
}
#endif

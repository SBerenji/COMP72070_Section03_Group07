#pragma once

#include "Packet.h"

#include <memory>
#include <iostream>
#include <fstream>


#ifdef __cplusplus
extern "C" {
#endif

	__declspec (dllexport) void serializeLoginData(char* BodyBuffer, struct LogIn login);

	/*__declspec(dllexport) void serializeSignupData(char* BodyBuffer, )*/

	__declspec(dllexport) char* AllocateLoginPtr();


	__declspec(dllexport) char* AllocateHeapMemory(int size);

	__declspec(dllexport) void SerializeStaticDataToBuffer(char* heapBuffer, char* username, char* password, char* email);

	__declspec(dllexport) void CopyBufferToHeap(char* heapBuffer, char* srcBuffer, int size);

	__declspec(dllexport) char* AllocateSignupPtr(int imageSize);

	__declspec(dllexport) char* AllocateListingPtr(int imageSize);

	__declspec(dllexport) Packet* CreatePacket();

	__declspec(dllexport) void DestroyPacket(Packet*& Pkt);


	__declspec(dllexport) void FreeBuffer(char*& Buffer);


	__declspec(dllexport) char* AllocateHeaderPtr();


	__declspec(dllexport) void Display(Packet* Pkt, std::ostream& os);

	__declspec(dllexport) void DeserializeHeader(Packet* pkt, char** TxBuffer);

	__declspec(dllexport) void DeserializePostCountBuffer(Packet* pkt, char* src, int& numberOfPosts);

	__declspec(dllexport) void Deserialization(Packet* Pkt, char* src);

	__declspec(dllexport) void DeserializationWithoutTail(Packet* Pkt, char* src);

	__declspec(dllexport) unsigned int DeserializeClientID(char* src);

	__declspec(dllexport) unsigned int DeserializeHeaderLengthMember(char* RxBuffer);

	__declspec(dllexport) unsigned int DeserializeImageLength(char* RxBuffer, int offset);

	__declspec(dllexport) void CopyImageFromRawBufferToByteArray(char* RxBuffer, char* imageArray, int imageSize);


	__declspec(dllexport) void SetHeader(Packet* Pkt, void* Head);


	__declspec(dllexport) void SetBody(Packet* Pkt, unsigned int User, char* Data, int DataSize);


	__declspec(dllexport) unsigned int CalculateChecksum();

	__declspec(dllexport) char* SerializeMyPostCountData(Packet* Pkt, int& totalSize);

	__declspec(dllexport) char* SerializeData(Packet* Pkt, int& TotalSize);

	__declspec(dllexport) char* SerializeHeaderOnlyPkt(Packet* Pkt, int& TotalSize);


	

#ifdef __cplusplus
}
#endif
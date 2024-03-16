#pragma once

#include "Packet.h"

#include <memory>
#include <iostream>
#include <fstream>


#ifdef __cplusplus
extern "C" {
#endif

	__declspec (dllexport) void serializeLoginData(char* BodyBuffer, struct LogIn login) {
		memcpy(BodyBuffer, &login, sizeof(login));
	}

	/*__declspec(dllexport) void serializeSignupData(char* BodyBuffer, )*/

	__declspec(dllexport) char* AllocateLoginPtr() {
		char* BodyBuffer = new char[sizeof(LogIn)];

		memset(BodyBuffer, 0, sizeof(LogIn));

		return BodyBuffer;
	}


	__declspec(dllexport) char* AllocateHeapMemory(int size) {
		char* TxBuffer = new char[size];

		memset(TxBuffer, 0, size);

		return TxBuffer;
	}

	__declspec(dllexport) void SerializeStaticDataToBuffer(char* heapBuffer, char* username, char* password, char* email) {
		memcpy(heapBuffer, username, sizeof(signup.username));
		memcpy(heapBuffer + sizeof(signup.username), password, sizeof(signup.password));
		memcpy(heapBuffer + sizeof(signup.username) + sizeof(signup.password), email, sizeof(signup.email));

		/*memcpy(heapBuffer, &signup, sizeof(signup) - sizeof(signup.ImageStructArray));*/
	}

	__declspec(dllexport) void CopyBufferToHeap(char* heapBuffer, char* srcBuffer, int size) {
		memcpy(heapBuffer, srcBuffer, size);
	}

	__declspec(dllexport) char* AllocateSignupPtr(int imageSize) {
		char* BodyBuffer = new char[(sizeof(signup.username) + sizeof(signup.password) + sizeof(signup.email)) + imageSize];

		memset(BodyBuffer, 0, (sizeof(signup.username) + sizeof(signup.password) + sizeof(signup.email)) + imageSize);

		return BodyBuffer;
	}


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

		Pkt = nullptr;
	}


	__declspec(dllexport) void FreeBuffer(char*& Buffer) {
		delete Buffer;

		Buffer = nullptr;
	}


	__declspec(dllexport) char* AllocateHeaderPtr() {
		Packet p;

		char* ptr =  new char[sizeof(p.Head)];

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
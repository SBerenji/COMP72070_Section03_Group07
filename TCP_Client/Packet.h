#pragma once
#include <memory>
#include <iostream>
#include <fstream>
#include <iostream>

#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

#include <fstream>
#include <string>


typedef int (*WSAStartupFunc)(WORD, LPWSADATA);
typedef SOCKET (*socketFunc)(int, int, int);
typedef int (*connectFunc)(SOCKET, const struct sockaddr*, int);


class __declspec(dllexport) Packet {
public:
	struct Header {
		char Source[20];
		char Destination[20];
		char Route[10];
		bool Authorization;
		unsigned int Length;
	} Head;

private:
	struct Body {
		unsigned char User;
		char* Data;
	} BodyContents;

	struct Tail {
		unsigned int Checksum;
	} TailContents;


	char* TxBuffer;

public:
	Packet() {

	}

	Header* GetHead() {
		return &(this->Head);
	}

	Body* GetBody() {
		return &(this->BodyContents);
	}

	Tail* GetTail() {
		return &(this->TailContents);
	}

	char*& GetTxBuffer() {
		return this->TxBuffer;
	}

	/*Packet* CreatePacket() {
		return new Packet();
	}*/

	//void DestroyPacket(Packet* Pkt) {
	//	delete Pkt;
	//}

	//void Display(Packet* Pkt, std::ostream& os)
	//{
	//	/*os << std::dec;
	//	os << "Source:  " << this->Head.Source << std::endl;
	//	os << "Destination: " << this->Head.Destination << std::endl;
	//	os << "Route:  " << this->Head.Route << std::endl;
	//	os << "Authorization:  " << this->Head.Authorization<< std::endl;
	//	os << "Length:  " << this->Head.Length << std::endl;

	//	os << "User:  " << this->Body.User << std::endl;
	//	os << "Listing:  " << this->Body.Listing << std::endl;
	//	os << "Message:  " << this->Body.Message << std::endl;

	//	os << "Checksum  " << std::hex << this->Tail.Checksum << std::endl;*/
	//}


	//Packet(char* src) {
	//	/*memcpy(&(this->Head), src, sizeof(Header));

	//	this->Body.Listing = new char[this->Head.Length];
	//	this->Body.Message = new char[this->Head.Length];

	//	memcpy(&(this->Body.User), src + sizeof(Header), sizeof(this->Body.User));
	//	memcpy(this->Body.Listing, src + sizeof(Header) + sizeof(this->Body.User), (unsigned int)(this->Head.Length / 2));
	//	memcpy(this->Body.Message, src + sizeof(Header) + sizeof(this->Body.User) + (this->Head.Length / 2), (this->Head.Length / 2));

	//	memcpy(&(this->Tail), src + sizeof(this->Head) + sizeof(this->Body.User) + (this->Head.Length), sizeof(this->Tail));*/
	//}


	//void SetData(char* ListingsData, char* MessageData, int ListingSize, int MessageSize) {
	//	/*ListingSize++;
	//	MessageSize++;

	//	this->Body.Listing = new char[ListingSize];
	//	this->Body.Listing[ListingSize - 1] = '\0';

	//	this->Body.Message = new char[MessageSize];
	//	this->Body.Message[MessageSize - 1] = '\0';

	//	memcpy(this->Body.Listing, ListingsData, ListingSize);
	//	memcpy(this->Body.Message, MessageData, MessageSize);

	//	this->Head.Length = MessageSize + ListingSize;*/
	//}


	/*char* SerializeData(int& TotalSize) {
		if (this->TxBuffer) {
			delete[] TxBuffer;
		}

		TotalSize = sizeof(Header) + sizeof(this->Body.User) + this->Head.Length + sizeof(this->Tail);

		this->TxBuffer = new char[TotalSize];

		memcpy(TxBuffer, &(this->Head), sizeof(this->Head));

		memcpy(TxBuffer + sizeof(this->Head), &(this->Body.User), sizeof(this->Body.User));
		memcpy(TxBuffer + sizeof(this->Head) + sizeof(this->Body.User), this->Body.Listing, (unsigned int)(this->Head.Length / 2));
		memcpy(TxBuffer + sizeof(this->Head) + sizeof(this->Body.User) + (unsigned int)(this->Head.Length / 2), this->Body.Message, (unsigned int)(this->Head.Length));

		this->Tail.Checksum = CalculateChecksum();

		memcpy(TxBuffer + sizeof(this->Head) + sizeof(this->Body.User) + this->Head.Length, &(this->Tail), sizeof(this->Tail));

		return this->TxBuffer;
	}*/


	/*unsigned int CalculateChecksum() {
		unsigned int checksum;

		memset(&(checksum), 0, sizeof(checksum));

		checksum = 0xFF00FF00;

		return checksum;
	}*/
};
#pragma once
#include <memory>
#include <iostream>
#include <fstream>
const int EmptyPktSize = 6;

class __declspec(dllexport) Packet {
	struct Header {
		char Source[20];
		char Destination[20];
		char Route[20];
		bool Authorization;
		unsigned char Length;
	} Head;

	struct Body {
		char* User;
		char* Listing;
		char* Message;
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

	Packet* CreatePacket() {
		return new Packet();
	}

};
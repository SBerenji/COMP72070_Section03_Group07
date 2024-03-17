#pragma once

#include <memory>
#include <iostream>
#include <fstream>

struct LogIn
{
	char username[10];
	char password[20];
};

struct SignUp
{
	char username[10];
	char password[20];
	char email[40];
	char* ImageStructArray;
};

struct SignUpCheck {
	char username[10];
	char email[40];
};

struct Listing
{
	char Image[100];
	char Title[100];
	char EstimatedWorth[100];
	char Location[100];
	char Condition[100];
	char Delivery[100];
	char LookingFor[100];
	char ListingDate[20];

	char* ImageStructArray;
};

class Packet {
	struct Header {
		char Source[20];
		char Destination[20];
		char Route[40];
		bool Authorization;
		unsigned int Length;
	} Head;

	struct Body {
		unsigned char User;
		char* Data;
	} BodyContents;

	struct Tail {
		unsigned int Checksum;
	} TailContents;


	char* TxBuffer;


	//void* structPtr = nullptr;

public:
	Packet() {

	};

	Header* GetHead();

	Body* GetBody();

	Tail* GetTail();

	char*& GetTxBuffer();

	//void* GetStructPtr();


	/*Packet(char* src) {
		memcpy(&(this->Head), src, sizeof(Header));

		this->Body.Listing = new char[this->Head.Length];
		this->Body.Message = new char[this->Head.Length];

		memcpy(&(this->Body.User), src + sizeof(Header), sizeof(this->Body.User));
		memcpy(this->Body.Listing, src + sizeof(Header) + sizeof(this->Body.User), (unsigned int)(this->Head.Length / 2));
		memcpy(this->Body.Message, src + sizeof(Header) + sizeof(this->Body.User) + (this->Head.Length / 2), (this->Head.Length / 2));

		memcpy(&(this->Tail), src + sizeof(this->Head) + sizeof(this->Body.User) + (this->Head.Length), sizeof(this->Tail));
	}*/


	/*void SetData(char* ListingsData, char* MessageData, int ListingSize, int MessageSize) {
		ListingSize++;
		MessageSize++;

		this->Body.Listing = new char[ListingSize];
		this->Body.Listing[ListingSize - 1] = '\0';

		this->Body.Message = new char[MessageSize];
		this->Body.Message[MessageSize - 1] = '\0';

		memcpy(this->Body.Listing, ListingsData, ListingSize);
		memcpy(this->Body.Message, MessageData, MessageSize);

		this->Head.Length = MessageSize + ListingSize;
	}*/


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


Packet* CreatePacket();



void DestroyPacket(Packet* Pkt);

void Display(Packet* Pkt, std::ostream& os, LogIn& log, SignUp& signup);


void Deserialization(Packet* Pkt, char* src, LogIn& log, SignUp& sign, SignUpCheck& check);

void SetHeaderInformation(Packet* pkt, char* source, int source_size, char* destination, int destination_size, char* Route, int Route_size, bool Authorization, unsigned int length);

void SetHeader(Packet* Pkt, void* Head);


void SetBody(Packet* Pkt, unsigned char User, char* Data, int DataSize);


unsigned int CalculateChecksum();

char* SerializeUserCheckingData(Packet* Pkt, int& TotalSize);

char* SerializeData(Packet* Pkt, int& TotalSize);
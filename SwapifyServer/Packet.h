#pragma once

#include <memory>
#include <iostream>
#include <fstream>

#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

#include <fstream>
#include <string>
/////// THESE ALLOW US TO REPLACE THE CALL TO THE ACTUAL SEND FUNCTION WITH A MOCK FUNCTION WHEN TESTING
/////// These pointers to funcction will allow us to replace the call to the actual functios (like connect, send, etc.) with a mock function when testing

typedef int (*WSAStartupFunc)(WORD, LPWSADATA);
typedef SOCKET(*socketFunc)(int, int, int);
typedef int (*connectFunc)(SOCKET, const struct sockaddr*, int);
typedef int (*closesocketFunc)(SOCKET);

typedef int (*WSACleanupFunc)();

typedef int (*SendFunction)(SOCKET, const char*, int, int);


struct UserCredentials {
	char username[10];
	char password[20];
	char email[20];

	char* imageStructArray;
};

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
	char Title[200];
	char Location[200];
	char Condition[200];
	char EstimatedWorth[200];
	char Delivery[200];
	char LookingFor[200];

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
		unsigned int User;
		char* Data;
	} BodyContents;

	struct Tail {
		unsigned int Checksum;
	} TailContents;


	char* TxBuffer;

public:
	Packet() {

	};

	Header* GetHead();

	Body* GetBody();

	Tail* GetTail();

	char*& GetTxBuffer();
};


Packet* CreatePacket();

void Deserialization(Packet* Pkt, char* src);

void DestroyPacket(Packet* Pkt);

void Display(Packet* Pkt, std::ostream& os, LogIn& log, SignUp& signup, Listing& list);


void Deserialization(Packet* Pkt, char* src, LogIn& log, SignUp& sign, SignUpCheck& check, Listing& list);

void SetHeaderInformation(Packet* pkt, char* source, int source_size, char* destination, int destination_size, char* Route, int Route_size, bool Authorization, unsigned int length);

void SetHeader(Packet* Pkt, void* Head);


void SetBody(Packet* Pkt, unsigned int User, char* Data, int DataSize);


unsigned int CalculateChecksum();

char* SerializeUserCheckingData(Packet* Pkt, int& TotalSize);

char* SerializeData(Packet* Pkt, int& TotalSize);
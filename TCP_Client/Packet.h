#pragma once
#include <memory>
#include <iostream>
#include <fstream>
#include <iostream>

#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

#include <fstream>
#include <string>

/////// THESE ALLOW US TO REPLACE THE CALL TO THE ACTUAL SEND FUNCTION WITH A MOCK FUNCTION WHEN TESTING
/////// These pointers to funcction will allow us to replace the call to the actual functios (like connect, send, etc.) with a mock function when testing

typedef int (*WSAStartupFunc)(WORD, LPWSADATA);
typedef SOCKET (*socketFunc)(int, int, int);
typedef int (*connectFunc)(SOCKET, const struct sockaddr*, int);
typedef int (*closesocketFunc)(SOCKET);

typedef int (*WSACleanupFunc)();

typedef int (*SendFunction)(SOCKET, const char*, int, int);



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
		// Empty constructor body
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

};
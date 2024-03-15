#include "Packet.h"

Packet::Header* Packet::GetHead() {
	return &(this->Head);
};

Packet::Body* Packet::GetBody() {
	return &(this->BodyContents);
};

Packet::Tail* Packet::GetTail() {
	return &(this->TailContents);
};

char*& Packet::GetTxBuffer() {
	return this->TxBuffer;
};

//void* Packet::GetStructPtr() {
//	return this->structPtr;
//}


Packet* CreatePacket() {
	Packet* Pkt = new Packet();

	Pkt->GetBody()->Data = nullptr;


	memset(Pkt->GetHead(), 0, sizeof(*(Pkt->GetHead())));
	memset(&(Pkt->GetBody()->User), 0, sizeof(Pkt->GetBody()->User));
	memset(&(Pkt->GetTail()->Checksum), 0, sizeof(Pkt->GetTail()->Checksum));


	return Pkt;
};

void DestroyPacket(Packet* Pkt) {
	delete Pkt;
};

void Display(Packet* Pkt, std::ostream& os, LogIn& log, SignUp& signup)
{
	os << std::dec;
	os << "Source:  " << Pkt->GetHead()->Source << std::endl;
	os << "Destination: " << Pkt->GetHead()->Destination << std::endl;
	os << "Route:  " << Pkt->GetHead()->Route << std::endl;
	os << "Authorization:  " << Pkt->GetHead()->Authorization << std::endl;
	os << "Length:  " << Pkt->GetHead()->Length << std::endl;

	os << "User:  " << Pkt->GetBody()->User << std::endl;

	os << "Username: " << signup.username << std::endl;
	os << "Password: " << signup.password << std::endl;
	os << "Email: " << signup.email << std::endl;
	os << "Image Array: " << *(signup.ImageStructArray) << std::endl;

	os << "Data:  " << Pkt->GetBody()->Data << std::endl;

	os << "Checksum  " << std::hex << Pkt->GetTail()->Checksum << std::endl;
};


void Deserialization(Packet* Pkt, char* src, LogIn& log, SignUp& sign) {
	memcpy(Pkt->GetHead(), src, sizeof(*(Pkt->GetHead())));

	Pkt->GetBody()->Data = new char[Pkt->GetHead()->Length];

	memcpy(&(Pkt->GetBody()->User), src + sizeof(*(Pkt->GetHead())), sizeof(Pkt->GetBody()->User));

	char* Data = nullptr;

	memcpy(Pkt->GetBody()->Data, src + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User), Pkt->GetHead()->Length);

	Data = Pkt->GetBody()->Data;

	memcpy(Pkt->GetTail(), src + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User) + Pkt->GetHead()->Length, sizeof(*(Pkt->GetTail())));


	if (strcmp(Pkt->GetHead()->Route, "LOGIN") == 0) {

		memcpy(&log, Pkt->GetBody()->Data, sizeof(log));
	}

	else if (strcmp(Pkt->GetHead()->Route, "SIGNUP") == 0) {
		char* data = nullptr;
		data = Pkt->GetBody()->Data;

		char* username = nullptr;
		char* password = nullptr;
		char* email = nullptr;

		memcpy(&(sign.username), Pkt->GetBody()->Data, sizeof(sign.username));
		username = sign.username;

		memcpy(&(sign.password), Pkt->GetBody()->Data + sizeof(sign.username), sizeof(sign.password));
		password = sign.password;

		memcpy(&(sign.email), Pkt->GetBody()->Data + sizeof(sign.username) + sizeof(sign.password), sizeof(sign.email));
		email = sign.email;

		char* imageArray = nullptr;

		sign.ImageStructArray = new char[Pkt->GetHead()->Length - (sizeof(sign.username) + sizeof(sign.password) + sizeof(sign.email))];
		imageArray = sign.ImageStructArray;


		memset(sign.ImageStructArray, 0, Pkt->GetHead()->Length - (sizeof(sign) - sizeof(sign.ImageStructArray)));

		memcpy(sign.ImageStructArray, Pkt->GetBody()->Data + (sizeof(sign.username) + sizeof(sign.password) + sizeof(sign.email)), Pkt->GetHead()->Length - (sizeof(sign.username) + sizeof(sign.password) + sizeof(sign.email)));
	}
};



void SetHeader(Packet* Pkt, void* Head) {
	memcpy(Pkt->GetHead(), Head, sizeof(*(Pkt->GetHead())));
};


void SetBody(Packet* Pkt, unsigned char User, char* Data, int DataSize) {
	Pkt->GetBody()->User = User;


	DataSize++;

	Pkt->GetBody()->Data = new char[DataSize];
	Pkt->GetBody()->Data[DataSize - 1] = '\0';


	memcpy(Pkt->GetBody()->Data, Data, DataSize);

	Pkt->GetHead()->Length = DataSize;
};



//void SetData(Packet* Pkt, char* ListingsData, char* MessageData, int ListingSize, int MessageSize) {
//	ListingSize++;
//	MessageSize++;
//
//	Pkt->GetBody()->Listing = new char[ListingSize];
//	Pkt->GetBody()->Listing[ListingSize - 1] = '\0';
//
//	Pkt->GetBody()->Message = new char[MessageSize];
//	Pkt->GetBody()->Message[MessageSize - 1] = '\0';
//
//
//	memcpy(Pkt->GetBody()->Listing, ListingsData, ListingSize);
//	memcpy(Pkt->GetBody()->Message, MessageData, MessageSize);
//
//	Pkt->GetHead()->Length = MessageSize + ListingSize;
//}


unsigned int CalculateChecksum() {
	unsigned int checksum;

	memset(&(checksum), 0, sizeof(checksum));

	checksum = 0xFF00FF00;

	return checksum;
};


char* SerializeData(Packet* Pkt, int& TotalSize) {
	if (Pkt->GetTxBuffer()) {
		delete[] Pkt->GetTxBuffer();
	}


	TotalSize = sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User) + Pkt->GetHead()->Length + sizeof(*(Pkt->GetTail()));

	Pkt->GetTxBuffer() = new char[TotalSize];


	memcpy(Pkt->GetTxBuffer(), Pkt->GetHead(), sizeof(*(Pkt->GetHead())));

	memcpy(Pkt->GetTxBuffer() + sizeof(*(Pkt->GetHead())), &(Pkt->GetBody()->User), sizeof(Pkt->GetBody()->User));
	memcpy(Pkt->GetTxBuffer() + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User), Pkt->GetBody()->Data, Pkt->GetHead()->Length);


	Pkt->GetTail()->Checksum = CalculateChecksum();

	memcpy(Pkt->GetTxBuffer() + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User) + Pkt->GetHead()->Length, Pkt->GetTail(), sizeof(*(Pkt->GetTail())));


	return Pkt->GetTxBuffer();
};
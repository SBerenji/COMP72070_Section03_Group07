#include "PacketWrapper.h"


//void serializeLoginData(char* BodyBuffer, struct LogIn login) {
//	memcpy(BodyBuffer, &login, sizeof(login));
//}

/*__declspec(dllexport) void serializeSignupData(char* BodyBuffer, )*/

char* AllocateLoginPtr() {
	char* BodyBuffer = new char[sizeof(LogIn)];

	memset(BodyBuffer, 0, sizeof(LogIn));

	return BodyBuffer;
}


char* AllocateHeapMemory(int size) {
	char* TxBuffer = new char[size];

	memset(TxBuffer, 0, size);

	return TxBuffer;
}

//void SerializeStaticDataToBuffer(char* heapBuffer, char* username, char* password, char* email) {
//	SignUp signup;
//
//	memcpy(heapBuffer, username, sizeof(signup.username));
//	memcpy(heapBuffer + sizeof(signup.username), password, sizeof(signup.password));
//	memcpy(heapBuffer + sizeof(signup.username) + sizeof(signup.password), email, sizeof(signup.email));
//
//	/*memcpy(heapBuffer, &signup, sizeof(signup) - sizeof(signup.ImageStructArray));*/
//}

void CopyBufferToHeap(char* heapBuffer, char* srcBuffer, int size) {
	memcpy(heapBuffer, srcBuffer, size);
}

char* AllocateSignupPtr(int imageSize) {
	SignUp signup;

	char* BodyBuffer = new char[(sizeof(signup.username) + sizeof(signup.password) + sizeof(signup.email)) + imageSize];

	memset(BodyBuffer, 0, (sizeof(signup.username) + sizeof(signup.password) + sizeof(signup.email)) + imageSize);

	return BodyBuffer;
}


char* AllocateListingPtr(int imageSize) {
	Listing list;

	char* BodyBuffer = new char[(sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery) + sizeof(list.LookingFor)) + imageSize];

	memset(BodyBuffer, 0, (sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery) + sizeof(list.LookingFor)) + imageSize);

	return BodyBuffer;
}


Packet* CreatePacket() {
	Packet* Pkt = new Packet();

	Pkt->GetBody()->Data = nullptr;


	memset(Pkt->GetHead(), 0, sizeof(*(Pkt->GetHead())));
	memset(&(Pkt->GetBody()->User), 0, sizeof(Pkt->GetBody()->User));
	memset(&(Pkt->GetTail()->Checksum), 0, sizeof(Pkt->GetTail()->Checksum));


	return Pkt;
}

void DestroyPacket(Packet*& Pkt) {
	delete Pkt;

	Pkt = nullptr;
}


void FreeBuffer(char*& Buffer) {
	delete Buffer;

	Buffer = nullptr;
}


char* AllocateHeaderPtr() {
	Packet p;

	char* ptr = new char[sizeof(p.Head)];

	memset(ptr, 0, sizeof(p.Head));

	return ptr;
}


void Display(Packet* Pkt, std::ostream& os)
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


void DeserializeHeader(Packet* pkt, char**** TxBuffer) {
	memcpy(pkt->GetHead(), ***TxBuffer, sizeof(*(pkt->GetHead())));
}


void DeserializePostCountBuffer(Packet* pkt, char* src, int& numberOfPosts) {
	memcpy(pkt->GetHead(), src, sizeof(*(pkt->GetHead())));

	numberOfPosts = pkt->GetHead()->Length;
}


//void Deserialization(Packet* Pkt, char* src) {
//	memcpy(Pkt->GetHead(), src, sizeof(*(Pkt->GetHead())));
//
//	Pkt->GetBody()->Data = new char[Pkt->GetHead()->Length];
//
//	memcpy(&(Pkt->GetBody()->User), src + sizeof(*(Pkt->GetHead())), sizeof(Pkt->GetBody()->User));
//	memcpy(Pkt->GetBody()->Data, src + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User), Pkt->GetHead()->Length);
//
//	memcpy(Pkt->GetTail(), src + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User) + Pkt->GetHead()->Length, sizeof(*(Pkt->GetTail())));
//}

unsigned int DeserializeBufferToClientID(char** TxBuffer) {
	unsigned int clientID;

	memcpy(&clientID, *TxBuffer, sizeof(clientID));

	return clientID;
}


void DeserializationWithoutTail(Packet* Pkt, char* src) {
	memcpy(Pkt->GetHead(), src, sizeof(*(Pkt->GetHead())));

	Pkt->GetBody()->Data = new char[Pkt->GetHead()->Length];

	memcpy(&(Pkt->GetBody()->User), src + sizeof(*(Pkt->GetHead())), sizeof(Pkt->GetBody()->User));
	memcpy(Pkt->GetBody()->Data, src + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User), Pkt->GetHead()->Length);
}


unsigned int DeserializeClientID(char* src) {
	unsigned int ClientID;

	Packet p;

	memcpy(&ClientID, src + sizeof(*(p.GetHead())), sizeof(ClientID));

	return ClientID;
}


unsigned int DeserializeHeaderLengthMember(char* RxBuffer) {
	/*unsigned int headerLength = 0;*/

	Packet* p = CreatePacket();

	memcpy(p->GetHead(), RxBuffer, sizeof(*(p->GetHead())));

	/*int source_size = sizeof(p->GetHead()->Source);

	int dest_size = sizeof(p->GetHead()->Destination);

	int route_size = sizeof(p->GetHead()->Route);

	int auth_size = sizeof(p->GetHead()->Authorization);

	int size = sizeof(headerLength);

	memcpy(&headerLength, RxBuffer + sizeof(p->GetHead()->Source) + sizeof(p->GetHead()->Destination) + sizeof(p->GetHead()->Route) + sizeof(p->GetHead()->Authorization), sizeof(headerLength));*/

	return p->GetHead()->Length;
}


unsigned int DeserializeImageLength(char* RxBuffer, int offset) {
	unsigned int imageLength;

	memcpy(&imageLength, RxBuffer + offset, sizeof(imageLength));

	return imageLength;
}


void CopyImageFromRawBufferToByteArray(char* RxBuffer, char* imageArray, int imageSize) {
	Packet* p = CreatePacket();

	UserCredentials cred;

	memcpy(imageArray, RxBuffer + sizeof(*(p->GetHead())) + sizeof(p->GetBody()->User) + sizeof(cred.username) + sizeof(cred.password) + sizeof(cred.email), imageSize);

	delete[] p;
}


void SetHeader(Packet* Pkt, void* Head) {
	memcpy(Pkt->GetHead(), Head, sizeof(*(Pkt->GetHead())));
}


void SetBody(Packet* Pkt, unsigned int User, char* Data, int DataSize) {
	Pkt->GetBody()->User = User;

	if (DataSize > 0) {
		DataSize++;

		Pkt->GetBody()->Data = new char[DataSize];
		Pkt->GetBody()->Data[DataSize - 1] = '\0';


		memcpy(Pkt->GetBody()->Data, Data, DataSize);

		Pkt->GetHead()->Length = DataSize;
	}
}


unsigned int CalculateChecksum() {
	unsigned int checksum;

	memset(&(checksum), 0, sizeof(checksum));

	checksum = 0xFF00FF00;

	return checksum;
}


char* SerializeMyPostCountData(Packet* Pkt, int& totalSize) {
	if (Pkt->GetTxBuffer()) {
		delete[] Pkt->GetTxBuffer();
	}
	
	totalSize = (sizeof(*(Pkt->GetHead())) + (sizeof(Pkt->GetBody()->User)));

	Pkt->GetTxBuffer() = new char[totalSize];

	memcpy(Pkt->GetTxBuffer(), Pkt->GetHead(), sizeof(*(Pkt->GetHead())));

	memcpy(Pkt->GetTxBuffer() + sizeof(*(Pkt->GetHead())), &(Pkt->GetBody()->User), sizeof(Pkt->GetBody()->User));


	return Pkt->GetTxBuffer();
}


char* SerializeData(Packet* Pkt, int& TotalSize) {
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



char* SerializeHeaderOnlyPkt(Packet* Pkt, int& TotalSize) {
	if (Pkt->GetTxBuffer()) {
		delete[] Pkt->GetTxBuffer();
	}

	TotalSize = sizeof(*(Pkt->GetHead()));

	Pkt->GetTxBuffer() = new char[TotalSize];

	memset(Pkt->GetTxBuffer(), 0, TotalSize);


	memcpy(Pkt->GetTxBuffer(), Pkt->GetHead(), TotalSize);

	return Pkt->GetTxBuffer();
}
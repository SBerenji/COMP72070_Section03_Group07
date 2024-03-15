#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include "Packet.h"
using namespace std;


class Logging
{

public:
	void writeToFile(Packet p)
	{
		string fileName = "loggingFileClient.txt";
		ofstream fout;
		fout.open(fileName, std::ios_base::app);
		if (fout.is_open()) {
			fout << p.GetHead()->Source << "," << p.GetHead()->Destination << "," << p.GetHead()->Route << "," << p.GetHead()->Length << endl;
		}
		else
		{
			cout << "Error writting to file" << endl;
		}

		fout.close();
	}





};
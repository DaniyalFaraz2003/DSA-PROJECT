#pragma once
#include "DHT.h"
#ifndef SYSTEM_H
#define SYSTEM_H

#include <iomanip>
using namespace std;

class System
{
private:
	DHT sys;
public:
	void start() {
		system("cls");
		cout << setw(80) << right << setfill(' ') << "WELCOME TO THE DISTRIBUTED HASH TABLE SYSTEM!!!" << endl;
		int bits;
		cout << "Enter the number of bits for the identifier space: "; cin >> bits;
		while (!sys.setBitSize(bits)) {
			cout << "Re-enter the bit size: (1 >= b >= 160) : ";
			cin >> bits;
		}


	}

};


#endif // !SYSTEM_H
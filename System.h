#pragma once
#include "DHT.h"
#ifndef SYSTEM_H
#define SYSTEM_H

#include <iomanip>
#include <string>
#include <ctime>
#include <chrono>
#include <thread>
using namespace std;
//class representing the overall distributed hash table (DHT) system
class System
{
private:
	DHT sys;
	void menu() {  //Function to display the menu options
		cout << " 1. Show DHT" << endl;
		cout << " 2. Insert Machine" << endl;
		cout << " 3. Remove Machine" << endl;
		cout << " 4. Add File" << endl;
		cout << " 5. Remove File" << endl;
		cout << " 6. Search File" << endl;
		cout << " 7. Print Routing Table" << endl;
		cout << " 8. Print B-Tree" << endl;
		cout << " 9. Exit" << endl;
	}
public:

	void start() {  //Function to start and manage the DHT system
		system("cls");
		cout << setw(80) << right << setfill(' ') << "WELCOME TO THE DISTRIBUTED HASH TABLE SYSTEM!!!" << endl;
		int bits;
		cout << "Enter the number of bits for the identifier space: "; cin >> bits; cin.ignore();
		while (!sys.setBitSize(bits)) {   // Validate the entered bit size
			cout << "Re-enter the bit size, (1 >= b >= 160): ";
			cin >> bits;
			cin.ignore();
		}
		string machines; cout << "Enter the number of machines you want at start: "; getline(cin, machines);
		while (!(BIG_INT(machines).validate()) || (machines[0] == '-') || (BIG_INT(machines) > power(BIG_INT("2"), bits))) {  // Validate the entered number of machines
			if (!BIG_INT(machines).validate()) {
				cout << "Invalid number, enter again: ";
			}
			else {
				cout << "Machines not fit in this identifier space: ";
			}
			getline(cin, machines);
		}
		for (BIG_INT i = string("0"); i < BIG_INT(machines); i = i + BIG_INT("1")) {  // Insert machines into the DHT system
			sys.insertMachine();
		}
		int choice;
		do {  // Display the menu and perform operations
			string fileId, machineId;
			string id;
			string path;
			BIG_INT e;
			BIG_INT p;
			menu();
			cout << endl << "What operation do you want to perform: "; cin >> choice;
			cin.ignore();
			switch (choice)
			{
			case 1: // Show the current state of the DHT system
				sys.showMachines();
				break;
			case 2: // Insert a new machine into the DHT system
				sys.insertMachine();
				break;
			case 3:
				// Remove a machine based on user input
				cout << "Enter which id machine to remove: "; getline(cin, id);
				sys.removeMachine(BIG_INT(id));
				break;
			case 4:
				// Add a file to the DHT system
				cout << "Enter the complete path of the file: "; getline(cin, path);
				sys.addFile(path);
				break;
			case 5:
				// Remove a file based on user input
				cout << "Enter the id of the file to remove: "; getline(cin, fileId);
				sys.removeFile(BIG_INT(fileId));
				break;
			case 6:
				// Search for a file based on user input
				cout << "Enter the id of the file to search: "; getline(cin, fileId);
				cout << "Enter the id of the machine to originate the search from: "; getline(cin, machineId);
				e = fileId;
				p = machineId;
				sys.searchFile(e, p);
				break;
			case 7:  // Print the routing table of the DHT system
				sys.printRoutingTable();
				break;
			case 8:  // Print the B-tree of the DHT system
				sys.printBtree();
				break;
			case 9:  // Exit the DHT system
				break;
			default:
				cout << "Invalid choice entered" << endl;
				break;
			}
			cout << "Press Any Key To Continue...";
			getchar();
			system("cls");
		} while (choice != 9);
	}

};


#endif // !SYSTEM_H
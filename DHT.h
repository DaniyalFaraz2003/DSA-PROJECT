#pragma once
#include <iostream>
#include <string>
#include "CIrcularLL.h"
#include "Machine.h"
#include "BigInt.h"
#include "Utils.h"
using namespace std;


class DHT
{
	int identifierSpace;
public:CircularLinkedList<Machine> ring;
public:
	DHT() {
		// here we write temporary code to add some machines assuming identifier space of 5 bits;
		identifierSpace = 5;
		for (int i : {1, 4, 9, 11, 14, 18, 20, 21, 28}) {
			ring.insertSorted(Machine(BIG_INT(to_string(i))));
		}
		cout << ring << endl;
	}
	void makeRoutingTables() {
		CircleListNode<Machine>* start = ring.head;
		while (start->next != ring.head) 
		{
			// here we create the routing table of every machine
			for (int i = 0; i < identifierSpace; i++) {
				BIG_INT nextMachineId = (start->data.getId() + power(BIG_INT("2"), i)) % power(BIG_INT("2"), identifierSpace);
				CircleListNode<Machine>* probe = ring.head;
				while (probe->data.getId() < nextMachineId) {
					
					if (nextMachineId > probe->data.getId() && probe->next == ring.head) {
						probe = probe->next; break;
					}
					else {
						probe = probe->next;
					}
				}
				start->data.addRoutingTableEntry(&(probe->data));
			}
			start = start->next;
		}
		for (int i = 0; i < identifierSpace; i++) {
			BIG_INT nextMachineId = (start->data.getId() + power(BIG_INT("2"), i)) % power(BIG_INT("2"), identifierSpace);
			CircleListNode<Machine>* probe = start->next;
			while (probe->data.getId() <= nextMachineId) {

				if (nextMachineId > probe->data.getId() && probe->next == ring.head) {
					probe = probe->next; break;
				}
				else {
					probe = probe->next;
				}
			}
			start->data.addRoutingTableEntry(&(probe->data));
		}

	}
	void insertMachine();
	void removeMachine();
	void destroyRoutingTables();
	void searchFile();
	void addFile();
	void removeFile();

	// some util functions here

	string convert_to_dec(const string& hexNum, int bits) {

		

	}
};


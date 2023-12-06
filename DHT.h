#pragma once
#include <iostream>
#include <string>
#include "CIrcularLL.h"
#include "Machine.h"
#include "BigInt.h"
#include "Utils.h"
#include "sha1.hpp"
using namespace std;


class DHT
{
	int identifierSpace;
public:CircularLinkedList<Machine> ring;
public:
	DHT(int bitSize = 0) {
        this->identifierSpace = bitSize;
	}
   
	void makeRoutingTables() {
		CircleListNode<Machine>* start = ring.head;
        if (start->next == ring.head) {
            for (int i = 0; i < identifierSpace; i++) {
                start->data.addRoutingTableEntry(&(start->data));
            }
            return;
        }
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
                        if (nextMachineId >= probe->data.getId() && nextMachineId <= probe->next->data.getId()) {
                            probe = probe->next; break;
                        }
                        else {
						    probe = probe->next;
                        }
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
                    if (nextMachineId >= probe->data.getId() && nextMachineId <= probe->next->data.getId()) {
                        probe = probe->next; break;
                    }
                    else {
                        probe = probe->next;
                    }
                }
			}
			start->data.addRoutingTableEntry(&(probe->data));
		}

	}
    void insertMachine() { // this has to be changed for the future
        // changes to make are to make a folder for a new machine and divide the files in the machines
        destroyRoutingTables();
        string machineName;
        string hash;
        BIG_INT machineId;
        cout << "Enter the name of the machine: ";
        getline(cin, machineName);
        SHA1 digest; digest.update(machineName); hash = digest.final(); machineId = hashMod(hash, identifierSpace);
        while (ring.exists(Machine(machineId))) {
            cout << "Machine already exists enter name again: "; getline(cin, machineName);
            digest.update(machineName); hash = digest.final(); machineId = hashMod(hash, identifierSpace);
        }
        ring.insertSorted(Machine(machineId)); // here we update the Btrees too. and to make a folder in the system for this machine
        makeRoutingTables();
        
    }
    void removeMachine() { // implementation to be done.

    }

    void destroyRoutingTables() {
        if (ring.isEmpty()) return;
        CircleListNode<Machine>* curr = ring.head;
        while (curr->next != ring.head) {
            curr->data.destroyRoutingTable();
            curr = curr->next;
        }
        curr->data.destroyRoutingTable();
    }

	void searchFile();
	void addFile();
	void removeFile();

};


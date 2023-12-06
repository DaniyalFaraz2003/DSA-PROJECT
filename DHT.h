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
	void removeMachine();

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

	// some util functions here

    BIG_INT hashMod(string hash, int n)
    {
        string result = "";
        string binaries[16] = { "0000",
                               "0001",
                               "0010",
                               "0011",
                               "0100",
                               "0101",
                               "0110",
                               "0111",
                               "1000",
                               "1001",
                               "1010",
                               "1011",
                               "1100",
                               "1101",
                               "1110",
                               "1111" };
        for (int i = hash.length() - 1; i >= 0; i--)
        {
            if (result.length() >= n)
                break;
            if (hash[i] >= 'a' && hash[i] <= 'f')
            {
                switch (hash[i])
                {
                case 'a':
                    result = binaries[10] + result;
                    break;
                case 'b':
                    result = binaries[11] + result;
                    break;
                case 'c':
                    result = binaries[12] + result;
                    break;
                case 'd':
                    result = binaries[13] + result;
                    break;
                case 'e':
                    result = binaries[14] + result;
                    break;
                case 'f':
                    result = binaries[15] + result;
                    break;
                }
            }
            else
                result = binaries[hash[i] - '0'] + result;
        }
        if (n < result.length())
            result = result.substr(result.length() - n);
        BIG_INT decimal("0");
        BIG_INT power("1");
        // Iterate through the binary string from right to left
        for (int i = result.length() - 1; i >= 0; --i)
        {
            if (result[i] == '1')
            {
                decimal = decimal + power;
            }
            // Update the power for the next bit position
            power = power * BIG_INT("2");
        }
        return decimal;
    }
};


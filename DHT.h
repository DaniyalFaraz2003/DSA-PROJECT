#pragma once
#include <iostream>
#include <string>
#include <fstream>
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

        /*for (int i : {1, 2, 3, 5, 8, 11, 19, 20, 28}) {
            this->ring.insertSorted(Machine(BIG_INT(to_string(i))));
        }*/
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
    void showMachines() {
        std::string dotCode = "graph CircularLinkedList {\n";
        CircleListNode<Machine>* current = ring.head;

        // Specify the layout engine as "circo"
        dotCode += "  graph [layout=neato];\n";

        while (current->next != ring.head) {
            // Add a node for each element
            dotCode += "  " + current->data.getId().getBIG_INT() + " -- ";

            current = current->next;
        } 

        // Close the loop in DOT code
        dotCode += "  " + current->data.getId().getBIG_INT() + " -- " + ring.head->data.getId().getBIG_INT() + " [label=\"\", shape=circle];\n";
        dotCode += "}\n";


        ofstream dotFile("dht.dot");
        dotFile << dotCode;
        dotFile.close();
        string command = "neato -Tpng -Gsplines=true dht.dot -o dht.png";
        system(command.c_str());
        system("start dht.png");
        
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
        CircleListNode<Machine>* current = ring.head;
        while (!(current->data.getId() == machineId)) {
            current = current->next;
        }
        current->data.shiftFiles('i', current->next->data);
        
    }
    void removeMachine(BIG_INT id) { // implementation to be done.
        if (!ring.exists(Machine(id))) {
            cout << "Machine does not exist" << endl;
            return;
        }
        int i = 0;
        CircleListNode<Machine>* current = ring.head;
        while (!(current->data.getId() == id)) {
            current = current->next;
            i++;
        }
        if (current == ring.head) ring.pop();
        else {
            current->data.shiftFiles('d', current->next->data);
            ring.delete_from_index(i);
        }
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

    void searchFile() {

    }
	void addFile();
	void removeFile();

};


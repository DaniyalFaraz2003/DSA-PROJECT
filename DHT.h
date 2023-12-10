#pragma once

#ifndef DHT_H
#define DHT_H

#include "CIrcularLL.h"
#include "Machine.h"
#include "Utils.h"
#include "sha1.hpp"
using namespace std;


class DHT
{
	int identifierSpace;
    bool validateId(BIG_INT id) { // returns true if id is invalid
        if (id.getBIG_INT()[0] == '-') return true;
        return (ring.exists(Machine(id)) || !id.validate() || (id > (power(BIG_INT("2"), identifierSpace) - BIG_INT("1"))));
    }
public:CircularLinkedList<Machine> ring;
public:
	DHT(int bitSize = 0) {
        this->identifierSpace = bitSize;
        
        // for debugging purpose
        /*for (int i : {5, 9, 11}) {
            ring.insertSorted(Machine(BIG_INT(to_string(i))));
        }*/
	}
    bool setBitSize(int bitSize) {
        if (bitSize > 0 && bitSize < 161) {
            this->identifierSpace = bitSize;
            return true;
        }
        else return false;
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
        // changes to make are to make a folder for a new machine and divide machineId e the files in the machines
        if (BIG_INT(to_string(ring.getSize())) == power(BIG_INT("2"), identifierSpace)) {
            cout << "DHT is full no more machines can be added" << endl; return;
        }
        destroyRoutingTables(); 
        string machineName, hash; int treeDegree; char idChoice; BIG_INT machineId("0");
        cout << "Enter the name of the machine: "; getline(cin, machineName);
        cout << "You want to manually assign id to this machine? (y for yes): "; cin >> idChoice;
        cin.ignore();
        if (idChoice != 'y') {
            SHA1 digest; digest.update(machineName); hash = digest.final(); machineId = hashMod(hash, identifierSpace);
            while (ring.exists(Machine(machineId))) {
                cout << "Machine already exists enter name again: "; getline(cin, machineName);
                digest.update(machineName); hash = digest.final(); machineId = hashMod(hash, identifierSpace);
            }
        }
        else {
            cout << "Enter the id you want to assign to this machine: ";
            getline(cin, hash); BIG_INT id = BIG_INT(hash);
            while (validateId(id)) {
                if (ring.exists(Machine(id))) {
                    cout << "Machine already exists, enter again: ";
                }
                else {
                    cout << "Invalid Id, enter again: ";
                }
                getline(cin, hash);
                id = BIG_INT(hash);
            }
        }
        cout << "Enter the degree of the B-Tree you want for this machine: "; cin >> treeDegree;
        while (treeDegree < 3) {
            cout << "Invalid tree degree, enter again: "; cin >> treeDegree;
        }
        cin.ignore();
        ring.insertSorted(Machine(machineId, machineName, treeDegree)); // here we update the Btrees too. and to make a folder in the system for this machine
        makeRoutingTables();
        CircleListNode<Machine>* current = ring.head;
        while (!(current->data.getId() == machineId)) {
            current = current->next;
        }
        current->data.shiftFiles('i', current->next->data);
        
    }
    void removeMachine(BIG_INT id) { // implementation to be done.
        if (ring.isEmpty()) {
            cout << "No machines in the system" << endl; return;
        }
        if (!ring.exists(Machine(id))) {
            cout << "Machine does not exist" << endl;
            return;
        }
        if (ring.getSize() == 1) {
            ring.pop(); cout << "machine removed" << endl;
            return;
        }
        int i = 0;
        CircleListNode<Machine>* current = ring.head;
        while (!(current->data.getId() == id)) {
            current = current->next;
            i++;
        }
        current->data.shiftFiles('d', current->next->data);
        ring.delete_from_index(i);
        cout << "machine removed" << endl;
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

    Machine* routerSearch(BIG_INT& e, BIG_INT& p) {
        if (!e.validate() || e.getBIG_INT()[0] == '-' || e >= power(BIG_INT("2"), identifierSpace)) return nullptr;
        else if (!p.validate() || p.getBIG_INT()[0] == '-' || p >= power(BIG_INT("2"), identifierSpace)) return nullptr;
        if (ring.head == ring.head->next) {
            if (p == ring.head->data.getId()) { // this means the machine is there and is the only machine in the system
                cout << p;
                return &ring.head->data;
            }
            else {
                cout << "machine does not exist" << endl;
                return nullptr;
            }
        }
        // first step is to find the machine from which the request has to be generated.
        Machine* currentMachinePtr = &ring.head->data;
        while (!(p == currentMachinePtr->getId())) {
            if (p < currentMachinePtr->getId() && currentMachinePtr == &ring.head->data) {
                break;
            }
            else if (p > currentMachinePtr->getId() && p <= currentMachinePtr->getRoutingTable().front()->getId()) {
                currentMachinePtr = currentMachinePtr->getRoutingTable().front();
                break;
            }
            else if (p > currentMachinePtr->getId() && currentMachinePtr->getRoutingTable().front()->getId() < currentMachinePtr->getId()) { // case where next is head
                currentMachinePtr = currentMachinePtr->getRoutingTable().front();
                break;
            }
            else if (p < currentMachinePtr->getId() && currentMachinePtr->getRoutingTable().front()->getId() < currentMachinePtr->getId()) { // case where next is head
                currentMachinePtr = currentMachinePtr->getRoutingTable().front();
                break;
            }
            else {
                for (DoublyLinkedList<Machine*>::Iterator it = currentMachinePtr->getRoutingTable().begin(); it != currentMachinePtr->getRoutingTable().end(); ++it) {
                    if (it + 1 == nullptr) {
                        if (currentMachinePtr->getId() == (*it)->getId())
                            currentMachinePtr = currentMachinePtr->getRoutingTable().front();
                        else
                            currentMachinePtr = *it;
                        break;
                    }
                    else {
                        if (p > (*it)->getId() && (*it)->getId() > (it + 1)->data->getId()) { // the case the where the next machine is the head
                            currentMachinePtr = *it;
                            break;
                        }
                        if (p > (*it)->getId() && p <= (it + 1)->data->getId()) {
                            currentMachinePtr = *it;
                            break;
                        }
                    }
                }
            }
        }
        if (!(currentMachinePtr->getId() == p)) {
            cout << "Machine does not exist" << endl; return nullptr;
        }
        // now we have the pointer to the machine from which the request has to be generated.
        p = currentMachinePtr->getId();
        while (true) { // search the ring dht using the machine's routing tables
            //if (!(e == p) && (e < p) && (e < currentMachinePtr->getRoutingTable().front()->getId())) { // this means we have found the machine
            //    break;
            //}
            if (e <= p && currentMachinePtr == &ring.head->data) {
                break;
            }
            if (e == p) { // currentMachinePtr contains the file
                break;
            }
            else if (e > p && e <= currentMachinePtr->getRoutingTable().front()->getId()) { // the next machine contains the file
                currentMachinePtr = currentMachinePtr->getRoutingTable().front();
                break;
            }
            else if ((p > currentMachinePtr->getRoutingTable().front()->getId()) && (e > p || e <= currentMachinePtr->getRoutingTable().front()->getId())) { // the case where the next machine is the head
                // here we consider two cases. one in which e is greater than current. and another where e is less than or equal to the head
                currentMachinePtr = currentMachinePtr->getRoutingTable().front();
                break;
            }
            else {
                for (DoublyLinkedList<Machine*>::Iterator it = currentMachinePtr->getRoutingTable().begin(); it != currentMachinePtr->getRoutingTable().end(); ++it) {
                    if (it + 1 == nullptr) { // not found in the routing table. we point to the last entry in the routing table
                        currentMachinePtr = *it;
                        break;
                    }
                    else {
                        if (e > (*it)->getId() && (*it)->getId() > (it + 1)->data->getId()) { // the case the where the next machine is the head
                            currentMachinePtr = *it;
                            break;
                        }
                        if (e > (*it)->getId() && e <= (it + 1)->data->getId()) { // file found in this range
                            currentMachinePtr = *it;
                            break;
                        }
                    }
                }
            }
            p = currentMachinePtr->getId();
        }
        // till here we have reached the machine where the actual file is stored
        return currentMachinePtr;
    }

    void searchFile(BIG_INT& e, BIG_INT& p) { // here we implement the search algorithm using the routing tables.
        Machine* machine = routerSearch(e, p);
        if (machine == nullptr) {
            cout << "no machine with id: " << p.getBIG_INT() << endl;
            return;
        }
        Pair<BTreeNode<KeyValuePair<BIG_INT, LinkedList<string>>>*, int, int> searchResult = machine->searchFile(e);
        if (searchResult.first == nullptr) {
            cout << "No such file in the system" << endl;
        }
        else {
            int count = 0;
            cout << "The files with id " << e.getBIG_INT() << " are: " << endl;
            for (LinkedList<string>::Iterator it = searchResult.first->keys[searchResult.second].value.begin(); it != searchResult.first->keys[searchResult.second].value.end(); ++it) {
                cout << ++count << ". " << *it << endl;
            }
        }
    }
    void addFile(const string& path) {
        ifstream file(path);
        if (file.is_open()) {
            BIG_INT p = ring.head->data.getId();
            string hash = SHA1::from_file(path);
            BIG_INT e = hashMod(hash, identifierSpace);
            cout << "File assigned the id of: " << e.getBIG_INT() << endl;
            Machine* machine = routerSearch(e, p);
            machine->addFile(e, ext(path));
            cout << "file inserted in system with id: " << e.getBIG_INT() << endl;
            file.close();
        }
        else {
            cout << "no file exists with this path" << endl;
        }
        
    }
    void printRoutingTable() {
        string id; cout << "Enter the id of the machine you want to print the routing table of: "; getline(cin, id);
        BIG_INT machineId(id);
        while (validateId(machineId)) {
            if (ring.exists(Machine(id))) {
                break;
            }
            else {
                cout << "Invalid Id, enter again: ";
            }
            getline(cin, id);
            machineId = BIG_INT(id);
        }
        BIG_INT startId = ring.head->data.getId();
        Machine* machine = routerSearch(machineId, machineId);
        if (machine) {
            machine->printRoutingTable();
        }
    }
    void printBtree() {
        string id; cout << "Enter the id of the machine you want to print the B-Tree of: "; getline(cin, id);
        BIG_INT machineId(id);
        while (validateId(machineId)) {
            if (ring.exists(Machine(id))) {
                break;
            }
            else {
                cout << "Invalid Id, enter again: ";
            }
            getline(cin, id);
            machineId = BIG_INT(id);
        }
        BIG_INT startId = ring.head->data.getId();
        Machine* machine = routerSearch(machineId, machineId);
        if (machine) {
            machine->printBtree();
        }
    }
    void removeFile(BIG_INT e) {
        BIG_INT p = ring.head->data.getId();
        Machine* machine = routerSearch(e, p);
        machine->removeFile(e);
        cout << "file removed" << endl;
    }
};


#endif // !DHT_H

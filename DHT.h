#pragma once

#ifndef DHT_H
#define DHT_H

#include <ctime>
#include <chrono>
#include <thread>
#include "CIrcularLL.h"
#include "Machine.h"
#include "Utils.h"
#include "sha1.hpp"
using namespace std;

// the whole dht class which fulfills all the functionality of a dht including:
/*
1. adding machine
2. removing machine
3. adding file
4. removing file
5. searching file
*/

class DHT
{
    FileHandling handle;
    int identifierSpace;
    bool validateId(BIG_INT id) { // returns true if id is invalid
        if (id.getBIG_INT()[0] == '-') return true;
        return (ring.exists(Machine(id)) || !id.validate() || (id > (power(BIG_INT("2"), identifierSpace) - BIG_INT("1"))));
    }
public:CircularLinkedList<Machine> ring;
public:
    DHT(int bitSize = 0) {
        this->identifierSpace = bitSize;
        handle.createFolder("D:\\storage");
        handle.createFolder("D:\\storage\\DHT");
        // for debugging purpose
        /*for (int i : {8, 10, 25}) {
            ring.insertSorted(Machine(BIG_INT(to_string(i)), to_string(i)));
        }*/
    }
    //sets the bit size of the identifier space
    bool setBitSize(int bitSize) {
        if (bitSize > 0 && bitSize < 161) {        //ensures the bit size is within a valid range
            this->identifierSpace = bitSize;
            return true;
        }
        else return false;
    }   //creates routing tables for all machines in the DHT
    void makeRoutingTables() {
        if (ring.isEmpty()) return;  //if the DHT is empty, routing table is not updated
        //updation of routing tables	
        cout << "Updating Routing Tables"; this_thread::sleep_for(chrono::seconds(1)); cout << "."; this_thread::sleep_for(chrono::seconds(1));
        cout << "."; this_thread::sleep_for(chrono::seconds(1)); cout << "." << endl;
        CircleListNode<Machine>* start = ring.head;
        //if there is only one machine, then it should have its routing table entries pointing to itself
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
                //we calculate the ID of the next machine in the routing table
                BIG_INT nextMachineId = (start->data.getId() + power(BIG_INT("2"), i)) % power(BIG_INT("2"), identifierSpace);
                CircleListNode<Machine>* probe = ring.head;
                while (probe->data.getId() < nextMachineId) {
                    //case in which the next machine is the last in DHT
                    if (nextMachineId > probe->data.getId() && probe->next == ring.head) {
                        probe = probe->next; break;
                    }
                    else {
                        //case in which the next machine is found within the DHT
                        if (nextMachineId >= probe->data.getId() && nextMachineId <= probe->next->data.getId()) {
                            probe = probe->next; break;
                        }
                        else {
                            probe = probe->next;
                        }
                    }
                }
                //add the machine in the current machine's routing table
                start->data.addRoutingTableEntry(&(probe->data));
            }
            start = start->next;  //moving on to the next machine
        }
        //handling routing table entries for the last machine
        for (int i = 0; i < identifierSpace; i++) {
            //calculating the ID of next machine
            BIG_INT nextMachineId = (start->data.getId() + power(BIG_INT("2"), i)) % power(BIG_INT("2"), identifierSpace);
            //finding the next machine
            CircleListNode<Machine>* probe = start->next;
            while (probe->data.getId() <= nextMachineId) {
                //case in which the next machine is last
                if (nextMachineId > probe->data.getId() && probe->next == ring.head) {
                    probe = probe->next; break;
                }
                else {
                    //case in which the next machine is found within the DHT
                    if (nextMachineId >= probe->data.getId() && nextMachineId <= probe->next->data.getId()) {
                        probe = probe->next; break;
                    }
                    else {
                        probe = probe->next;
                    }
                }
            }
            //adding the machine found to the last machine's routing table
            start->data.addRoutingTableEntry(&(probe->data));
        }
        cout << "Routing Tables Updated Successfully" << endl;
    }
    void showMachines() {
        //checks if the DHT is empty
        if (ring.isEmpty()) {
            cout << "DHT Is Empty" << endl;
            return;
        }
        //initializing the dot code string for visualization
        std::string dotCode = "graph CircularLinkedList {\n";
        CircleListNode<Machine>* current = ring.head;

        // Specify the layout engine as "circo"
        dotCode += "  graph [layout=neato];\n";

        while (current->next != ring.head) {
            // Add a node for each element in the dot code
            dotCode += "  " + current->data.getId().getBIG_INT() + " -- ";

            current = current->next;
        }

        // Close the loop in DOT code
        dotCode += "  " + current->data.getId().getBIG_INT() + " -- " + ring.head->data.getId().getBIG_INT() + " [label=\"\", shape=circle];\n";
        dotCode += "}\n";

        //write the dot code to a file named "dht.dot"
        ofstream dotFile("dht.dot");
        dotFile << dotCode;
        dotFile.close();
        //generates a png image from the dot file 
        string command = "neato -Tpng -Gsplines=true dht.dot -o dht.png";
        system(command.c_str());
        system("start dht.png");

    }

    void insertMachine() {
        //checks if the DHT is full
        if (BIG_INT(to_string(ring.getSize())) == power(BIG_INT("2"), identifierSpace)) {
            cout << "DHT is full no more machines can be added" << endl; return;
        }
        destroyRoutingTables();
        string machineName, hash; int treeDegree; char idChoice; BIG_INT machineId("0");
        cout << "Enter the name of the machine: "; getline(cin, machineName);
        cout << "You want to manually assign id to this machine? (y for yes): "; cin >> idChoice;
        cin.ignore();
        if (idChoice != 'y') {
            //automatically generates an ID based on the machine name using SHA1 hashing
            SHA1 digest; digest.update(machineName); hash = digest.final(); machineId = hashMod(hash, identifierSpace);
            //check if a machine with the generated ID already exists
            while (ring.exists(Machine(machineId))) {
                cout << "Machine already exists enter name again: "; getline(cin, machineName);
                digest.update(machineName); hash = digest.final(); machineId = hashMod(hash, identifierSpace);
            }
        }
        else {
            //manually assign an ID entered by the user
            cout << "Enter the id you want to assign to this machine: ";
            getline(cin, hash); machineId = BIG_INT(hash);
            //validating the machine ID
            while (validateId(machineId)) {
                if (ring.exists(Machine(machineId))) {
                    cout << "Machine already exists, enter again: ";
                }
                else {
                    cout << "Invalid Id, enter again: ";
                }
                getline(cin, hash);
                machineId = BIG_INT(hash);
            }

        }
        cout << "Enter the degree of the B-Tree you want for this machine: "; cin >> treeDegree;
        while (treeDegree < 3) {  //ensures the degree of BTree is valid
            cout << "Invalid tree degree, enter again: "; cin >> treeDegree;
        }
        cin.ignore();
        //insert the new machine into the DHT's circular linked list and update routing tables
        ring.insertSorted(Machine(machineId, machineName, treeDegree)); // here we update the Btrees too. and to make a folder in the system for this machine
        makeRoutingTables();
        CircleListNode<Machine>* current = ring.head;
        while (!(current->data.getId() == machineId)) {
            current = current->next;
        }
        // we create the directory for this machine and then shift the files
        handle.createFolder("D:\\storage\\DHT\\" + machineName + "_" + machineId.getBIG_INT());
        current->data.shiftFiles('i', current->next->data);
        cout << "Machine Inserted With Name: " << machineName << endl; cout << "Id Assigned To This Machine: " << machineId.getBIG_INT() << endl;
    }
    void removeMachine(BIG_INT id) {
        //checks if the DHT is empty
        if (ring.isEmpty()) {
            cout << "No machines in the system" << endl; return;
        }
        //checks if the machine with the given ID exists in the DHT
        if (!ring.exists(Machine(id))) {
            cout << "Machine does not exist" << endl;
            return;
        } //if there is only one machine in the DHT, we remove it 
        if (ring.getSize() == 1) {
            ring.pop(); cout << "machine removed" << endl;
            return;
        }
        destroyRoutingTables();
        int i = 0;
        CircleListNode<Machine>* current = ring.head;
        while (!(current->data.getId() == id)) {   //locating the machine with the specified ID 
            current = current->next;
            i++;
        }
        current->data.shiftFiles('d', current->next->data);  //shift files to the next machine and update BTrees too
        handle.removeFolder("D:\\storage\\DHT\\" + current->data.getName());  //removing the directory associated with the machine
        ring.delete_from_index(i);
        makeRoutingTables();

        cout << "machine removed" << endl;
    }
    //function to destroy routing tables of machines in DHT
    void destroyRoutingTables() {
        if (ring.isEmpty()) return;
        CircleListNode<Machine>* curr = ring.head;
        //traverse the circular linked list and destroy routing tables of each machine
        while (curr->next != ring.head) {
            curr->data.destroyRoutingTable();
            curr = curr->next;
        }
        curr->data.destroyRoutingTable();  //destroying the routing table of the last machine
    }

    //function to search for a machine in the DHT based on file ID 'e' and machine ID 'p'
    Machine* routerSearch(BIG_INT& e, BIG_INT& p) {
        if (ring.isEmpty()) {
            cout << "DHT Is Empty" << endl;
            return nullptr;
        }
        //validation of the input IDs
        if (!e.validate() || e.getBIG_INT()[0] == '-' || e >= power(BIG_INT("2"), identifierSpace)) return nullptr;
        else if (!p.validate() || p.getBIG_INT()[0] == '-' || p >= power(BIG_INT("2"), identifierSpace)) return nullptr;
        if (ring.head == ring.head->next) {
            if (p == ring.head->data.getId()) { // this means the machine is there and is the only machine in the system, check if it matches with the specifies ID
                return &ring.head->data;
            }
            else {
                cout << "Machine Does Not Exist" << endl;
                return nullptr;
            }
        }
        //displaying a message indicating the start of the search
        cout << "Finding The Origin Machine"; this_thread::sleep_for(chrono::seconds(1)); cout << "."; this_thread::sleep_for(chrono::seconds(1)); cout << ".";
        this_thread::sleep_for(chrono::seconds(1)); cout << "." << endl;
        Machine* currentMachinePtr = &ring.head->data;
        cout << "Control Passed To The Head Machine: " << currentMachinePtr->getName() << endl;
        //traverse the DHT to find the origin machine
        while (!(p == currentMachinePtr->getId())) {
            this_thread::sleep_for(chrono::seconds(1));
            //check if the starting ID is less than the current machine's ID and the current machine is the head
            if (p < currentMachinePtr->getId() && currentMachinePtr == &ring.head->data) {
                break;
            }  //check if the starting ID is greater than the current machine's ID and less than or equal to the next machine's ID
            else if (p > currentMachinePtr->getId() && p <= currentMachinePtr->getRoutingTable().front()->getId()) {
                cout << "Control Passed From [" << currentMachinePtr->getName() << "] To [";
                currentMachinePtr = currentMachinePtr->getRoutingTable().front();
                cout << currentMachinePtr->getName() << "]" << endl;
                break;
            } //check if the starting ID is greater than the current machine's ID, and the next machine is the head
            else if (p > currentMachinePtr->getId() && currentMachinePtr->getRoutingTable().front()->getId() < currentMachinePtr->getId()) {
                cout << "Control Passed From [" << currentMachinePtr->getName() << "] To [";
                currentMachinePtr = currentMachinePtr->getRoutingTable().front();
                cout << currentMachinePtr->getName() << "]" << endl;
                break;
            } //check if the starting ID is less than the current machine's ID, and the next machine is the head
            else if (p < currentMachinePtr->getId() && currentMachinePtr->getRoutingTable().front()->getId() < currentMachinePtr->getId()) { // case where next is head
                cout << "Control Passed From [" << currentMachinePtr->getName() << "] To [";
                currentMachinePtr = currentMachinePtr->getRoutingTable().front();
                cout << currentMachinePtr->getName() << "]" << endl;
                break;
            }
            else { //traversing the routing table of the current machine
                cout << "Traversing The Routing Table Of Machine: [" << currentMachinePtr->getName() << "]" << endl;
                for (DoublyLinkedList<Machine*>::Iterator it = currentMachinePtr->getRoutingTable().begin(); it != currentMachinePtr->getRoutingTable().end(); ++it) {
                    if (it + 1 == nullptr) {
                        if (currentMachinePtr->getId() == (*it)->getId()) {
                            cout << "Control Passed From [" << currentMachinePtr->getName() << "] To [";
                            currentMachinePtr = currentMachinePtr->getRoutingTable().front();
                            cout << currentMachinePtr->getName() << "]" << endl;
                        }
                        else {
                            cout << "Control Passed From [" << currentMachinePtr->getName() << "] To [";
                            currentMachinePtr = *it;
                            cout << currentMachinePtr->getName() << "]" << endl;
                        }
                        break;
                    }
                    else {
                        if (p > (*it)->getId() && (*it)->getId() > (it + 1)->data->getId()) { // the case the where the next machine is the head
                            cout << "Control Passed From [" << currentMachinePtr->getName() << "] To [";
                            currentMachinePtr = *it;
                            cout << currentMachinePtr->getName() << "]" << endl;
                            break;
                        }
                        if (p > (*it)->getId() && p <= (it + 1)->data->getId()) {
                            cout << "Control Passed From [" << currentMachinePtr->getName() << "] To [";
                            currentMachinePtr = *it;
                            cout << currentMachinePtr->getName() << "]" << endl;
                            break;
                        }
                    }
                }
            }
        }
        if (!(currentMachinePtr->getId() == p)) {
            cout << "Machine Does Not Exist" << endl; return nullptr;   //check if the origin machine has been found
        }
        cout << "Origin Machine Found: [" << currentMachinePtr->getName() << "]" << endl;
        cout << "Starting Search For The File With Id: " << e << " ."; this_thread::sleep_for(chrono::seconds(1));
        cout << "."; this_thread::sleep_for(chrono::seconds(1)); cout << "." << endl;
        // now we have the pointer to the machine from which the request has to be generated.
        p = currentMachinePtr->getId();
        while (true) { // search the ring dht using the machine's routing tables
            this_thread::sleep_for(chrono::seconds(1));
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
                cout << "Control Passed From [" << currentMachinePtr->getName() << "] To [";
                currentMachinePtr = currentMachinePtr->getRoutingTable().front();
                cout << currentMachinePtr->getName() << "]" << endl;
                break;
            }
            else if ((p > currentMachinePtr->getRoutingTable().front()->getId()) && (e > p || e <= currentMachinePtr->getRoutingTable().front()->getId())) { // the case where the next machine is the head
                // here we consider two cases. one in which e is greater than current. and another where e is less than or equal to the head
                cout << "Control Passed From [" << currentMachinePtr->getName() << "] To [";
                currentMachinePtr = currentMachinePtr->getRoutingTable().front();
                cout << currentMachinePtr->getName() << "]" << endl;
                break;
            }
            else {
                cout << "Traversing The Routing Table Of Machine: [" << currentMachinePtr->getName() << "]" << endl;
                for (DoublyLinkedList<Machine*>::Iterator it = currentMachinePtr->getRoutingTable().begin(); it != currentMachinePtr->getRoutingTable().end(); ++it) {
                    if (it + 1 == nullptr) { // not found in the routing table. we point to the last entry in the routing table
                        cout << "Control Passed From [" << currentMachinePtr->getName() << "] To [";
                        currentMachinePtr = *it;
                        cout << currentMachinePtr->getName() << "]" << endl;
                        break;
                    }
                    else {
                        if (e > (*it)->getId() && (*it)->getId() > (it + 1)->data->getId()) { // the case the where the next machine is the head
                            cout << "Control Passed From [" << currentMachinePtr->getName() << "] To [";
                            currentMachinePtr = *it;
                            cout << currentMachinePtr->getName() << "]" << endl;
                            break;
                        }
                        if (e > (*it)->getId() && e <= (it + 1)->data->getId()) { // file found in this range
                            cout << "Control Passed From [" << currentMachinePtr->getName() << "] To [";
                            currentMachinePtr = *it;
                            cout << currentMachinePtr->getName() << "]" << endl;
                            break;
                        }
                    }
                }
            }
            p = currentMachinePtr->getId();
        }
        cout << "Machine Found Which Has This File: [" << currentMachinePtr->getName() << "]" << endl;
        // till here we have reached the machine where the actual file is stored
        return currentMachinePtr;
    }

    void searchFile(BIG_INT& e, BIG_INT& p) { // here we implement the search algorithm using the routing tables.
        Machine* machine = routerSearch(e, p);
        //check if the machine is found
        if (machine == nullptr) {
            cout << "No Machine With Id: " << p.getBIG_INT() << endl;
            return;
        }
        this_thread::sleep_for(chrono::seconds(1));
        cout << "Going In The BTree Of Machine [" << machine->getName() << "_" << machine->getId() << "] .";
        this_thread::sleep_for(chrono::seconds(1)); cout << "."; this_thread::sleep_for(chrono::seconds(1)); cout << "." << endl;
        //search for the file with ID 'e' in the B-tree of the found machine
        Pair<BTreeNode<KeyValuePair<BIG_INT, LinkedList<string>>>*, int, int> searchResult = machine->searchFile(e);
        if (searchResult.first == nullptr) {
            cout << "No Such File In The System" << endl;
        }
        else {
            int count = 0;
            cout << "The Files With Id " << e.getBIG_INT() << " are: " << endl;
            //display the list of files with the specified ID
            for (LinkedList<string>::Iterator it = searchResult.first->keys[searchResult.second].value.begin(); it != searchResult.first->keys[searchResult.second].value.end(); ++it) {
                cout << ++count << ". " << *it << endl;
            }
        }
    }
    void addFile(const string& path) {     //here we add a file in the system
        if (ring.isEmpty()) {      //check if the DHT is empty
            cout << "DHT Is Empty" << endl; return;
        }
        cout << "Starting To Add File [" << path << "] To The System.";
        this_thread::sleep_for(chrono::seconds(1)); cout << "."; this_thread::sleep_for(chrono::seconds(1)); cout << '.' << endl;
        ifstream file(path);
        if (file.is_open()) {  //check if the file is open
            BIG_INT p = ring.head->data.getId(); //set the starting machine ID to the ID of the head machine in the DHT
            //calculate the file ID based on the SHA-1 hash of the file content
            string hash = SHA1::from_file(path);
            BIG_INT e = hashMod(hash, identifierSpace);
            cout << "File Assigned The Id Of: " << e.getBIG_INT() << endl;
            Machine* machine = routerSearch(e, p);  //use routerSearch to find the machine where the file should be inserted
            if (machine) {
                machine->addFile(e, ext(path), path);   //add the file to the found machine's B-tree
                cout << "File Inserted In the System With Id: " << e.getBIG_INT() << endl;
            }
            else {
                cout << "File Not Inserted" << endl;
            }
            file.close();
        }
        else {
            cout << "No File Exists With This Path" << endl;
        }

    }
    void printRoutingTable() {    //function to print the routing table
        if (ring.isEmpty()) {
            cout << "DHT Is Empty" << endl; return;
        }
        string id; cout << "Enter The Id Of The Machine You Want To Print The Routing Table Of: "; getline(cin, id);  //Get the ID of the machine from the user
        BIG_INT machineId(id);
        while (validateId(machineId)) {       // Validate the entered ID
            if (ring.exists(Machine(id))) {
                break;
            }
            else {
                cout << "Invalid Id, Enter Again: ";
            }
            getline(cin, id);
            machineId = BIG_INT(id);
        }
        BIG_INT startId = ring.head->data.getId();
        Machine* machine = routerSearch(machineId, machineId);  //Use routerSearch to find the machine with the specified ID
        if (machine) {
            machine->printRoutingTable();
            cout << endl;
        }
    }
    void printBtree() {   // Function to print the B-tree of a specified machine
        if (ring.isEmpty()) {
            cout << "DHT Is Empty" << endl; return;
        }
        string id; cout << "Enter The Id Of The Machine You Want To Print The B-Tree Of: "; getline(cin, id);
        BIG_INT machineId(id);
        while (validateId(machineId)) {
            if (ring.exists(Machine(id))) {
                break;
            }
            else {
                cout << "Invalid Id, Enter Again: ";
            }
            getline(cin, id);
            machineId = BIG_INT(id);
        }
        BIG_INT startId = ring.head->data.getId();
        Machine* machine = routerSearch(machineId, machineId);  // Use routerSearch to find the machine with the specified ID
        if (machine) {
            machine->printBtree();
        }
    }
    void removeFile(BIG_INT e) {   //Function to remove a file with a specified ID from the DHT
        if (ring.isEmpty()) {
            cout << "DHT Is Empty" << endl; return;
        }
        cout << "Starting To Remove The File With Id: " << e; this_thread::sleep_for(chrono::seconds(1)); cout << ".";
        this_thread::sleep_for(chrono::seconds(1)); cout << "."; this_thread::sleep_for(chrono::seconds(1)); cout << "." << endl;
        BIG_INT p = ring.head->data.getId(); //Set the starting machine ID to the ID of the head machine in the DHT
        Machine* machine = routerSearch(e, p); //Use routerSearch to find the machine where the file with ID 'e' is located
        machine->removeFile(e);
    }
    ~DHT() {  //destructor in which the directories are removed too
        this->ring.makenull();
        handle.removeFolder("D:\\storage\\DHT");
        handle.removeFolder("D:\\storage");
    }
};


#endif // !DHT_H
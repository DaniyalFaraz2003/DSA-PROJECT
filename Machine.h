#pragma once
#ifndef MACHINE_H
#define MACHINE_H


#include <iostream>
#include "BTree.h"
#include "BigInt.h"
using namespace std;

template <class T, class U>
struct KeyValuePair {
	T key;
	U value;
	bool operator<(const KeyValuePair& other) {
		return this->key < other.key;
	}
};

class Machine
{
	BTree<KeyValuePair<BIG_INT, LinkedList<string>>> indexTree;
	string name;
	BIG_INT id;
	DoublyLinkedList<Machine*> router;
public:
	Machine(BIG_INT id) { 
		this->id = id;
	}
	BIG_INT getId() const {
		return this->id;
	}
	bool operator<(const Machine& other) {
		return this->getId() < other.getId();
	}
	bool operator>(const Machine& other) {
		return this->getId() > other.getId();
	}
	bool operator==(const Machine& other) {
		return this->getId() == other.getId();
	}
	bool operator<=(const Machine& other) {
		return *this < other && *this == other;
	}
	bool operator>=(const Machine& other) {
		return *this > other && *this == other;
	}
	friend ostream& operator<<(ostream& out, Machine& m) {
		out << m.getId();
		return out;
	}


	void addRoutingTableEntry(Machine* val) {
		router.push(val);
	}

	void destroyRoutingTable() {
		router.makenull();
	}

	DoublyLinkedList<Machine*>& getRoutingTable() {
		return router;
	}

	void printRoutingTable() {
		for (DoublyLinkedList<Machine*>::Iterator it = router.begin(); it != router.end(); ++it) {
			if (it + 1 != nullptr) {
				cout << "[" << (*it)->getId() << "]<->";
			}
			else {
				cout << "[" << (*it)->getId() << "]";
			}
		}
	}
};


#endif // !MACHINE_H
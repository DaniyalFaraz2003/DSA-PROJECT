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
	bool operator>(const KeyValuePair& other) {
		return this->key > other.key;
	}
	bool operator==(const KeyValuePair& other) {
		return this->key == other.key;
	}
	bool operator<=(const KeyValuePair& other) {
		return this->key <= other.key;
	}
	bool operator>=(const KeyValuePair& other) {
		return this->key >= other.key;
	}
};

class Machine
{
	BIG_INT id;
	string name;
	BTree<KeyValuePair<BIG_INT, LinkedList<string>>> indexTree;
	DoublyLinkedList<Machine*> router;
public:
	
	Machine(BIG_INT id, string name = "", int treeDegree = 3) {
		this->id = id;
		this->name = name;
		this->indexTree.setDegree(treeDegree); // to be taken by the user
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

	BTreeNode<KeyValuePair<BIG_INT, LinkedList<string>>>* getBtreeHead() {
		return indexTree.getRoot();
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

	void addFile(BIG_INT id, const string& filename) {
		KeyValuePair<BIG_INT, LinkedList<string>> data; data.key = id; 
		string path = name + "/" + id.getBIG_INT() + "/";
		Pair<BTreeNode<KeyValuePair<BIG_INT, LinkedList<string>>>*, int, int> res = indexTree.search(data);
		if (res.first == nullptr) { // if this id is not already present then just insert the node
			path += filename + "_file_" + to_string(data.value.getSize());
			data.value.push(path);
			indexTree.insert(data);
		}
		else { // collision. here we chain the situation.
			res.first->keys[res.second].value.push(path + filename + "_file_" + to_string(res.first->keys[res.second].value.getSize()));
			cout << res.first->keys[res.second].value;
		}
		// and add the actual file to the path too.
	}

	void removeFile(BIG_INT id, string filename) {
		
	}

	void searchFile() {

	}
};



#endif // !MACHINE_H
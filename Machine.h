#pragma once
#ifndef MACHINE_H
#define MACHINE_H

#include <fstream>
#include "BTree.h"
#include "BigInt.h"
#include "DoublyLL.h"
#include "SinglyLL.h"
#include "Filehandling.h"
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
	FileHandling handle;
	BIG_INT id;
	string name;
	BTree<KeyValuePair<BIG_INT, LinkedList<string>>> indexTree;
	DoublyLinkedList<Machine*> router;

	// some private functions
	string getExtension(const string filename) {
		string res; int i;
		for (i = filename.length() - 1; i >= 0; i--) {
			if (filename[i] == '.') break;
		}
		for (i; i < filename.length(); i++) {
			res += filename[i];
		}
		return res;
	}


	void visualizeTree(const string& dotCode)
	{
		ofstream dotFile("btree.dot");
		dotFile << dotCode;
		dotFile.close();
		string command = "dot -Tpng btree.dot -o btree.png";
		system(command.c_str());
		system("start btree.png");
	}

	string generateDotCode(BTreeNode<KeyValuePair<BIG_INT, LinkedList<string>>>* btreeRoot)
	{
		string dotCode = "digraph BTree {\n";
		dotCode += "\tnode [shape=record, height=.1];\n\n";

		Queue<BTreeNode<KeyValuePair<BIG_INT, LinkedList<string>>>*> levelOrderQueue;
		Queue<int> idQ;
		levelOrderQueue.enqueue(btreeRoot);
		int count = 0;
		idQ.enqueue(count);
		while (!levelOrderQueue.isEmpty())
		{
			BTreeNode<KeyValuePair<BIG_INT, LinkedList<string>>>* current = levelOrderQueue.head();
			levelOrderQueue.dequeue();
			int j = idQ.dequeue();

			dotCode += "\tnode" + to_string(j) + " [label=\"";
			for (int i = 0; i < current->keys.getSize(); i++)
			{
				dotCode += "|" + current->keys[i].key.getBIG_INT();
			}
			dotCode += "|\"];\n";

			for (size_t i = 0; i < current->children.getSize(); ++i)
			{
				count++;
				dotCode += "\tnode" + to_string(j) + " -> node" + to_string(count) + ";\n";
				if (current->children[i]->p == current)
				{
					dotCode += "\tnode" + to_string(count) + " -> node" + to_string(j) + ";\n";
				}
				idQ.enqueue(count);
				levelOrderQueue.enqueue(current->children[i]);
			}
		}

		dotCode += "}\n";
		return dotCode;
	}

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

	string getName() {
		return this->name;
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

	void addFile(BIG_INT id, string extension, const string filepath) {
		KeyValuePair<BIG_INT, LinkedList<string>> data; data.key = id; 
		handle.createFolderIfNotExists("D:\\storage\\DHT\\" + name + "\\" + id.getBIG_INT());
		string path = "D:\\storage\\DHT\\" + name + "\\" + id.getBIG_INT() + "\\";
		Pair<BTreeNode<KeyValuePair<BIG_INT, LinkedList<string>>>*, int, int> res = indexTree.search(data);
		if (res.first == nullptr) { // if this id is not already present then just insert the node
			path += "file_" + id.getBIG_INT() + '_' + to_string(data.value.getSize()) + extension;
			data.value.push(path);
			indexTree.insert(data);
		}
		else { // collision. here we chain the situation.
			path += "file_" + res.first->keys[res.second].key.getBIG_INT() + "_" + to_string(res.first->keys[res.second].value.getSize()) + extension;
			res.first->keys[res.second].value.push(path);
			cout << res.first->keys[res.second].value;
		}
		// and add the actual file to the path too.
		handle.copyFile(filepath, path);
	}

	void removeFile(BIG_INT id) {
		KeyValuePair<BIG_INT, LinkedList<string>> data; data.key = id;
		Pair<BTreeNode<KeyValuePair<BIG_INT, LinkedList<string>>>*, int, int> res = indexTree.search(data);
		if (res.first == nullptr) {
			cout << "no file found with id: " << id.getBIG_INT() << endl;
			return;
		}; // if not there so return
		if (res.first->keys[res.second].value.getSize() == 1) { // only one value of this hash so delete the whole key
			indexTree.deleteNode(data);
			// here we delete the whole folder with the specific key too
			handle.removeFolder("D:\\storage\\DHT\\" + name + "\\" + id.getBIG_INT());
		}
		else { // here multiple values of this id so delete from its chain
			int count = 0;
			cout << "These are the files with this id: " << endl;
			for (LinkedList<string>::Iterator it = res.first->keys[res.second].value.begin(); it != res.first->keys[res.second].value.end(); ++it) {
				cout << ++count << ". " << *it << endl;
			}
			int choice;
			cout << "Enter which file to delete: "; cin >> choice;
			if (choice > 0 && choice - 1 <= res.first->keys[res.second].value.getSize()) {
				string path; int count = 0;
				for (LinkedList<string>::Iterator it = res.first->keys[res.second].value.begin(); it != res.first->keys[res.second].value.end(); ++it) {
					path = *it;
					if (count == (choice - 1)) break;
					count++;
				}
				res.first->keys[res.second].value.delete_from_index(choice);
				cout << res.first->keys[res.second].value;
				// and here we do not have to delete the whole folder just the path of the file
				handle.removeFile(path);
			}
			else {
				cout << "invalid choice entered" << endl;
			}
		}
	}

	Pair<BTreeNode<KeyValuePair<BIG_INT, LinkedList<string>>>*, int, int> searchFile(BIG_INT id) {
		KeyValuePair<BIG_INT, LinkedList<string>> data; data.key = id;
		Pair<BTreeNode<KeyValuePair<BIG_INT, LinkedList<string>>>*, int, int> res = indexTree.search(data);
		return res;
	}

	void printBtree() {
		if (this->indexTree.getRoot() == nullptr) {
			cout << "Tree is empty" << endl;
			return;
		}
		Queue<BTreeNode<KeyValuePair<BIG_INT, LinkedList<string>>>*> levelOrderQueue;
		levelOrderQueue.enqueue(this->indexTree.getRoot());
		while (!levelOrderQueue.isEmpty())
		{
			BTreeNode<KeyValuePair<BIG_INT, LinkedList<string>>>* current = levelOrderQueue.head();
			levelOrderQueue.dequeue();

			for (int i = 0; i < current->keys.getSize(); i++)
			{
				// here we need to move files too but that is later work
				cout << "key: " << current->keys[i].key << endl;
				cout << "value: " << current->keys[i].value << endl;
				cout << endl;
			}

			for (size_t i = 0; i < current->children.getSize(); ++i)
			{
				levelOrderQueue.enqueue(current->children[i]);
			}
		}
		visualizeTree(generateDotCode(indexTree.getRoot()));
	}

	void shiftFiles(char mode, Machine& other) {
		if (mode == 'i') { // if insert is mode then we add other's files with id e where e <= currentMachineId
			if (other.indexTree.getRoot() == nullptr) return;
			BIG_INT thisId = id;
			BIG_INT nextId = other.getId();
			// now we do level order traversal of others Btree and insert these files in this's Btree
			ArrayBasedList<KeyValuePair<BIG_INT, LinkedList<string>>> dataToBeDeleted;
			Queue<BTreeNode<KeyValuePair<BIG_INT, LinkedList<string>>>*> levelOrderQueue;
			levelOrderQueue.enqueue(other.indexTree.getRoot());
			while (!levelOrderQueue.isEmpty())
			{
				BTreeNode<KeyValuePair<BIG_INT, LinkedList<string>>>* current = levelOrderQueue.head();
				levelOrderQueue.dequeue();

				for (int i = 0; i < current->keys.getSize(); i++)
				{
					if (current->keys[i].key <= thisId) {
						// here we need to move files too but that is later work
						dataToBeDeleted.push(current->keys[i]);
						for (LinkedList<string>::Iterator it = current->keys[i].value.begin(); it != current->keys[i].value.end(); ++it) {
							addFile(current->keys[i].key, getExtension(*it), *it);
						}
						handle.removeFolder("D:\\storage\\DHT\\" + other.getName() + "\\" + current->keys[i].key.getBIG_INT());
					}
				}
				
				for (size_t i = 0; i < current->children.getSize(); ++i)
				{
					levelOrderQueue.enqueue(current->children[i]);
				}
			}
			for (int i = 0; i < dataToBeDeleted.getSize(); i++) {
				other.indexTree.deleteNode(dataToBeDeleted[i]);
			}
		}
		else if (mode == 'd') { // if delete is mode then we empty this machines files into other
			// now we do level order traversal of others Btree and insert these files in this's Btree
			if (this->indexTree.getRoot() == nullptr) return;
			Queue<BTreeNode<KeyValuePair<BIG_INT, LinkedList<string>>>*> levelOrderQueue;
			levelOrderQueue.enqueue(this->indexTree.getRoot());
			while (!levelOrderQueue.isEmpty())
			{
				BTreeNode<KeyValuePair<BIG_INT, LinkedList<string>>>* current = levelOrderQueue.head();
				levelOrderQueue.dequeue();

				for (int i = 0; i < current->keys.getSize(); i++)
				{
					// here we need to move files too but that is later work
					for (LinkedList<string>::Iterator it = current->keys[i].value.begin(); it != current->keys[i].value.end(); ++it) {
						other.addFile(current->keys[i].key, getExtension(*it), *it);
					}
					
				}

				for (size_t i = 0; i < current->children.getSize(); ++i)
				{
					levelOrderQueue.enqueue(current->children[i]);
				}
			}
		}
	}
};



#endif // !MACHINE_H
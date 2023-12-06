#pragma once
#include <iostream>
#include "BigInt.h"
#include "BTree.h"
#include "Machine.h"
#include "SinglyLL.h"
using namespace std;

BIG_INT power(BIG_INT num, const int power) {
    if (power == 0) return BIG_INT("1");
	BIG_INT temp = num;
	for (int i = 0; i < power - 1; i++) {
		num = num * temp;
	}
	return num;
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

string generateDotCode(BTreeNode<int>* btreeRoot)
{
    string dotCode = "digraph BTree {\n";
    dotCode += "\tnode [shape=record, height=.1];\n\n";

    Queue<BTreeNode<int>*> levelOrderQueue;
    Queue<int> idQ;
    levelOrderQueue.enqueue(btreeRoot);
    int count = 0;
    idQ.enqueue(count);
    while (!levelOrderQueue.isEmpty())
    {
        BTreeNode<int>* current = levelOrderQueue.head();
        levelOrderQueue.dequeue();
        int j = idQ.dequeue();

        dotCode += "\tnode" + to_string(j) + " [label=\"";
        for (int i = 0; i < current->keys.getSize(); i++)
        {
            dotCode += "|" + to_string(current->keys[i]);
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
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
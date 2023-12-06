#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "Queue.h"
#include "ArrayBasedList.h"
using namespace std;


template <typename T, typename U, typename V>
struct Pair {
    T first;
    U second;
    V third;
    Pair(T f, U s, V t) : first(f), second(s), third(t) {}
};
template <typename T>
class BTreeNode
{
public:
    ArrayBasedList<T> keys;
    ArrayBasedList<BTreeNode<T>*> children;
    bool leaf;
    BTreeNode<T>* p;
    BTreeNode(bool isLeaf = false) : leaf(isLeaf), p(nullptr) {}
};

template <typename T>
class BTree
{
private:
    BTreeNode<T>* root;
    int t;

public:
    BTree(int degree = 0) : t(degree)
    {
        root = new BTreeNode<T>(true);
    }

    void setDegree(int degree) {
        t = degree;
    }

    Pair<BTreeNode<T>*, int, int> search(int key, BTreeNode<T>* node = nullptr, int childIndex = 0)
    {
        node = (node == nullptr) ? root : node;

        int i = 0;
        while (i < node->keys.getSize() && key > node->keys[i])
        {
            i++;
        }

        if (i < node->keys.getSize() && key == node->keys[i])
        {
            return { node, i, childIndex };
        }
        else if (node->leaf)
        {
            return { nullptr, -1, -1 };
        }
        else
        {
            return search(key, node->children[i], i);
        }
    }

    void splitChild(BTreeNode<T>* x, int i)
    {
        BTreeNode<T>* y = x->children[i];
        BTreeNode<T>* z = new BTreeNode<T>(y->leaf);
        x->children.insert(z, i + 1);
        x->keys.insert(y->keys[t - 1], i);
        z->p = x;
        y->p = x;
        for (int i = t; i < y->keys.getSize(); i++)
        {
            z->keys.push(y->keys[i]);
        }

        for (int i = 0; i < t; i++)
        {
            y->keys.pop();
        }

        if (!y->leaf)
        {
            for (int i = t; i < y->children.getSize(); i++)
            {
                z->children.push(y->children[i]);
                y->children[i]->p = z;
            }
            for (int i = 0; i < t; i++)
            {
                y->children.pop();
            }
        }
    }

    void insert(int k)
    {
        BTreeNode<T>* r = root;
        if (r->keys.getSize() == (2 * t) - 1)
        {
            BTreeNode<T>* s = new BTreeNode<T>();
            root->p = s;
            root = s;
            s->children.push(r);
            r->p = s;
            splitChild(s, 0);
            insertNonFull(s, k);
        }
        else
        {
            insertNonFull(r, k);
        }
    }

    void insertNonFull(BTreeNode<T>* x, int k)
    {
        int i = x->keys.getSize() - 1;

        if (x->leaf)
        {
            x->keys.push(0); // Extend the vector to make room for the new key
            while (i >= 0 && k < x->keys[i])
            {
                x->keys[i + 1] = x->keys[i];
                i--;
            }
            x->keys[i + 1] = k;
        }
        else
        {
            while (i >= 0 && k < x->keys[i])
            {
                i--;
            }
            i++;
            if (x->children[i]->keys.getSize() == (2 * t) - 1)
            {
                splitChild(x, i);
                if (k > x->keys[i])
                {
                    i++;
                }
            }
            x->children[i]->p = x;
            insertNonFull(x->children[i], k);
        }
    }

    void deleteNode(int val)
    {
        Pair<BTreeNode<T>*, int, int> res = search(val);
        if (res.first == nullptr)
            return;

        if (res.first->leaf)
        {
            deleteFromLeaf(res.first, res.second, res.third);
        }
        else
        {
            deleteInternalNode(res.first, res.second, res.third);
        }
    }

    void deleteFromLeaf(BTreeNode<T>* node, int index, int childIndex)
    {
        // case 1a. That we can delete from the leaf without violating the property
        if (node->keys.getSize() >= t)
        {
            node->keys.deleteItem(index);
        }
        else
        { // Then we can not delete from node without violating property. we check the siblings for borrow
            // case 1b. check for borrow from the siblings.
            if (childIndex != 0 && node->p->children[childIndex - 1]->keys.getSize() >= t)
            { // check if we can borrow from left child
                BTreeNode<T>* leftSibling = node->p->children[childIndex - 1];
                node->keys.deleteItem(index);
                node->keys.insertSorted(node->p->keys[childIndex - 1]);
                node->p->keys[childIndex - 1] = leftSibling->keys.back();
                leftSibling->keys.pop();
            }
            else if (childIndex != node->p->children.getSize() - 1 && node->p->children[childIndex + 1]->keys.getSize() >= t)
            {
                BTreeNode<T>* rightSibling = node->p->children[childIndex + 1];
                node->keys.deleteItem(index);
                node->keys.insertSorted(node->p->keys[childIndex]);
                node->p->keys[childIndex] = rightSibling->keys.front();
                rightSibling->keys.popFront();
            }
            else
            { // here we see that we cannot borrow from anyof the siblings then we merge this shit by checking where we can merge the node. left or right
                if (childIndex == 0)
                { // then only right merging is possible
                    BTreeNode<T>* rightSibling = node->p->children[childIndex + 1];
                    rightSibling->keys.pushFront(node->p->keys.front());
                    node->p->keys.popFront();
                    for (int i = 0; i < node->keys.getSize(); i++)
                    {
                        if (i != index)
                        {
                            rightSibling->keys.pushFront(node->keys[i]);
                        }
                    }
                    for (int i = 0; i < node->p->children.getSize() - 1; i++)
                    {
                        node->p->children[i] = node->p->children[i + 1];
                    }
                    node->p->children.pop();
                }
                else if (childIndex == node->p->children.getSize() - 1)
                { // then only left merging is possible
                    BTreeNode<T>* leftSibling = node->p->children[childIndex - 1];
                    leftSibling->keys.push(node->p->keys.back());
                    node->p->keys.pop();
                    for (int i = 0; i < node->keys.getSize(); i++)
                    {
                        if (i != index)
                        {
                            leftSibling->keys.push(node->keys[i]);
                        }
                    }
                    node->p->children.pop();
                }
                else
                { // any merging side is possible we will do left in this case
                    BTreeNode<T>* leftSibling = node->p->children[childIndex - 1];
                    leftSibling->keys.push(node->p->keys[childIndex - 1]);
                    node->p->keys.deleteItem(childIndex - 1);
                    for (int i = 0; i < node->keys.getSize(); i++)
                    {
                        if (i != index)
                        {
                            leftSibling->keys.push(node->keys[i]);
                        }
                    }
                    for (int i = childIndex; i < node->p->children.getSize() - 1; i++)
                    {
                        node->p->children[i] = node->p->children[i + 1];
                    }
                    node->p->children.pop();
                }
                // now we propagate the conditions upwards to check if there is violation in the parents nodes
                BTreeNode<T>* current = node->p;
                delete node;
                while (current != root)
                {
                    BTreeNode<T>* temp = nullptr; // in case we have to delete this node after merging
                    // first check which child is the current of its parent
                    if (current->keys.getSize() >= t)
                        break;
                    int j = 0;
                    while (j < current->p->children.getSize())
                    {
                        if (current == current->p->children[j])
                            break;
                        j++;
                    }
                    // case 1b. check for borrow from the siblings.
                    if (j != 0 && current->p->children[j - 1]->keys.getSize() >= t)
                    { // check if we can borrow from left child
                        BTreeNode<T>* left = current->p->children[j - 1];
                        current->keys.insertSorted(current->p->keys[j - 1]);
                        current->p->keys[j - 1] = left->keys.back();
                        left->keys.pop();

                        current->children.pushFront(left->children.back());
                        left->children.pop();
                    }
                    else if (j != current->p->children.getSize() - 1 && current->p->children[j + 1]->keys.getSize() >= t)
                    { // here we borrow from right
                        BTreeNode<T>* right = current->p->children[j + 1];
                        current->keys.insertSorted(current->p->keys[j]);
                        current->p->keys[j] = right->keys.front();
                        right->keys.popFront();

                        right->children.front()->p = current;
                        current->children.push(right->children.front());
                        right->children.popFront();
                    }
                    else
                    { // here we see that we cannot borrow from anyof the siblings then we merge this shit by checking where we can merge the node. left or right
                        if (j == 0)
                        { // then only right merging is possible
                            BTreeNode<T>* right = current->p->children[j + 1];
                            right->keys.pushFront(current->p->keys.front());
                            current->p->keys.popFront();
                            for (int i = 0; i < current->keys.getSize(); i++)
                            {
                                right->keys.pushFront(current->keys[i]);
                            }
                            for (int i = 0; i < current->p->children.getSize() - 1; i++)
                            {
                                current->p->children[i] = current->p->children[i + 1];
                            }
                            // here current also has children to be given to right sibling
                            for (int i = current->children.getSize() - 1; i >= 0; i--)
                            {
                                current->children[i]->p = right;
                                right->children.pushFront(current->children[i]);
                            }
                            current->p->children.pop();
                        }
                        else if (j == current->p->children.getSize() - 1)
                        { // then only left merging is possible
                            BTreeNode<T>* left = current->p->children[j - 1];
                            left->keys.push(current->p->keys.back());
                            current->p->keys.pop();
                            for (int i = 0; i < current->keys.getSize(); i++)
                            {
                                left->keys.push(current->keys[i]);
                            }
                            // here current also has children to be given to left sibling
                            for (int i = 0; i < current->children.getSize(); i++)
                            {
                                current->children[i]->p = left;
                                left->children.push(current->children[i]);
                            }
                            current->p->children.pop();
                        }
                        else
                        { // any merging side is possible we will do left in this case
                            BTreeNode<T>* left = current->p->children[j - 1];
                            left->keys.push(current->p->keys[j - 1]);
                            current->p->keys.deleteItem(j - 1);
                            for (int i = 0; i < current->keys.getSize(); i++)
                            {
                                left->keys.push(current->keys[i]);
                            }
                            for (int i = j; i < current->p->children.getSize() - 1; i++)
                            {
                                current->p->children[i] = current->p->children[i + 1];
                            }
                            // here current also has children
                            for (int i = 0; i < current->children.getSize(); i++)
                            {
                                current->children[i]->p = left;
                                left->children.push(current->children[i]);
                            }
                            current->p->children.pop();
                        }
                        temp = current;
                    }
                    current = current->p;
                    if (temp)
                        delete temp;
                }
                if (root->keys.empty())
                {
                    BTreeNode<T>* temporary = root;
                    root = root->children[0];
                    delete temporary;
                }
            }
        }
    }

    void deleteInternalNode(BTreeNode<T>* node, int index, int childIndex)
    {
        // case 1 : that if we can take predecessor or successor of the current key being deleted
        BTreeNode<T>* leftChild = node->children[index];
        BTreeNode<T>* rightChild = node->children[index + 1];
        BTreeNode<T>* predecessorNode = leftChild;
        BTreeNode<T>* successorNode = rightChild;

        int keyToDelete = node->keys[index];
        // now getting the predecessor and successor nodes.
        while (!predecessorNode->children.empty())
            predecessorNode = predecessorNode->children.back();
        while (!successorNode->children.empty())
            successorNode = successorNode->children.front();

        // we would also need which child is the predecessor and successor nodes of their specific parent
        int predecessorIndex = 0, successorIndex = 0;
        for (int i = 0; i < predecessorNode->p->children.getSize(); i++)
        {
            if (predecessorNode->p->children[i] == predecessorNode)
                break;
            predecessorIndex++;
        }
        for (int i = 0; i < successorNode->p->children.getSize(); i++)
        {
            if (successorNode->p->children[i] == successorNode)
                break;
            successorIndex++;
        }

        // check to see if predecessor can be given
        if (predecessorNode->keys.getSize() >= t)
        {
            // then we can take predecessor up to the parent node and delete from leaf the original value
            int temp = predecessorNode->keys.back();
            predecessorNode->keys.pop();
            predecessorNode->keys.push(keyToDelete);
            node->keys[index] = temp;
            deleteFromLeaf(predecessorNode, predecessorNode->keys.getSize() - 1, predecessorIndex);
        }
        // check to see if successor can be brought up
        else if (successorNode->keys.getSize() >= t)
        {
            // then we can take successor up to the parent node and delete from leaf the original value
            int temp = successorNode->keys.front();
            successorNode->keys.popFront();
            successorNode->keys.pushFront(node->keys[index]);
            node->keys[index] = temp;
            deleteFromLeaf(successorNode, 0, successorIndex);
        }
        else
        { // we cannot take the predecessor or successor up. here we consider two scenarios
            if (node->keys.getSize() >= t)
            { // we take this key down to the leaf and delete from leaf is called
                int temp = predecessorNode->keys.back();
                predecessorNode->keys.pop();
                predecessorNode->keys.push(keyToDelete);
                node->keys[index] = temp;
                deleteFromLeaf(predecessorNode, predecessorNode->keys.getSize() - 1, predecessorIndex);
            }
            else
            { // we take this key down to the leaf and delete from leaf is called
                int temp = predecessorNode->keys.back();
                predecessorNode->keys.pop();
                predecessorNode->keys.push(keyToDelete);
                node->keys[index] = temp;
                deleteFromLeaf(predecessorNode, predecessorNode->keys.getSize() - 1, predecessorIndex);
            }
        }
    }

    BTreeNode<T>* getRoot() { return this->root; }

    void printTree(BTreeNode<T>* x, int level = 0)
    {
        std::cout << "Level " << level << ": ";
        for (int i = 0; i < x->keys.getSize(); i++)
        {
            std::cout << x->keys[i] << " ";
        }
        std::cout << std::endl;

        level++;
        if (!x->children.empty())
        {
            for (int i = 0; i < x->children.getSize(); i++)
            {
                printTree(x->children[i], level);
            }
        }
    }

    
};


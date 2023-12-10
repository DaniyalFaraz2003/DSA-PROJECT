#pragma once

#include <iostream>
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
    int m;

public:
    BTree(int degree = 0) : m(degree)
    {
        root = nullptr;
    }

    void setDegree(int degree) {
        m = degree;
    }

    Pair<BTreeNode<T>*, int, int> search(T key, BTreeNode<T>* node = nullptr, int childIndex = 0)
    {
        if (root == nullptr) return { nullptr, -1, -1 };
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

    Pair<BTreeNode<T>*, int, int> searchForInsert(T key, BTreeNode<T>* node = nullptr, int childIndex = 0)
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
            return { node, -1, childIndex };
        }
        else
        {
            return searchForInsert(key, node->children[i], i);
        }
    }

    void setLeafNodes() {
        Queue<BTreeNode<T>*> levelOrderQueue;
        levelOrderQueue.enqueue(root);
        root->p = nullptr;
        while (!levelOrderQueue.isEmpty()) {
            BTreeNode<T>* current = levelOrderQueue.head();
            levelOrderQueue.dequeue();
            if (current->children.getSize() == 0) {
                current->leaf = true;
            }
            else {
                current->leaf = false;
            }
            for (size_t i = 0; i < current->children.getSize(); ++i) {
                levelOrderQueue.enqueue(current->children[i]);
            }
        }
    }



    void insert(T k)
    {
        if (root == nullptr) {
            root = new BTreeNode<T>;
            root->keys.push(k);
            return;
        }
        setLeafNodes();
        Pair<BTreeNode<T>*, int, int> leafNodeForInsert = searchForInsert(k);
        BTreeNode<T>* node = leafNodeForInsert.first;
        node->keys.insertSorted(k);

        while (node && node->keys.getSize() == m) {
            int splitFrom = node->keys.getSize() / 2; // the index of the splitting node
            BTreeNode<T>* leftNode = new BTreeNode<T>;
            BTreeNode<T>* rightNode = new BTreeNode<T>;
            for (int i = 0; i < splitFrom; i++) leftNode->keys.push(node->keys[i]); // insertion in left childs of the parent which is split
            for (int i = splitFrom + 1; i < m; i++) rightNode->keys.push(node->keys[i]);

            if (node == root) {
                T splitKey = node->keys[splitFrom]; // the key which is to be moved to parent
                if (node->keys.getSize() == m) {
                    manageChilds(node, leftNode, rightNode, splitFrom);
                }
                node->keys.clear(); node->keys.push(splitKey);
                node->children.clear();
                node->children.push(leftNode); node->children.push(rightNode);
                leftNode->p = node; rightNode->p = node;
            }
            else {
                node->p->keys.insertSorted(node->keys[splitFrom]);
                int currentChildIndex = 0;
                for (int i = 0; i < node->p->children.getSize(); i++) {
                    if (node == node->p->children[i]) break;
                    currentChildIndex++;
                }
                node->p->children[currentChildIndex] = leftNode;
                node->p->children.insert(rightNode, currentChildIndex + 1);
                leftNode->p = node->p; rightNode->p = node->p;
                if (node->keys.getSize() == m) {
                    manageChilds(node, leftNode, rightNode, splitFrom);
                }
                node->children.clear();
                node->children.push(leftNode); node->children.push(rightNode);
            }
            BTreeNode<T>* next = node->p;
            if (next != nullptr) delete node;
            node = next;
            setLeafNodes();
        }
    }

    void manageChilds(BTreeNode<T>* node, BTreeNode<T>* left, BTreeNode<T>* right, int mid) {
        for (int i = 0, j = 0; i <= mid; i++, j++) {
            if (i < node->children.getSize()) {
                left->children.push(node->children[i]);
                if (j < left->children.getSize()) left->children[j]->p = left;
                left->leaf = false;
            }
        }
        for (int i = mid + 1, j = 0; i <= m; i++, j++) {
            if (i < node->children.getSize()) {
                right->children.push(node->children[i]);
                if (j < right->children.getSize()) right->children[j]->p = right;
                right->leaf = false;
            }
        }
    }


    void deleteNode(T val)
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
        if (node->keys.getSize() > ceil(m / 2.0) - 1)
        {
            node->keys.deleteItem(index);
        }
        else
        { // Then we can not delete from node without violating property. we check the siblings for borrow
            // case 1b. check for borrow from the siblings.
            if (childIndex != 0 && node->p->children[childIndex - 1]->keys.getSize() > ceil(m / 2.0) - 1)
            { // check if we can borrow from left child
                BTreeNode<T>* leftSibling = node->p->children[childIndex - 1];
                node->keys.deleteItem(index);
                node->keys.insertSorted(node->p->keys[childIndex - 1]);
                node->p->keys[childIndex - 1] = leftSibling->keys.back();
                leftSibling->keys.pop();
            }
            else if (childIndex != node->p->children.getSize() - 1 && node->p->children[childIndex + 1]->keys.getSize() > ceil(m / 2.0) - 1)
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
                    if (current->keys.getSize() >= ceil(m / 2.0) - 1)
                        break;
                    int j = 0;
                    while (j < current->p->children.getSize())
                    {
                        if (current == current->p->children[j])
                            break;
                        j++;
                    }
                    // case 1b. check for borrow from the siblings.
                    if (j != 0 && current->p->children[j - 1]->keys.getSize() > ceil(m / 2.0) - 1)
                    { // check if we can borrow from left child
                        BTreeNode<T>* left = current->p->children[j - 1];
                        current->keys.insertSorted(current->p->keys[j - 1]);
                        current->p->keys[j - 1] = left->keys.back();
                        left->keys.pop();

                        left->children.back()->p = current;
                        current->children.pushFront(left->children.back());
                        left->children.pop();
                    }
                    else if (j != current->p->children.getSize() - 1 && current->p->children[j + 1]->keys.getSize() > ceil(m / 2.0) - 1)
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

        T keyToDelete = node->keys[index];
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
        if (predecessorNode->keys.getSize() > ceil(m / 2.0) - 1)
        {
            // then we can take predecessor up to the parent node and delete from leaf the original value
            T temp = predecessorNode->keys.back();
            predecessorNode->keys.pop();
            predecessorNode->keys.push(keyToDelete);
            node->keys[index] = temp;
            deleteFromLeaf(predecessorNode, predecessorNode->keys.getSize() - 1, predecessorIndex);
        }
        // check to see if successor can be brought up
        else if (successorNode->keys.getSize() > ceil(m / 2.0) - 1)
        {
            // then we can take successor up to the parent node and delete from leaf the original value
            T temp = successorNode->keys.front();
            successorNode->keys.popFront();
            successorNode->keys.pushFront(node->keys[index]);
            node->keys[index] = temp;
            deleteFromLeaf(successorNode, 0, successorIndex);
        }
        else
        { // we cannot take the predecessor or successor up. here we consider two scenarios
            if (node->keys.getSize() > ceil(m / 2.0) - 1)
            { // we take this key down to the leaf and delete from leaf is called
                T temp = predecessorNode->keys.back();
                predecessorNode->keys.pop();
                predecessorNode->keys.push(keyToDelete);
                node->keys[index] = temp;
                deleteFromLeaf(predecessorNode, predecessorNode->keys.getSize() - 1, predecessorIndex);
            }
            else
            { // we take this key down to the leaf and delete from leaf is called
                T temp = predecessorNode->keys.back();
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

    ~BTree() {

        BTreeNode<T>* rootnode = getRoot();
        if (rootnode == nullptr) return;
        Queue<BTreeNode<T>*> q;
        q.enqueue(rootnode);
        while (!q.isEmpty()) {

            BTreeNode<T>* current = q.head();
            q.dequeue();
            for (int i = 0; i < current->children.getSize(); i++) {
                q.enqueue(current->children[i]);
            }
            delete current;
        }
    }
};


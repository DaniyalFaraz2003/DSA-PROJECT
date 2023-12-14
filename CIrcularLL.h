#pragma once

#ifndef CIRCULARLL_H
#define CIRCULARLL_H

#include <iostream>
#include <string>
using namespace std;

template <typename T>
class CircleListNode
{
public:
	T data;
	CircleListNode* next;

        //constructor
	CircleListNode(T data) : data(data), next(nullptr) {}
};

template <typename T>
class CircularLinkedList
{
	int size = 0;
public:
	CircleListNode<T>* head;
        //default constructor
	CircularLinkedList() : head(nullptr) {}
        //copy constructor
	CircularLinkedList(const CircularLinkedList<T>& other)
	{
		if (other.head == nullptr) return;
		//copy the head node
		this->head = new CircleListNode<T>(other.head->data);
		CircleListNode<T>* curr = other.head->next;
		CircleListNode<T>* Next = this->head;
		//copy the rest of the nodes
		while (curr)
		{
			Next->next = new CircleListNode<T>(curr->data);
			Next = Next->next;
			if (curr->next == other->head)
				break;
			curr = curr->next;
		}
		Next->next = this->head;
	}
        //overloading the assignment operator
	CircularLinkedList<T> operator=(const CircularLinkedList<T>& other)
	{
		if (other.head == nullptr) {
			this->size = other.size;
			makenull(); return *this;
		};
		this->size == other.size;
		this->head = new CircleListNode<T>(other.head->data);
		CircleListNode<T>* curr = other.head->next;
		CircleListNode<T>* Next = this->head;
		while (curr)
		{
			Next->next = new CircleListNode<T>(curr->data);
			Next = Next->next;
			//break the loop if next node is the head 
			if (curr->next == other->head)
				break;
			curr = curr->next;
		}
		Next->next = this->head;
		return *this;
	}
       // checks if the list is empty
	bool isEmpty() const
	{

		return head == nullptr;
	}
      //pushes a new node to the end of the list
	void push(T value)
	{

		CircleListNode<T>* newNode = new CircleListNode<T>(value);

		if (isEmpty())
		{
                   //if list is empty, set the new node as the head
			head = newNode;
			head->next = head;
		}

		else
		{
                    //traverse to the end and connect the new node  
			CircleListNode<T>* currNode = head;

			while (currNode->next != head)
			{

				currNode = currNode->next;
			}

			currNode->next = newNode;
			newNode->next = head;
		}
		size++;
	}
     //pushes a new node to the front of the list
	void push_front(T value)
	{

		CircleListNode<T>* newNode = new CircleListNode<T>(value);

		if (isEmpty())
		{
                      //if list is empty, set the new node as the head
			head = newNode;
			head->next = head;
		}

		else
		{
                     
			CircleListNode<T>* currNode = head;
                     //traverse to the end and connect the new node
			while (currNode->next != head)
			{

				currNode = currNode->next;
			}

			currNode->next = newNode;
			newNode->next = head;
			head = newNode;
		}
		size++;
	}
   //pop the front node of the list
	void pop_front()
	{

		if (isEmpty())
		{
			cout << "List is empty \n";
			return;
		}

		CircleListNode<T>* tempNode = head;

		while (tempNode->next != head)
		{
                    
			tempNode = tempNode->next;
		}

		if (tempNode == head)
		{
                    //when there is only one node, we delete it and set the head to nullptr
			delete tempNode;
			head = nullptr;
		}

		else
		{
                    //delete the front node by updating connections in the list
			tempNode->next = head->next;
			delete head;
			head = tempNode->next;
		}
		size--;
		return;
	}
       //pop the last node of the list
	void pop()
	{


		if (isEmpty())
		{

			cout << "List is empty \n";
			return;
		}

		CircleListNode<T>* prevNode = nullptr;
		CircleListNode<T>* currNode = head;

		while (currNode->next != head)
		{

			prevNode = currNode;
			currNode = currNode->next;
		}

		if (currNode == head)
		{
                    //when there is only one node, we delete it and set the head to nullptr
			delete head;
			head = nullptr;
		}

		else
		{
                    //delete the node by updating connections
			prevNode->next = head;
			delete currNode;
		}
		size--;
	}
       //insert a node at a specific index
	void insert_at_index(int index, T value)
	{

		CircleListNode<T>* newNode = new CircleListNode<T>(value);

		if (index < 0)
		{

			cout << "Index is invalid \n";
		}

		if (index == 0)
		{
                  //if insertion is at the beginning, set the new node as the head
			head = newNode;
			head->next = head;
			size++;
		}

		else
		{

			CircleListNode<T>* current = head;
                      //traverse to the index specified and insert the new node
			for (int i = 0; i < index - 1 && current != head; ++i)
			{

				current = current->next;
			}

			if (current == nullptr)
			{

				cout << "Index is invalid \n";
				delete newNode;
				return;
			}

			newNode->next = current->next;
			current->next = newNode;
			size++;
		}
	}
     //deletes a node from a specific index
	void delete_from_index(int index)
	{


		if (isEmpty() || index < 0)
		{

			cout << "Index is invalid or the list is empty \n";
			return;
		}

		if (index == 0)
		{
                    //deletion of the front node
			CircleListNode<T>* tempNode = head;

			if (head->next == head)
			{

				delete tempNode;
				head = nullptr;
			}

			else
			{

				head = head->next;
				CircleListNode<T>* last = head;

				while (last->next != tempNode)
				{

					last = last->next;
				}

				last->next = head;
				delete tempNode;
			}
		}

		else
		{

			CircleListNode<T>* current = head;
			CircleListNode<T>* prevNode = nullptr;
                        //traverse to the specified index and delete the node
			for (int i = 0; i < index && current->next != head; ++i)
			{

				prevNode = current;
				current = current->next;
			}

			if (current == nullptr)
			{

				cout << "Index is invalid \n";
				return;
			}

			prevNode->next = current->next;

			delete current;
		}
		size--;
	}
        //concatenates two circular linked lists
	CircularLinkedList<T> operator+(const CircularLinkedList<T>& ot) const
	{

		CircularLinkedList<T> resultant = *this;

		if (!resultant.isEmpty())
		{

			CircleListNode<T>* last1 = resultant.head;

			while (last1->next != resultant.head)
			{

				last1 = last1->next;
			}

			last1->next = ot.head;
		}

		if (!ot.isEmpty())
		{

			CircleListNode<T>* last2 = ot.head;

			while (last2->next != ot.head)
			{

				last2 = last2->next;
			}

			last2->next = resultant.head;
		}

		return resultant;
	}
       //inserts value in the list in a sorted manner
	void insertSorted(T value)
	{

		if (isEmpty() || value < head->data)
		{

			push_front(value);
		}

		else
		{

			CircleListNode<T>* newNode = new CircleListNode<T>(value);
			CircleListNode<T>* current = head;
                      //traverse the list to find the correct position to insert the value
			while (current->next != head && current->next->data < value)
			{

				current = current->next;
			}

			newNode->next = current->next;
			current->next = newNode;
			size++;
		}
	}
       //clears the list, frees up memory
	void makenull()
	{
		size = 0;
		if (head == nullptr) return;
		CircleListNode<T>* next;
		CircleListNode<T>* current = head;

		do
		{

			next = current->next;
			delete current;
			current = next;

		} while (current != head);

		head = nullptr;
	}
      //returns the value if it is found in the list
	T search(const T& value)
	{

		CircleListNode<T>* current = head;

		while (current != nullptr)
		{

			if (current->data == value)
			{

				return current->data;
			}

			current = current->next;
		}
	}
      //returns true if a value exists in the list, else it returns false
	bool exists(const T& value)
	{
		if (isEmpty()) return false;
		CircleListNode<T>* current = head;

		while (current->next != head)
		{

			if (current->data == value)
			{

				return true;
			}

			current = current->next;
		}
		if (current->data == value) return true;
		return false;
	}
        //overloading ostream operator to print the list
	friend ostream& operator<<(ostream& out, const CircularLinkedList<T>& circularLL)
	{

		CircleListNode<T>* current = circularLL.head;

		do
		{

			out << current->data << " ";
			current = current->next;

		} while (current != circularLL.head);
		out << endl;
		return out;
	}
      //returns size of the list
	int getSize() {
		return this->size;
	}
      //destructor 
	~CircularLinkedList() {
		makenull();
	}
};


#endif // !CIRCULARLL_H

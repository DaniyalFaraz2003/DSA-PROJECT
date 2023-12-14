#pragma once
#ifndef SINGLYLL_H
#define SINGLYLL_H

#include <iostream>
#include <string>
using namespace std;

// Returns true if s1 is substring of s2
bool isSubstring(string s1, string s2)
{
	int M = s1.length();
	int N = s2.length();


	for (int i = 0; i <= N - M; i++) {
		int j;


		for (j = 0; j < M; j++)
			if (s2[i + j] != s1[j])
				break;

		if (j == M)
			return true;
	}

	return false;
}

template <typename T>
class ListNode
{
public:
	T data;
	ListNode* next;
	//constructor
	ListNode(T data) : data(data), next(nullptr) {}
};

template <typename T>
class LinkedList
{
	int size = 0;
	ListNode<T>* head;

public:
	//constructor
	LinkedList() : head(nullptr) {}
	//copy constructor
	LinkedList(const LinkedList<T>& other)
	{
		if (other.head == nullptr) {
			this->size = other.size;
			this->head = nullptr; return;
		};
		this->size = other.size;
		this->head = new ListNode<T>(other.head->data);
		ListNode<T>* curr = other.head->next;
		ListNode<T>* Next = this->head;
		while (curr)
		{
			Next->next = new ListNode<T>(curr->data);
			Next = Next->next;
			curr = curr->next;
		}
	}
	//overloading the assignment operator
	LinkedList<T> operator=(const LinkedList<T>& other)
	{
		if (other.head == nullptr) {
			this->size = other.size;
			makenull(); this->head = nullptr; return *this;
		};
		makenull();
		this->size = other.size;
		this->head = new ListNode<T>(other.head->data);
		ListNode<T>* curr = other.head->next;
		ListNode<T>* Next = this->head;
		while (curr)
		{
			Next->next = new ListNode<T>(curr->data);
			Next = Next->next;
			curr = curr->next;
		}
		return *this;
	}
	//Iterator class for the linked list
	class Iterator
	{

		ListNode<T>* node;

	public:
		Iterator(ListNode<T>* node) : node(node) {}

		T operator*() const
		{

			return node->data;
		}

		T& operator*()
		{

			return node->data;
		}

		ListNode<T>* operator+(const int val)
		{
			return node->next;
		}

		Iterator& operator++()
		{

			node = node->next;
			return *this;
		}

		bool operator!=(const Iterator& ot) const
		{

			return node != ot.node;
		}
	};
	//returns an iterator pointing to the beginning of the list
	Iterator begin() const
	{

		return Iterator(head);
	}
	//returns an iterator pointing to the end of the list
	Iterator end() const
	{

		return Iterator(nullptr);
	}
	//checks if the list is empty
	bool isEmpty()
	{

		return head == nullptr;
	}
	//pushes a new node to the end of the list
	void push(T value)
	{

		ListNode<T>* newNode = new ListNode<T>(value);

		if (isEmpty())
		{

			head = newNode;
		}

		else
		{

			ListNode<T>* temp = head;

			while (temp->next != nullptr)
			{

				temp = temp->next;
			}

			temp->next = newNode;
		}
		size++;
	}
	//pushes a new node to the front of the list
	void push_front(T value)
	{

		ListNode<T>* newNode = new ListNode<T>(value);
		newNode->next = head;
		head = newNode;
		size++;
	}
	//deletes a node from the front of the list
	T pop_front()
	{

		if (isEmpty())
		{

			cout << "List is empty \n";
			return T();
		}

		ListNode<T>* temp = head;
		T value;

		value = temp->data;
		head = head->next;
		size--;
		delete temp;
		return value;
	}
	//deletes a node from the end of the list
	T pop()
	{

		if (isEmpty())
		{

			cout << "List is empty \n";
			return T();
		}

		if (head->next == nullptr)
		{

			T val = head->data;
			delete head;
			size--;
			head = nullptr;
			return val;
		}

		ListNode<T>* prevNode = nullptr;
		ListNode<T>* currNode = head;

		while (currNode->next != nullptr)
		{

			prevNode = currNode;
			currNode = currNode->next;
		}

		T val = currNode->data;
		delete currNode;
		size--;
		prevNode->next = nullptr;
		return val;
	}
	//inserts a node at a specified index in the list
	void insert_at_index(int index, T value)
	{

		ListNode<T>* newNode = new ListNode<T>(value);

		if (index < 0)
		{

			cout << "Index is invalid \n";
		}

		if (index == 0)
		{

			newNode->next = head;
			head = newNode;
			size++;
		}

		else
		{

			ListNode<T>* current = head;

			for (int i = 0; i < index - 1 && current != nullptr; ++i)
			{

				current = current->next;
			}

			if (current == nullptr)
			{

				cout << "Index is invalid \n";
				delete newNode;
			}

			newNode->next = current->next;
			current->next = newNode;
			size++;
		}
	}
	//deletes a node from a specified index in the list
	T delete_from_index(int index)
	{

		T value;

		if (isEmpty() || index <= 0)
		{

			cout << "Index is invalid or the list is empty \n";
			return T();
		}

		if (index == 1)
		{

			ListNode<T>* tempNode = head;
			value = tempNode->data;
			head = head->next;
			size--;
			delete tempNode;
			return value;
		}

		else
		{

			ListNode<T>* current = head;
			ListNode<T>* prevNode = nullptr;

			for (int i = 0; i < index - 1 && current != nullptr; ++i)
			{

				prevNode = current;
				current = current->next;
			}

			if (current == nullptr)
			{

				cout << "Index is invalid \n";
				return T();
			}

			value = current->data;
			prevNode->next = current->next;
			size--;
			delete current;
			return value;
		}
	}
	//clears the list, frees it from memory
	void makenull()
	{
		if (head == nullptr) return;
		ListNode<T>* next;
		ListNode<T>* current = head;

		while (current != nullptr)
		{

			next = current->next;
			delete current;
			current = next;
		}
		size = 0;
		head = nullptr;
	}

	//searches for a value in the list and returns it if found
	T search(const T& value)
	{

		ListNode<T>* current = head;

		while (current != nullptr)
		{

			if (current->data == value)
			{

				return current->data;
			}

			current = current->next;
		}
	}
	//checks if a value exists in the list
	T exists(const T& value)
	{

		ListNode<T>* current = head;

		while (current != nullptr)
		{

			if (current->data == value)
			{

				return true;
			}

			current = current->next;
		}

		return false;
	}
	//concatenates two lists
	LinkedList<T> operator+(const LinkedList<T>& l1) const
	{

		LinkedList<T> resultant = *this;

		for (auto it = l1.begin(); it != l1.end(); ++it)
		{

			resultant.push(*it);
		}

		return resultant;
	}
	//overloading the ostream operator to print the list
	friend ostream& operator<<(ostream& out, const LinkedList<T>& singlyLL)
	{

		ListNode<T>* currNode = singlyLL.head;

		for (Iterator it = singlyLL.begin(); it != singlyLL.end(); ++it)
		{
			if (it + 1 == nullptr)
			{
				out << "[" << *it << "]";
			}
			else
			{
				out << "[" << *it << "]"
					<< "->";
			}
		}
		out << endl;
		return out;
	}
	//returns the size of the list
	int getSize() {
		return this->size;
	}
	//function to delete a node containing a specific substring in its data
	template <typename string>
	void deleteNodeByString(const string filename) {
		if (isEmpty())
		{
			cout << "Index is invalid or the list is empty \n";
			return;
		}
		if (isSubstring(filename, head->data))
		{       //if the substring is present in the head node's data, we delete the head node
			ListNode<string>* tempNode = head;
			head = head->next;
			size--;
			delete tempNode;
			return;
		}
		else
		{
			ListNode<string>* current = head;
			ListNode<string>* prevNode = nullptr;
			//traverse the list until the substring is found
			for (int i = 0; current != nullptr && !isSubstring(filename, current->data); ++i)
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
			size--;
			delete current;
		}
	}
	//destructor
	~LinkedList() {
		makenull();
	}
};

#endif // !SINGLYLL_H
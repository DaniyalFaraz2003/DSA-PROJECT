#pragma once
#ifndef DOUBLYLL_H
#define DOUBLYLL_H


#include <iostream>
#include <string>
using namespace std;

template <typename T>
class DoubleListNode
{
public:
	T data;
	DoubleListNode* next;
	DoubleListNode* prev;

	DoubleListNode(T data) : data(data), next(nullptr), prev(nullptr) {}
};

template <typename T>
class DoublyLinkedList
{

	DoubleListNode<T>* head;
	DoubleListNode<T>* tail;

public:
	DoublyLinkedList() : head(nullptr), tail(nullptr) {}
	DoublyLinkedList(const DoublyLinkedList<T>& other)
	{
		if (other.head == nullptr) {
			this->head = nullptr; return;
		}
		this->head = new DoubleListNode<T>(other.head->data);
		DoubleListNode<T>* curr = other.head->next;
		DoubleListNode<T>* Next = this->head;
		while (curr)
		{
			Next->next = new DoubleListNode<T>(curr->data);
			Next->next->prev = Next;
			Next = Next->next;
			curr = curr->next;
		}
	}

	DoublyLinkedList<T> operator=(const DoublyLinkedList<T>& other)
	{
		if (other.head == nullptr) {
			this->makenull();
			this->head = nullptr; return;
		}
		this->head = new DoubleListNode<T>(other.head->data);
		DoubleListNode<T>* curr = other.head->next;
		DoubleListNode<T>* Next = this->head;
		while (curr)
		{
			Next->next = new DoubleListNode<T>(curr->data);
			Next->next->prev = Next;
			Next = Next->next;
			curr = curr->next;
		}
		return *this;
	}

	class Iterator
	{

		DoubleListNode<T>* node;

	public:
		Iterator(DoubleListNode<T>* node) : node(node) {}

		T operator*() const
		{

			return node->data;
		}

		T& operator*() {
			return node->data;
		}

		DoubleListNode<T>* operator+(const int val)
		{
			return node->next;
		}
		Iterator& operator++()
		{

			node = node->next;
			return *this;
		}

		Iterator& operator--()
		{

			node = node->prev;
			return *this;
		}

		bool operator!=(const Iterator& ot) const
		{

			return node != ot.node;
		}
	};

	Iterator begin() const
	{

		return Iterator(head);
	}

	Iterator end() const
	{

		return Iterator(nullptr);
	}

	T& front() {
		return this->head->data;
	}

	bool isEmpty()
	{

		return head == nullptr;
	}

	void push(T value)
	{

		DoubleListNode<T>* newNode = new DoubleListNode<T>(value);

		if (isEmpty())
		{

			head = tail = newNode;
		}

		else
		{

			tail->next = newNode;
			newNode->prev = tail;
			tail = newNode;
		}
	}

	void push_front(T value)
	{

		DoubleListNode<T>* newNode = new DoubleListNode<T>(value);

		if (isEmpty())
		{

			head = tail = newNode;
		}

		else
		{

			newNode->next = head;
			head->prev = newNode;
			head = newNode;
		}
	}

	T pop_front()
	{

		if (isEmpty())
		{

			cout << "The list is empty \n";
		}

		DoubleListNode<T>* tempNode = head;
		;
		T deletedValue = tempNode->data;

		if (head == tail)
		{

			delete tempNode;
			head = tail = nullptr;
		}

		else
		{

			head = head->next;
			head->prev = nullptr;
			delete tempNode;
		}

		return deletedValue;
	}

	T pop()
	{

		if (isEmpty())
		{

			cout << "The list is empty \n";
		}

		DoubleListNode<T>* tempNode = tail;
		T deletedValue = tempNode->data;

		if (tail == head)
		{

			delete tempNode;
			head = tail = nullptr;
		}

		else
		{

			tail = tail->prev;
			tail->next = nullptr;
			delete tempNode;
		}

		return deletedValue;
	}

	void insert_at_index(int index, T value)
	{

		DoubleListNode<T>* newNode = new DoubleListNode<T>(value);

		if (index < 0)
		{

			cout << "Invalid index \n";
		}

		if (index == 0)
		{

			newNode->next = head;

			if (head != nullptr)
			{

				head->prev = newNode;
			}

			head = newNode;
		}

		else
		{

			DoubleListNode<T>* current = head;

			for (int i = 0; i < index - 1 && current != nullptr; ++i)
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
			newNode->prev = current;

			if (current->next != nullptr)
			{

				current->next->prev = newNode;
			}

			current->next = newNode;
		}
	}

	T delete_from_index(int index)
	{

		T value;

		if (isEmpty() || index < 0)
		{

			cout << "Index is invalid or the list is empty \n";
			return T();
		}

		if (index == 0)
		{

			DoubleListNode<T>* tempNode = head;
			value = tempNode->data;

			if (head == tail)
			{

				head = tail = nullptr;
			}

			else
			{

				head = head->next;
				head->prev = nullptr;
			}

			delete tempNode;
			return value;
		}

		else
		{

			DoubleListNode<T>* current = head;
			DoubleListNode<T>* prevNode = nullptr;

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

			if (current->prev != nullptr)
			{

				current->prev->next = current->next;
			}

			if (current->next != nullptr)
			{

				current->next->prev = current->prev;
			}

			value = current->data;
			delete current;
			return value;
		}
	}

	void makenull()
	{

		DoubleListNode<T>* next;
		DoubleListNode<T>* current = head;

		while (current != nullptr)
		{

			next = current->next;
			delete current;
			current = next;
		}

		head = nullptr;
	}

	T search(const T& value)
	{

		DoubleListNode<T>* current = head;

		while (current != nullptr)
		{

			if (current->data == value)
			{

				return current->data;
			}

			current = current->next;
		}
	}

	T exists(const T& value)
	{

		DoubleListNode<T>* current = head;

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

	DoublyLinkedList<T> operator+(const DoublyLinkedList<T>& l1) const
	{

		DoublyLinkedList<T> resultant = *this;

		for (Iterator it = l1.begin(); it != l1.end(); ++it)
		{

			resultant.push(*it);
		}

		return resultant;
	}

	friend ostream& operator<<(ostream& out, const DoublyLinkedList<T>& DoublyLL)
	{
		
		DoubleListNode<T>* currNode = DoublyLL.head;
		if (currNode == nullptr) return out;
		for (Iterator it = DoublyLL.begin(); it != DoublyLL.end(); ++it)
		{
			if (it + 1 == nullptr)
			{
				out << "[" << *it << "]";
			}
			else
			{
				out << "[" << *it << "]"
					<< "<->";
			}
		}
		out << endl;
		return out;
	}

	~DoublyLinkedList() {
		makenull();
	}
};

#endif // !DOUBLYLL_H
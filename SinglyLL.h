#pragma once
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

	ListNode(T data) : data(data), next(nullptr) {}
};

template <typename T>
class LinkedList
{
	int size = 0;
	ListNode<T>* head;

public:
	LinkedList() : head(nullptr) {}
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

	Iterator begin() const
	{

		return Iterator(head);
	}

	Iterator end() const
	{

		return Iterator(nullptr);
	}

	bool isEmpty()
	{

		return head == nullptr;
	}

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

	void push_front(T value)
	{

		ListNode<T>* newNode = new ListNode<T>(value);
		newNode->next = head;
		head = newNode;
		size++;
	}

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

	LinkedList<T> operator+(const LinkedList<T>& l1) const
	{

		LinkedList<T> resultant = *this;

		for (auto it = l1.begin(); it != l1.end(); ++it)
		{

			resultant.push(*it);
		}

		return resultant;
	}

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

	int getSize() {
		return this->size;
	}

	template <typename string>
	void deleteNodeByString(const string filename) {
		if (isEmpty())
		{
			cout << "Index is invalid or the list is empty \n";
			return;
		}
		if (isSubstring(filename, head->data))
		{
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

	~LinkedList() {
		makenull();
	}
};

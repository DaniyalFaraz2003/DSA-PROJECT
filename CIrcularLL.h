#pragma once
#include <iostream>
#include <string>
using namespace std;

template <typename T>
class CircleListNode
{
public:
	T data;
	CircleListNode* next;

	CircleListNode(T data) : data(data), next(nullptr) {}
};

template <typename T>
class CircularLinkedList
{
public:
	CircleListNode<T>* head;
	CircularLinkedList() : head(nullptr) {}
	CircularLinkedList(const CircularLinkedList<T>& other)
	{
		if (other.head == nullptr) return;
		this->head = new CircleListNode<T>(other.head->data);
		CircleListNode<T>* curr = other.head->next;
		CircleListNode<T>* Next = this->head;
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

	CircularLinkedList<T> operator=(const CircularLinkedList<T>& other)
	{
		if (other.head == nullptr) {
			makenull(); return *this;
		};
		this->head = new CircleListNode<T>(other.head->data);
		CircleListNode<T>* curr = other.head->next;
		CircleListNode<T>* Next = this->head;
		while (curr)
		{
			Next->next = new CircleListNode<T>(curr->data);
			Next = Next->next;
			if (curr->next == other->head)
				break;
			curr = curr->next;
		}
		Next->next = this->head;
		return *this;
	}

	bool isEmpty() const
	{

		return head == nullptr;
	}

	void push(T value)
	{

		CircleListNode<T>* newNode = new CircleListNode<T>(value);

		if (isEmpty())
		{

			head = newNode;
			head->next = head;
		}

		else
		{

			CircleListNode<T>* currNode = head;

			while (currNode->next != head)
			{

				currNode = currNode->next;
			}

			currNode->next = newNode;
			newNode->next = head;
		}
	}

	void push_front(T value)
	{

		CircleListNode<T>* newNode = new CircleListNode<T>(value);

		if (isEmpty())
		{

			head = newNode;
			head->next = head;
		}

		else
		{

			CircleListNode<T>* currNode = head;

			while (currNode->next != head)
			{

				currNode = currNode->next;
			}

			currNode->next = newNode;
			newNode->next = head;
			head = newNode;
		}
	}

	T pop_front()
	{

		if (isEmpty())
		{
			cout << "List is empty \n";
		}

		T val = head->data;
		CircleListNode<T>* tempNode = head;

		while (tempNode->next != head)
		{

			tempNode = tempNode->next;
		}

		if (tempNode == head)
		{

			delete tempNode;
			head = nullptr;
		}

		else
		{

			tempNode->next = head->next;
			delete head;
			head = tempNode->next;
		}

		return val;
	}

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

			delete head;
			head = nullptr;
		}

		else
		{

			prevNode->next = head;
			delete currNode;
		}

	}

	void insert_at_index(int index, T value)
	{

		CircleListNode<T>* newNode = new CircleListNode<T>(value);

		if (index < 0)
		{

			cout << "Index is invalid \n";
		}

		if (index == 0)
		{

			head = newNode;
			head->next = head;
		}

		else
		{

			CircleListNode<T>* current = head;

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
		}
	}

	void delete_from_index(int index)
	{


		if (isEmpty() || index < 0)
		{

			cout << "Index is invalid or the list is empty \n";
			return;
		}

		if (index == 0)
		{

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

	}

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

			while (current->next != head && current->next->data < value)
			{

				current = current->next;
			}

			newNode->next = current->next;
			current->next = newNode;
		}
	}

	void makenull()
	{

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

	~CircularLinkedList() {
		makenull();
	}
};

#pragma once
#include <iostream>
using namespace std;

template <class T>
struct QueueNode {
	T data;
	QueueNode<T>* next;
	QueueNode(T data) : data(data) {}
};

template <class T>
class Queue {
	QueueNode<T>* front;
	QueueNode<T>* rear;
	int s;
	int currentSum;
public:
	Queue() {
		front = rear = nullptr;
		s = 0;
		currentSum = 0;
	}
	bool isEmpty() {
		return s == 0;
	}
	void enqueue(T data) {
		QueueNode<T>* newNode = new QueueNode<T>(data);
		if (isEmpty()) {
			front = newNode;
			rear = newNode;
		}
		else {
			rear->next = newNode;
			rear = newNode;
		}
		s++;
	}
	T head() {
		return front->data;
	}
	T tail() {
		return rear->data;
	}
	T dequeue() {
		QueueNode<T>* temp = nullptr;
		T val = 0;
		if (isEmpty()) {

		}
		else {
			val = front->data;
			temp = front->next;
			delete front;
			front = temp;
			s--;
			if (!s) rear = nullptr;
		}
		return val;
	}
	int size() {
		return s;
	}

	void clear() {
		while (!isEmpty()) dequeue();
	}
	~Queue() {
		clear();
	}
};


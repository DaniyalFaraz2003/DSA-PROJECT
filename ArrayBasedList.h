#pragma once
#ifndef ARRAYBASEDLIST_H
#define ARRAYBASEDLIST_H


#include <iostream>
using namespace std;

/*
A robust class designed to simulate the functionalities of the in built vector class of standard C++ library
provides all necessary functionality for a list and can behave like both a stack and a queue.
the array based approach made it easier for direct access of elements through the [] operator.
*/

template<class T>
class ArrayBasedList {
private:
    int size;
    int currentPtr;
    //function to increase the size of the array
    void increaseSize() {
        int n = size;
        size *= 2;
        T* temp = new T[size];
        for (int i = 0; i < n; i++) {
            temp[i] = arr[i];
        }
        delete[] arr;
        arr = temp;
    }
public:
    T* arr;
    //constructor
    ArrayBasedList<T>() {
        size = 5;
        currentPtr = -1;
        arr = new T[size];
    }
    //copy constructor
    ArrayBasedList<T>(const ArrayBasedList<T>& other) {
        this->size = other.size;
        this->currentPtr = other.currentPtr;
        this->arr = new T[size];
        for (int i = 0; i < getSize(); i++) {
            this->arr[i] = other.arr[i];
        }
    }
    //this function inserts the value in a sorted manner
    void insertSorted(T val) {
        bool inBetween = false;
        int index = 0;
        for (int i = 0; i < getSize(); i++) {
            if (arr[i] > val) {
                inBetween = true;
                index = i;
                break;
            }
        }
        if (inBetween) insert(val, index);
        else push(val);
    }
    //overloading the assignment operator
    ArrayBasedList<T>& operator=(const ArrayBasedList<T>& other) {
        delete[] this->arr;
        this->size = other.size;
        this->currentPtr = other.currentPtr;
        this->arr = new T[size];
        for (int i = 0; i < getSize(); i++) {
            this->arr[i] = other.arr[i];
        }
        return *this;
    }
    //this function checks if the list is empty
    bool empty() {
        return currentPtr == -1;
    }
    //pushes an element to the end of the list
    void push(T data) {
        if (currentPtr == size - 1) {
            increaseSize();
        }
        currentPtr++;
        arr[currentPtr] = data;
    }
    //clears the list
    void clear() {
        currentPtr = -1;
    }
    //deletes the element at the beginning of the list
    void popFront() {
        deleteItem(0);
    }
    //pushes an element at the beginning of the list
    void pushFront(T val) {
        insert(val, 0);
    }
    //returns the element at the beginning of the list
    T front() {
        if (!empty()) return arr[0];
        return T();
    }
   //removes the element at the end of the list
    void pop() {
        if (empty()) {
            return;
        }
        currentPtr--;
    }
    //returns the last element of the list
    T back() {
        return arr[currentPtr];
    }
    //overloading the subscript operator to access elements
    T& operator[](int i) {
        return arr[i];
    }
    //insert an element at the desired position
    void insert(T data, int pos) {
        if ((pos > (currentPtr + 1)) || (pos < 0)) {
            return;
        }
        if (currentPtr == size - 1) {
            increaseSize();
        }
        currentPtr++;
        int i;
        for (i = currentPtr; i > pos; i--) {
            arr[i] = arr[i - 1];
        }
        arr[i] = data;
    }
    //returns the size of the list
    int getSize() {
        return currentPtr + 1;
    }
    //deletes the element from the specified position
    void deleteItem(int pos) {
        if ((pos > (currentPtr + 1)) || (pos < 0) || empty()) {
            return;
        }
        for (int i = pos; i < currentPtr; i++) {
            arr[i] = arr[i + 1];
        }
        currentPtr--;
    }
    //searches for the given value and returns the index
    int search(T value) {
        for (int i = 0; i <= currentPtr; i++) {
            if (value == arr[i]) return i;
        }
        return -1;
    }
   //overloading the ostream operator to print the list
    friend ostream& operator<<(ostream& out, ArrayBasedList& l) {
        for (int i = 0; i <= l.currentPtr; i++) {
            out << l.arr[i] << " ";
        }
        out << endl;
        return out;
    }
   //overloading the subscript operator
    T operator[](int index) const {
        return arr[index];
    }
   //destructor
    ~ArrayBasedList() {
        delete[] arr;
    }
};

#endif // !ARRAYBASEDLIST_H

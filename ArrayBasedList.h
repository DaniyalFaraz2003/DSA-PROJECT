#pragma once
#ifndef ARRAYBASEDLIST_H
#define ARRAYBASEDLIST_H


#include <iostream>
using namespace std;

template<class T>
class ArrayBasedList {
private:
    int size;
    int currentPtr;

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

    ArrayBasedList<T>() {
        size = 5;
        currentPtr = -1;
        arr = new T[size];
    }
    ArrayBasedList<T>(const ArrayBasedList<T>& other) {
        this->size = other.size;
        this->currentPtr = other.currentPtr;
        this->arr = new T[size];
        for (int i = 0; i < getSize(); i++) {
            this->arr[i] = other.arr[i];
        }
    }

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

    bool empty() {
        return currentPtr == -1;
    }

    void push(T data) {
        if (currentPtr == size - 1) {
            increaseSize();
        }
        currentPtr++;
        arr[currentPtr] = data;
    }

    void clear() {
        currentPtr = -1;
    }

    void popFront() {
        deleteItem(0);
    }

    void pushFront(T val) {
        insert(val, 0);
    }

    T front() {
        if (!empty()) return arr[0];
        return T();
    }

    void pop() {
        if (empty()) {
            return;
        }
        currentPtr--;
    }

    T back() {
        return arr[currentPtr];
    }

    T& operator[](int i) {
        return arr[i];
    }

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

    int getSize() {
        return currentPtr + 1;
    }

    void deleteItem(int pos) {
        if ((pos > (currentPtr + 1)) || (pos < 0) || empty()) {
            return;
        }
        for (int i = pos; i < currentPtr; i++) {
            arr[i] = arr[i + 1];
        }
        currentPtr--;
    }

    int search(T value) {
        for (int i = 0; i <= currentPtr; i++) {
            if (value == arr[i]) return i;
        }
        return -1;
    }

    friend ostream& operator<<(ostream& out, ArrayBasedList& l) {
        for (int i = 0; i <= l.currentPtr; i++) {
            out << l.arr[i] << " ";
        }
        out << endl;
        return out;
    }

    T operator[](int index) const {
        return arr[index];
    }

    ~ArrayBasedList() {
        delete[] arr;
    }
};

#endif // !ARRAYBASEDLIST_H
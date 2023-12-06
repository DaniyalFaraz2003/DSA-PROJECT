#pragma once
#include <iostream>
#include "BigInt.h"
using namespace std;

BIG_INT power(BIG_INT num, const int power) {
	BIG_INT temp = num;
	for (int i = 0; i < power - 1; i++) {
		num = num * temp;
	}
	return num;
}

#include <iostream>
#include <string>
#include "sha1.hpp"
#include "BTree.h"
#include "CIrcularLL.h"
#include "DoublyLL.h"
#include "SinglyLL.h"
#include "DHT.h"
#include "BigInt.h"
#include "Utils.h"
using namespace std;


//int main() {
//    BTree<int> B(3);
//
//    for (int i = 1; i <= 23; i += 1) {
//        B.insert(i);
//    }
//
//    /*for (int i : {54, 365, 20, 38, 56, 26, 52, 47, 90, 48, 25, 74, 35, 37}) {
//        B.insert(i);
//    }
//    for (int i : {100, 302, 309, 373, 18, 374}) {
//        B.insert(i);
//    }*/
//
//    //B.deleteNode(9);
//    visualizeTree(generateDotCode(B.getRoot()));
//    return 0;
//}

//int main() {
//	CircularLinkedList<int> cl;
//	for (int i = 0; i < 5; i++) {
//		cl.push(i);
//	}
//	CircleListNode<int>* current = cl.head;
//	for (int i = 0; i < 10; i++) {
//		cout << current->data << ' ';
//		current = current->next;
//	}
//	
//
//	cout << endl;
//	const string input1 = "abc";
//	const string input2 = "daniyal is a good programmer";
//
//	SHA1 checksum;
//	checksum.update(input1);
//	const string hash = checksum.final();
//
//	SHA1 checksum2;
//	checksum2.update(input2);
//	const string hash2 = checksum2.final();
//
//	cout << "The SHA-1 of \"" << input1 << "\" is: " << hash << endl;
//	cout << "The SHA-1 of \"" << input2 << "\" is: " << hash2 << endl;
//	cout << hash.length() << endl;;
//	cout << SHA1::from_file("test.txt") << " *" << "test.txt" << endl;
//	return 0;
//}

int main() {
	/*DHT sys(5);

	sys.makeRoutingTables();
	cout << sys.ring;

	auto ptr = sys.ring.head;
	while (!(ptr->data.getId() == BIG_INT("19"))) ptr = ptr->next;
	ptr->data.printRoutingTable();*/

	Machine m(BIG_INT("12784"));
	for (int i : {1, 43, 5, 6, 23, 7, 87, 8, 4, 3, 50, 8}) {
		m.addFile(BIG_INT(to_string(i)), "daniyal");
	}
	m.removeFile(BIG_INT("8"), "daniyal");
	auto res = m.searchFile(BIG_INT("8"), "daniyal");
	cout << res.first->keys[res.second].value << endl;
	visualizeTree(generateDotCode(m.getBtreeHead()));
	return 0;
}
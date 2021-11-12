#include "avl_tester.h"
#include <iostream>
#include <fstream>
using namespace std;

int main() {

	AvlTester tester = AvlTester();
	tester.readInput();
/*	ofstream fout;
	SearchTree* a = new SearchTree();
	a->insert(75);
	a->insert(85);
	a->insert(95);
	a->insert(105);
	a->insert(55);
	a->insert(65);
	a->insert(66);
	a->insert(45);
	a->insert(25);
	a->insert(70);
	a->insert(77);
	a->insert(76);
	SearchTree::Iterator it = a->begin();

	for (int i = 0; i < a->size(); i++)
	{
		if (i != 0) ++it;
		cout << *it << endl;
		
	}
	a->printDFSWithHeights(fout);
		*/
		
	

//	for (SearchTree::Iterator it = a->begin(); it != a->end(); ++it) {
//		cout << *it << " ";
//		cout << endl;
//	}


	return 0;
}
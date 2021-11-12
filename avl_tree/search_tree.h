/*****************************************
* Shahrood User ID:  -
* Submitted for Data Structure Course
* Department of Computer Engineering and IT
* Shahrood University of Technology
*
* By submitting this file, I affirm that
* I am the author of all modifications to
* the provided code.
*****************************************/

#ifndef SEARCH_TREE_H
#define SEARCH_TREE_H

#include<iostream>
#include<algorithm>
#include<assert.h>
#include<fstream>
#include "exception.h"
#include "stack.h"
using namespace std;

class SearchTree {
private:
	class Node {
	public:
		int nodeValue;
		int treeHeight;

		// The left and right sub-trees
		Node* leftTree;
		Node* rightTree;
		Node* parent;

		// Hint as to how you can create your iterator
		// Point to the previous and next nodes in linear order
		Node* previousNode;
		Node* nextNode;

		// Member functions
		Node(int const& , Node*);

		void updateHeight();

		int height() const;
		bool isLeaf() const;
		Node* front();
		Node* back();
		Node* find(int const&);

		void clear();
		// YOU CAN change the return type if these two functions if you want to!
		bool insert(int const&, Node*& toThis , Node* &root);
		bool erase(int const&, Node*& toThis , Node* &root);
		void balance(int const& , Node* &root);
		void simpleRotationL( Node* middle , Node* down , Node* &root);
		void doubleRotationL( Node* middle, Node* down , Node* &root);
		void simpleRotationR(Node* middle, Node* down ,Node* &root);
		void doubleRotationR(Node* middle, Node* down ,Node* &root);
	};

	Node* rootNode;
	int treeSize;

	// Hint as to how to start your linked list of the nodes in order 
	Node* frontSentinel;
	Node* backSentinel;

public:
	class Iterator {
	private:
		SearchTree* containingTree;
		Node* currentNode;
		Stack<Node*>* inorder = new Stack<Node*>;
		bool isEnd;

		// The constructor is private so that only the search tree can create an iterator
		Iterator(SearchTree* tree, Node* startingNode);

	public:
		// DO NOT CHANGE THE SIGNATURES FOR ANY OF THESE
		int operator*() const;
		Iterator& operator++();
		Iterator& operator--();
		bool operator==(Iterator const& rhs) const;
		bool operator!=(Iterator const& rhs) const;

		// Make the search tree a friend so that it can call the constructor
		friend class SearchTree;
	};

	// DO NOT CHANGE THE SIGNATURES FOR ANY OF THESE
	SearchTree();
	~SearchTree();

	bool empty() const;
	int size() const;
	int height() const;
	void printDFSWithHeights(ofstream&) const;

	int front() const;
	int back() const;

	Iterator begin();
	Iterator end();
	Iterator rbegin();
	Iterator rend();
	Iterator find(int const&);

	void clear();
	bool insert(int const&);
	bool erase(int const&);

};

//////////////////////////////////////////////////////////////////////
//                Search Tree Public Member Functions               //
//////////////////////////////////////////////////////////////////////

// The initialization of the front and back sentinels is a hint
SearchTree::SearchTree() :
	rootNode(nullptr),
	treeSize(0),
	frontSentinel(new SearchTree::Node(0,nullptr)),
	backSentinel(new SearchTree::Node(0,nullptr)) {
	frontSentinel->nextNode = backSentinel;
	backSentinel->previousNode = frontSentinel;
}

SearchTree::~SearchTree() {
	clear();
}

bool SearchTree::empty() const {
	return (rootNode == nullptr);
}

int SearchTree::size() const {
	return treeSize;
}

int SearchTree::height() const {
	return rootNode->height();
}

void SearchTree::printDFSWithHeights(ofstream& out) const {
	out << "START->";
	if (!empty()) {
		Stack<Node*> dfs;
		Node* current = rootNode;
		dfs.push(current);
		while (!dfs.isEmpty()) {
			current = dfs.top();
			out << "[" << current->nodeValue << ", " << current->height() << "]->";
			dfs.pop();
			if (current->rightTree != nullptr) dfs.push(current->rightTree);
			if (current->leftTree != nullptr) dfs.push(current->leftTree);

		}
	}

	out << "END" << endl;
	// print each node value and height like this
    // START->[value, height]->[value, height]->END
}

 int SearchTree::front() const {
	if (empty()) {
		throw Underflow();
	}
	return rootNode->front()->nodeValue;
}

int SearchTree::back() const {
	if (empty()) {
		throw Underflow();
	}

	return rootNode->back()->nodeValue;
}

 SearchTree::Iterator SearchTree::begin() {
	return empty() ? Iterator(this, backSentinel) : Iterator(this, rootNode);
}

SearchTree::Iterator SearchTree::end() {
	return Iterator(this, backSentinel);
}

SearchTree::Iterator SearchTree::rbegin() {
	return empty() ? Iterator(this, frontSentinel) : Iterator(this, rootNode->back());
}

SearchTree::Iterator SearchTree::rend() {
	return Iterator(this, frontSentinel);
} 

SearchTree::Iterator SearchTree::find(int const& val) {
	if (empty()) {
		return Iterator(this, backSentinel);
	}

	SearchTree::Node* searchResult = rootNode->find(val);

	if (searchResult == nullptr) {
		return Iterator(this, backSentinel);
	}
	else {
		return Iterator(this, searchResult);
	}
}

void SearchTree::clear() {
	if (!empty()) {
		rootNode->clear();
		rootNode = nullptr;
		treeSize = 0;
	}

	// Reinitialize the sentinels
	frontSentinel->nextNode = backSentinel;
	backSentinel->previousNode = frontSentinel;
}

bool SearchTree::insert(int const& val) {
	if (empty()) {
		rootNode = new SearchTree::Node(val,nullptr);
		treeSize = 1;

		return true;
	}
	else if (rootNode->insert(val, rootNode , rootNode)) {
		++treeSize;
		return true;
	}
	else {
		return false;
	}
}

bool SearchTree::erase(int const& val) {
	if (!empty() && rootNode->erase(val, rootNode , rootNode)) {
		--treeSize;
		return true;
	}
	else {
		return false;
	}
}

//////////////////////////////////////////////////////////////////////
//                   Node Public Member Functions                   //
//////////////////////////////////////////////////////////////////////

SearchTree::Node::Node(int const& val , Node* parent) :
	nodeValue(val),
	leftTree(nullptr),
	rightTree(nullptr),
	nextNode(nullptr),
	previousNode(nullptr),
	treeHeight(0),
	parent(parent)
{
	// does nothing
}

void SearchTree::Node::updateHeight() {
	treeHeight = max(leftTree->height(), rightTree->height()) + 1;
}

int SearchTree::Node::height() const {
	return (this == nullptr) ? -1 : treeHeight;

}

// Return true if the current node is a leaf node, false otherwise
bool SearchTree::Node::isLeaf() const {
	return ((leftTree == nullptr) && (rightTree == nullptr));
}

// Return a pointer to the front node
SearchTree::Node* SearchTree::Node::front() {
	return (leftTree == nullptr) ? this : leftTree->front();
}

// Return a pointer to the back node
SearchTree::Node* SearchTree::Node::back() {
	return (rightTree == nullptr) ? this : rightTree->back();
}

SearchTree::Node* SearchTree::Node::find(int const& val) {
	if (val == nodeValue) {
		return this;
	}
	else if (val < nodeValue) {
		return (leftTree == nullptr) ? nullptr : leftTree->find(val);
	}
	else {
		return (rightTree == nullptr) ? nullptr : rightTree->find(val);
	}
}

// Recursively clear the tree
void SearchTree::Node::clear() {
	if (leftTree != nullptr) {
		leftTree->clear();
	}

	if (rightTree != nullptr) {
		rightTree->clear();
	}

	delete this;
}

bool SearchTree::Node::insert(int const& val, SearchTree::Node*& toThis , Node* &root) {
	if (val < nodeValue) {
		if (leftTree == nullptr) {
			leftTree = new SearchTree::Node(val,this);
			updateHeight();
		    balance(val,root);
			return true;
		}
		else {
			if (leftTree->insert(val, leftTree,root)) {
				updateHeight();
				balance(val,root);
				return true;
			}
			else {
				return false;
			}
		}
	}
	else if (val > nodeValue) {
		if (rightTree == nullptr) {
			rightTree = new SearchTree::Node(val,this);
			updateHeight();
			balance(val,root);
			return true;
		}
		else {
			if (rightTree->insert(val, rightTree , root)) {
				updateHeight();
				balance(val,root);
				return true;
			}
			else {
				return false;
			}
		}
	}
	else {
		return false;
	}
}

bool SearchTree::Node::erase(int const& val, SearchTree::Node*& toThis ,Node* &root) {
	if (val < nodeValue) {
		if (leftTree == nullptr) {
			return false;
		}
		else {
			if (leftTree->erase(val, leftTree , root)) {
				updateHeight();
				balance(val, root);
				return true;
			}

			return false;
		}
	}
	else if (val > nodeValue) {
		if (rightTree == nullptr) {
			return false;
		}
		else {
			if (rightTree->erase(val, rightTree , root)) {
				updateHeight();
				balance(val, root);
				return true;
			}

			return false;
		}
	}
	else {
		assert(val == nodeValue);

		if (isLeaf()) {
			toThis = nullptr;
			delete this;
		}
		else if (leftTree == nullptr) {
			toThis = rightTree;
			delete this;
		}
		else if (rightTree == nullptr) {
			toThis = leftTree;
			delete this;
		}
		else {
			nodeValue = rightTree->front()->nodeValue;
			rightTree->erase(nodeValue, rightTree , root);
			updateHeight();
			balance(val, root);
		}

		return true;
	}
}

void SearchTree::Node::balance(int const& val , Node* &root)
{
	if (abs(rightTree->height() - leftTree->height()) > 1) {
		Node* up = this;
		Node* middle = nullptr;
		Node* down = nullptr;
		

		if (rightTree->height() - leftTree->height() > 1) {
			middle = up->rightTree;
//			val < middle->nodeValue ? doubleRotationR(middle, middle->leftTree , root) : simpleRotationR(middle, middle->rightTree , root);
			middle->rightTree->height() > middle->leftTree->height() ? simpleRotationR(middle, middle->rightTree, root) : doubleRotationR(middle, middle->leftTree, root);


		}
		else if (leftTree->height() - rightTree->height() > 1) {
			middle = up->leftTree;
//			val < middle->nodeValue ? simpleRotationL(middle,middle->leftTree , root) : doubleRotationL(middle , middle->rightTree , root);
			middle->rightTree->height() < middle->leftTree->height() ? simpleRotationL(middle, middle->leftTree, root) : doubleRotationL(middle, middle->rightTree, root);
		
		}



		updateHeight();
		root->parent = nullptr;
	}
}

void SearchTree::Node::simpleRotationL( Node* middle, Node* down , Node* &root)
{
	if (this->parent != nullptr) this == this->parent->leftTree ?  this->parent->leftTree = middle : this->parent->rightTree = middle ;
	else root = middle;
	middle->parent = this->parent;
	this->parent = middle;
	Node* temp = middle->rightTree;
	if(middle->rightTree != nullptr)middle->rightTree->parent = this;
	middle->rightTree = this;
	this->leftTree = temp;
	this->updateHeight();
	middle->updateHeight();
}

void SearchTree::Node::doubleRotationL(Node* middle, Node* down , Node* &root)
{
	middle->rightTree = down->leftTree;
	down->parent = this;
	middle->parent = down;
	down->leftTree = middle;
	std::swap(down, middle);
	this->leftTree = middle;
	if(down->rightTree != nullptr)down->rightTree->parent = down;
	down->updateHeight();
	middle->updateHeight();
	simpleRotationL(middle, down,root);

}

void SearchTree::Node::simpleRotationR(Node* middle, Node* down , Node* &root)
{
	if (this->parent != nullptr) this == this->parent->leftTree ? this->parent->leftTree = middle : this->parent->rightTree = middle;
	else root = middle;
	middle->parent = this->parent;
	this->parent = middle;
	Node* temp = middle->leftTree;
	if(middle->leftTree !=nullptr)middle->leftTree->parent = this;
	middle->leftTree = this;
	this->rightTree = temp;
	this->updateHeight();
	middle->updateHeight();
}

void SearchTree::Node::doubleRotationR(Node* middle, Node* down , Node* &root)
{
	middle->leftTree = down->rightTree;
	down->parent = this;
	middle->parent = down;
	down->rightTree = middle;
	std::swap(down, middle);
	this->rightTree = middle;
	if(down->leftTree != nullptr)down->leftTree->parent = down;
	down->updateHeight();
	middle->updateHeight();
	simpleRotationR(middle, down , root);
}


//////////////////////////////////////////////////////////////////////
//                   Iterator Private Constructor                   //
//////////////////////////////////////////////////////////////////////

SearchTree::Iterator::Iterator(SearchTree* tree, SearchTree::Node* startingNode) :
	containingTree(tree),
	currentNode(startingNode) {
	// This is done for you...
	//// Does nothing...  nope it does sth important!
	while (currentNode != nullptr)
	{
		inorder->push(currentNode);
		currentNode = currentNode->leftTree;
	}
	currentNode = inorder->top();

}

//////////////////////////////////////////////////////////////////////
//                 Iterator Public Member Functions                 //
//////////////////////////////////////////////////////////////////////

int SearchTree::Iterator::operator*() const {
	// This is done for you...
	 
	return currentNode->nodeValue;
}

SearchTree::Iterator& SearchTree::Iterator::operator++() {
	// Update the current node to the node containing the next higher value
	// If we are already at end do nothing
	if (inorder->isEmpty()) { isEnd = true; 
	return *this;
	}
	if ( currentNode->rightTree == nullptr) inorder->pop();
	if (currentNode->rightTree != nullptr) {
		inorder->pop();
		inorder->push(currentNode->rightTree);
		currentNode = currentNode->rightTree;
		while (currentNode->leftTree != nullptr) {
			inorder->push(currentNode->leftTree);
			currentNode = currentNode->leftTree;
		}
	}

	currentNode = inorder->top();
	// Your implementation here, do not change the return value

	return *this;
}

SearchTree::Iterator& SearchTree::Iterator::operator--() {
	// Update the current node to the node containing the next smaller value
	// If we are already at either rend, do nothing

	if (currentNode->previousNode != nullptr) currentNode = currentNode->previousNode;

	// Your implementation here, do not change the return value

	return *this;
}

bool SearchTree::Iterator::operator==(SearchTree::Iterator const& rhs) const {
	// This is done for you...
	return (currentNode == rhs.currentNode);
}

bool SearchTree::Iterator::operator!=(SearchTree::Iterator const& rhs) const {
	// This is done for you...
	return (currentNode != rhs.currentNode);
}

#endif


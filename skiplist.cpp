// skiplist.cpp
// Implementations for SkipList class
// Author: Juan Arias
//
// SkipList class stores non-duplicate integers and can search for an item
// in O(logn) time. A SkipList can have multiple levels. SkipList of depth 1
// is similar to a doubly linked list. All elements are inserted at level 0,
// then has a 50% chance to be added to the next level and so forth. The
// interface of a SkipList includes:
//	  -adding an item
//	  -removing an item
//	  -checking for an item
//	  -reassinging to another SkipList

#include <cstdlib>
#include <ctime>
#include "skiplist.h"

// Friend function
// Overloaded output stream operator
// Displays all elements of SkipList
template <class T>
std::ostream& operator<<(std::ostream& out, const SkipList<T>& list) {

	for (int i(list.depth - 1); i >= 0; --i) {

		out << "Level: " << i <<  " -- ";

		auto* curr = list.frontGuards[i];

		while (curr != nullptr) {
		
			out << *(curr->itemPtr) << ", ";

			curr = curr->next;
		}

		out << std::endl;
	}

	return out;
}

// Constructs SkipList with depth as parameter levels,
// default SkipList has depth of 1, just one doubly-linked list
// Initializes frontGaurds & rearGuards to SNode* arrays,
// then initializes with helper method and sets time seed for PRNG
template <class T>
SkipList<T>::SkipList(const T& minItem, const T& maxItem, int levels)
	
	:MIN_PTR(new T), MAX_PTR(new T), depth(levels), /*WHY NOT WORKING?*/
	 frontGuards(new SNode*[depth]), rearGuards(new SNode*[depth]) {

	*(MIN_PTR) = minItem;
	*(MAX_PTR) = minItem;

	initialize();
	srand(time(NULL));
}

// Copy constructor
// initializes frontGuards & rearGuards to nullptr,
// then uses assignment operator overload
template <class T>
SkipList<T>::SkipList(const SkipList<T>& other) :frontGuards(nullptr),
												  rearGuards(nullptr) {
	*this = other;
}

// Destroys SkipList
// Calls helper method clear
template <class T>
SkipList<T>::~SkipList() {
	clear();
}

// Adds parameter item to SkipList, returns true if successful,
// false otherwise
// Duplicates not allowed
template <class T>
bool SkipList<T>::Add(const T& item) {

	SNode* curr = getFirst(item);

	if (*(curr->next->itemPtr) == item) {
		
		return false;
	}

	SNode* newPtr = new SNode(item);

	addAfter(curr, newPtr);

	while (alsoHigher()) {
		
		curr = getAbove(curr, item);

		if (curr != nullptr) {

			SNode* newUpper = new SNode(item);

			addAfter(curr, newUpper);
			addAbove(newPtr, newUpper);

			newPtr = newUpper;

			continue;
		} 

		break;
	}

	return true;
}

// Removes parameter item from SkipList, returns true if successful,
// false otherwise
template <class T>
bool SkipList<T>::Remove(const T& item) {

	SNode* curr = getFirst(item)->next;

	if (*(curr->itemPtr) != item) {
		
		return false;
	}

	do {

		SNode* nextPtr = curr->next;
		SNode* prevPtr = curr->prev;

		nextPtr->prev = prevPtr;
		prevPtr->next = nextPtr;

		SNode* temp = curr;

		curr = curr->downLevel;

		delete temp;

	}  while (curr != nullptr);

	return true;
}

// Returns true if parameter item in SkipList,
// false otherwise
template <class T>
bool SkipList<T>::Contains(const T& item) const {
	
	return *(getFirst(item)->next->itemPtr) == item;
}

// Assignment operator overload,
// copies data of parameter other
// Checks for self assignment before deallocating dynamic memory
template <class T>
SkipList<T>& SkipList<T>::operator=(const SkipList<T>& other) {

	if (this != &other) {

		clear();

		*(MIN_PTR) = *(other.MIN_PTR);
		*(MAX_PTR) = *(other.MAX_PTR);

		depth = other.depth;

		frontGuards = new SNode*[depth];
		 rearGuards = new SNode*[depth];

		initialize();

		copyAcross(other);
	}

	return *this;
}

// Constructs SNode with data as parameter userData
// Initializes all pointers to nullptr
template <class T>
SkipList<T>::SNode::SNode(const T& item)
	
	:itemPtr(new T(item)), next(nullptr), prev(nullptr), upLevel(nullptr),
	 downLevel(nullptr) {}


// Constructs and connects all frontGuards and rearGuards
template <class T>
void SkipList<T>::initialize() const {

	SNode* frontCurr = nullptr;
	SNode*  rearCurr = nullptr;

	for (int i(0); i < depth; ++i) {

		frontGuards[i] = new SNode(*(MIN_PTR));
		 rearGuards[i] = new SNode(*(MAX_PTR));

		frontGuards[i]->next =  rearGuards[i];
		 rearGuards[i]->prev = frontGuards[i];

		 addAbove(frontCurr, frontGuards[i]);
		 addAbove(rearCurr,   rearGuards[i]);

		 frontCurr = frontGuards[i];
		  rearCurr =  rearGuards[i];
	}
}

// Deallocates all dynamic memory
template <class T>
void SkipList<T>::clear() const {
	
	if (frontGuards != nullptr && rearGuards != nullptr) {
	
		for (int i(0); i < depth; ++i) {
			
			SNode* curr = frontGuards[i];
			SNode* temp = curr;

			while (curr != nullptr) {

				curr = curr->next;

				delete temp;

				temp = curr;
			}
		}

		delete[] frontGuards;
		delete[] rearGuards;
	}
}

// Places parameter newPtr in left-right chain after paremeter curr
template <class T>
void SkipList<T>::addAfter(SNode* curr, SNode* newPtr) const {

	SNode* nextPtr = curr->next;

	curr->next = newPtr;
	newPtr->next = nextPtr;
	nextPtr->prev = newPtr;
	newPtr->prev = curr;
}

// Places parameter upPtr in up-down chain above parameter curr
template <class T>
void SkipList<T>::addAbove(SNode* curr, SNode* upPtr) const {

	if (curr != nullptr) {

		curr->upLevel = upPtr;
		upPtr->downLevel = curr;
	}
}


// Returns true 50% of the time,
// each SNode has a 50% chance of being at a higher level
template <class T>
bool SkipList<T>::alsoHigher() const {

	return rand() % 2 == 1;
}

// Returns a pointer to first SNode left of the SNode that either
// contains parameter item or
// contains the first number greater than item
template <class T>
auto* SkipList<T>::getFirst(const T& item) const {

	SNode* curr = goRight(frontGuards[depth - 1], item);

	while (curr->downLevel != nullptr && *(curr->next->itemPtr) != item) {

		curr = goRight(curr->downLevel, item);
	}

	return curr;
}

// Returns a pointer to SNode on level above parameter curr & 
// left of the SNode that contains the first number greater
// than parameter item, or nullptr if there are no higher levels
template <class T>
auto* SkipList<T>::getAbove(SNode* curr, const T& item) const {

	curr = goLeft(curr);

	return goRight(curr->upLevel, item);
}

// Returns a pointer to SNode right of parameter curr
// & left of the SNode that contains
// the first number greater than parameter item 
template <class T>
auto* SkipList<T>::goRight(SNode* curr, const T& item) const {

	while (curr != nullptr && *(curr->next->itemPtr) < item) {

		curr = curr->next;
	}

	return curr;
}

// Returns a pointer to first SNode left of
// parameter curr that has an upLevel SNode
template <class T>
auto* SkipList<T>::goLeft(SNode* curr) const {

	while (curr->upLevel == nullptr && curr->prev != nullptr) {
		
		curr = curr->prev;
	}

	return curr;
}

// Copies all non-guard SNodes from parameter other
template <class T>
void SkipList<T>::copyAcross(const SkipList<T>& other) const {

	SNode* curr = frontGuards[0];
	SNode* otherCurr = other.frontGuards[0]->next;

	while (*(otherCurr->itemPtr) != *(other.MAX_PTR)) { //change for template

		SNode* newPtr = new SNode(*(otherCurr->itemPtr));

		addAfter(curr, newPtr);

		copyAbove(newPtr, otherCurr->upLevel);

		otherCurr = otherCurr->next;
		curr = newPtr;
	}
}

// Copies all levels of parameter otherCurr's
// SNode above parameter curr
template <class T>
void SkipList<T>::copyAbove(SNode* curr, SNode* otherCurr) const {

	while (otherCurr != nullptr) {

		SNode* newUpper = new SNode(*(otherCurr->itemPtr));
		SNode* nextUpper = getAbove(curr, otherCurr->data);

		addAfter(nextUpper, newUpper);

		addAbove(curr, newUpper);

		otherCurr = otherCurr->upLevel;
		curr = newUpper;
	}
}

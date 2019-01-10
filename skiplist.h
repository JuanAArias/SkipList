// skiplist.h
// Specifications for SkipList class
// Authors: Yusuf Pisan & Juan Arias
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

#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <iostream>

template <class T>
class SkipList {

	// Friend function
	// Overloaded output stream operator
	// Displays all elements of SkipList
	friend std::ostream& operator<<(std::ostream& out, const SkipList<T>& list);

public:

	// Constructs SkipList with depth as parameter levels,
	// default SkipList has depth of 1, just one doubly-linked list
	explicit SkipList(const T& minItem, const T& maxItem, int levels = 1);

	// Copy constructor
	SkipList(const SkipList<T>& other);

	// Destroys SkipList
	virtual ~SkipList();

	// Adds parameter item to SkipList, returns true if successful,
	// false otherwise
	// Duplicates not allowed
	bool Add(const T& item);

	// Removes parameter item from SkipList, returns true if successful,
	// false otherwise
	bool Remove(const T& item);

	// Returns true if parameter item in SkipList,
	// false otherwise
	bool Contains(const T& item) const;

	// Assignment operator overload,
	// copies data of parameter other
	SkipList& operator=(const SkipList<T>& other);

private:

	// Private nodes of SkipList
	struct SNode {

		// Constructs SNode with data as parameter userData
		explicit SNode(const T& item);

		// Data for SNode
		T* itemPtr;

		// Link to next SNode
		SNode* next;

		// Link to prev SNode
		SNode* prev;

		// Link to up one level
		SNode* upLevel;

		// Link to down one level
		SNode* downLevel;

	};

	// Pointer to max item for rearGuards
	T* MIN_PTR, MAX_PTR;

	// Depth of Skiplist, levels are 0 to depth - 1
	int depth;

	// Array of depth SNode* objects as frontGuards linking levels
	SNode** frontGuards;

	// Array of depth SNode* objects as rearGuards linking levels
	SNode** rearGuards;

	// Constructs and connects all frontGuards and rearGuards
	void initialize() const;

	// Deallocates all dynamic memory
	void clear() const;

	// Places parameter newPtr in left-right chain after paremeter curr
	void addAfter(SNode* curr, SNode* newPtr) const;

	// Places parameter upPtr in up-down chain above parameter curr
	void addAbove(SNode* curr, SNode* upPtr) const;

	// Returns true 50% of the time,
	// each SNode has a 50% chance of being at a higher level
	bool alsoHigher() const;

	// Returns a pointer to first SNode left of the SNode that either
	// contains parameter item or
	// contains the first number greater than item
	auto* getFirst(const T& item) const;

	// Returns a pointer to SNode on level above parameter curr & 
	// left of the SNode that contains the first number greater
	// than parameter item, or nullptr if there are no higher levels
	auto* getAbove(SNode* curr, const T& item) const;

	// Returns a pointer to SNode right of parameter curr
	// & left of the SNode that contains
	// the first number greater than parameter item 
	auto* goRight(SNode* curr, const T& item) const;

	// Returns a pointer to first SNode left of
	// parameter curr that has an upLevel SNode
	auto* goLeft(SNode* curr) const;

	// Copies all non-guard SNodes from parameter other
	void copyAcross(const SkipList<T>& other) const;

	// Copies all levels of parameter otherCurr's
	// SNode above parameter curr
	void copyAbove(SNode* curr, SNode* otherCurr) const;

};
#include "skiplist.cpp"
#endif

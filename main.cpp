// int-tests.cpp
// Test functions for SkipList class
// Authors: Yusuf Pisan & Juan Arias

#include <cstdlib>
#include <ctime>
#include <fstream>
#include "skiplist.h"

// Prompt constants
const int          QUIT = -666;
const char    PROMPT1[] = "\nEnter levels for SkipList: ";
const char    PROMPT2[] = "\nEnter number to add (-666 to quit): ";
const char    PROMPT3[] = "\nEnter number to remove (-666 to quit): ";
const char TRANSITION[] = "\nCopying list1 to list2...";
const char   HEADING1[] = "------------------LIST1-----------------";
const char   HEADING2[] = "------------------LIST2-----------------";

// Tests constructors,
// uses user input to construct SkipList,
// tests copy constructor by returning by value,
SkipList<int> getList() {

	// Prompt user for levels to construct SkipList
	std::cout << PROMPT1;
	
	// Store user input in variable
	int userInput;
	std::cin >> userInput;
	std::cout << std::endl;

	// Return SkipList
	return SkipList<int>(INT_MIN, INT_MAX, userInput);
}

// Tests Add, Contains & operator<< overload,
// uses user input
void Test1(SkipList<int>* const listPtr) {

	// Prompt user for numbers to add
	std::cout << PROMPT2;

	// Store user input in variable
	int userInput;
	std::cin >> userInput;
	std::cout << std::endl;

	// Sentinel loop for user to continue adding items until they quit
	while (userInput != QUIT) {

		// Test Add, should print true for new numbers,
		// false for duplicates
		std::cout << std::boolalpha << "Add " << userInput << "?   \t" 
				  << listPtr->Add(userInput)  << std::endl;

		// Test Contains, should print true if number in SkipList,
		// false otherwise
		std::cout << std::boolalpha << "Contains " << userInput << "?\t"
								<< listPtr->Contains(userInput) << std::endl;

		// Test operator<< overload, should display all elements in SkipList
		std::cout << std::endl << (*listPtr) << std::endl;

		// Prompt user to continue adding or quit
		std::cout << PROMPT2;
		std::cin >> userInput;
		std::cout << std::endl;

	}
}

// Tests Remove, Contains & operator<< overload,
// uses user input
void Test2(SkipList<int>* const listPtr) {

	// Prompt user for numbers to remove
	std::cout << PROMPT3;

	// Store user input in variable
	int userInput;
	std::cin >> userInput;
	std::cout << std::endl;

	// Sentinel loop for user to continue removing items until they quit
	while (userInput != QUIT) {

		// Test Remove, should print true for items in SkipList,
		// false for items not in SkipList
		std::cout << std::boolalpha << "Remove " << userInput << "?   \t"
				  << listPtr->Remove(userInput)  << std::endl;

		// Test Contains, should print true if number in SkipList,
		// false otherwise
		std::cout << std::boolalpha << "Contains " << userInput << "?\t"
			      << listPtr->Contains(userInput)  << std::endl;

		// Test operator<< overload, should display all elements in SkipList
		std::cout << std::endl << (*listPtr) << std::endl;

		// Prompt user to continue removing or quit
		std::cout << PROMPT3;
		std::cin >> userInput;
		std::cout << std::endl;
	}
}

// Run all SkipList tests
void TestAll() {

	// Test copy constructor
	SkipList<int> list1(getList());

	// Run Test1 & Test2 with list1
	Test1(&list1);
	Test2(&list1);

	// Test assignment overload
	std::cout << TRANSITION << std::endl;
	SkipList<int> list2(0, 0, 0);
	list2 = list1;

	// Run Test1 & Test2 with list2
	Test1(&list2);
	Test2(&list2);

	// Check that list2 is not a shallow copy,
	// should display different elements
	std::cout << HEADING1 << std::endl;
	std::cout << list1 << std::endl;
	std::cout << HEADING2 << std::endl;
	std::cout << list2 << std::endl;

}

// Start SkipList tests
int main() {
	TestAll();
	return 0;
}

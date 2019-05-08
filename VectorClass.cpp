// VectorClass.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Vector.h"
#include <algorithm>


//predicate returns true if student pass
bool passedClass(const int& num)
{
	if (num > 5)
		return true;
	return false;
}



int main()
{
	std::cout << "test 1:\n";
	Vector<int> b{ 5 };
	std::cout << "b.size = " << b.size() << "\n";
	std::cout << "b.capacity = " << b.capacity() << "\n";
	//b.reserve(20);
	std::cout << "b.size = " << b.size() << "\n";
	std::cout << "b.capacity = " << b.capacity() << "\n";

	for (int i = 0; i < 10; i++)
	{
		b.push_back(i);
		std::cout << b[i] << "\n";
		std::cout << "b[" << i << "].size = " << b.size() << "\n";
		std::cout << "b[" << i << "].capacity = " << b.capacity() << "\n";
	}

	for (int i = 0; i < 10; i++)
	{
		std::cout << b[i] << " ";
	}
	std::cout << "\n";

	std::cout << "test 2:\n";
	
	Vector<int> c{ 10, 5 };
	for (int i = 0; i < 10; i++)
	{
		std::cout << c[i] << " ";
	}
	std::cout << "\n";

	std::cout << "test 3:\n";

	Vector<int> d{ c };
	//d = c;
	for (int i = 0; i < 10; i++)
	{
		std::cout << d[i] << " ";
	}
	std::cout << "\n";

	std::cout << "test 4:\n";

	int* iterator = d.begin();
	while (iterator != d.end())
	{
		std::cout << *iterator++ << " ";
	}
	std::cout << "\n";

	std::cout << b.at(2) << " " << b.at(7) << "\n";
	std::cout << b.front() << " " << b.back();

	std::cout << "\n";

	std::cout << "test 5:\n";
	//Vector<int>::iterator it = b.begin();
	Vector<int>::iterator it{ b.begin() };
	it = 4;
	std::cout << it << "\n";
	Vector<int>::iterator it2{ b[8] };
	it = it2;
	std::cout << it << "\n";

	Vector<int>::iterator it3;
	it3 = true;
	std::cout << "\n\n" << it3 << "\n";

	std::cout << "\nSWAP TEST:\n";
	std::cout << b << "\n";
	b.swap(&b[8], &b[11]);
	std::cout << b << "\n\n";

	b.swap(4, 9);
	std::cout << b << "\n\n";

	for (auto& it : b)
	{
		std::cout << &it << " ";
	}

	std::cout << "\nERASE TEST:\n";
	std::cout << b << "\n";
	b.erase(4);
	std::cout << b << "\n\n";


	std::cout << "\nMASS ERASE TEST:\n";
	std::cout << b << "\n";
	Vector<int>::iterator it4 = &b[5];
	b.erase(it4, b[8]);
	std::cout << b << "\n\n";

	std::cout << "\n\nINSERTION TEST:\n";
	std::cout << "\n" << b << "\n";
	b.insert(4, 15);
	std::cout << "\n" << b << "\n";
	b.insert(0, 333);
	std::cout << "\n" << b << "\n";
	b.insert(3, 8888);
	std::cout << "\n" << b << "\n";

	std::cout << "\nMASS ERASE TEST:\n";
	std::cout << b << "\n";
	Vector<int>::iterator it5 = &b[3];
	Vector<int>::iterator it6 = &b[5];
	b.erase(it5, it6);
	std::cout << b << "\n\n";

	std::cout << "\nSTABLE PARTITION TEST:\n";
	std::cout << b << "\n";
	Vector<int>::iterator it10 = std::stable_partition(b.begin(), b.end(), passedClass);
	Vector<int> not_passed{ it10, b.end() };
	b.erase(it10, b.end());
	std::cout << "PASSED: " << b << "\n";
	std::cout << "NOT PASSED: " << not_passed << "\n\n";

	std::cout << "\n\nsize of(b) = " << sizeof(b) + (sizeof(int) * b.size()) << "\n";
	b.clear();
	std::cout << b << "\n";
	std::cout << "size of(b) = " << sizeof(b) + (sizeof(int) * b.size()) << "\n";



}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

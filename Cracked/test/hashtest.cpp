#include "MyHash.h"
#include <iostream>
#include <string>
#include <cassert>

using namespace std;

int main() {
	MyHash<string, int> dict;
	assert(dict.getNumItems() == 0);
	dict.associate("Aaron", 21);
	assert(*dict.find("Aaron") == 21);
	assert(dict.getNumItems() == 1);
	assert(dict.find("Not Aaron") == nullptr);
	dict.associate("Not Aaron", 20);
	assert(*dict.find("Not Aaron") == 20);
	assert(dict.getNumItems() == 2);
	dict.associate("Aaron", 20);
	assert(*dict.find("Aaron") == 20);
	assert(dict.getNumItems() == 2);
	dict.reset();
	assert(dict.getNumItems() == 0);
	assert(dict.find("Not Aaron") == nullptr);
	string s = "";
	for (int i = 0; i < 1000; i++) {
		s += "#";
		dict.associate(s, i);
		assert(*dict.find(s) == i);
	}
	dict.reset();
	assert(dict.getNumItems() == 0);
	assert(dict.find("#") == nullptr);
	for (int i = 0; i < 1000; i++) {
		string s = "";
		for (int j = i; j > 0; j--)
			s += '#';
		dict.associate(s, i);
		assert(dict.getNumItems() == i + 1);
		assert(dict.getLoadFactor() <= .5);
		s = "";
		for (int k = 0; k <= i; k++) {
			assert(*dict.find(s) == k);
			s += '#';
		}
	}

	cout << "Passed all tests." << endl;
}
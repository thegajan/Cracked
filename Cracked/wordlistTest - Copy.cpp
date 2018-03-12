#include "provided.h"
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator>
using namespace std;

int main() {
	WordList wl;
	if (!wl.loadWordList("wordlist.txt"))
	{
		cout << "Unable to load word list" << endl;
		return 0;
	}
		assert(wl.contains("onomatopoeia"));
		assert(wl.contains("AaH"));
		assert(wl.contains("'em"));
		assert(!wl.contains("gajan"));
	
		string cipher = "xyqbbq";
		string decodedSoFar = "?r????";

		vector<string> v = wl.findCandidates(cipher, decodedSoFar);
		assert(!v.empty());
		for (size_t k = 0; k < v.size(); k++)
			assert(v[k] == "grotto" || v[k] == "troppo");

		decodedSoFar = "??????";
		v = wl.findCandidates(cipher, decodedSoFar);
		assert(!v.empty());
		string arr[] = { "apollo", "blotto", "career", "chilli", "djinni", "fusees", "grotto", "guerre", "indeed", "piazza", "pierre", "quagga", "steppe", "troppo" };
		for (size_t k = 0; k < v.size(); k++) {
			assert(find(begin(arr), end(arr), v[k]));
		}

		decodedSoFar = "??o??o";
		v = wl.findCandidates(cipher, decodedSoFar);
		assert(!v.empty());
		string arr2[] = { "apollo", "blotto","grotto", "troppo" };
		for (size_t k = 0; k < v.size(); k++) {
			assert(find(begin(arr2), end(arr2), v[k]));
		}

		decodedSoFar = "???mm?";
		v = wl.findCandidates(cipher, decodedSoFar);
		assert(v.empty());

		cout << "Passed all Test Cases" << endl;
}
#include "provided.h"
#include <string>
#include <vector>
//#include "MyHash.h"
#include <fstream>
using namespace std;

class WordListImpl
{
public:
	WordListImpl() {}
	bool loadWordList(string filename);
	bool contains(string word) const;
	vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
	//MyHash<int, string> m_table;
};

bool WordListImpl::loadWordList(string filename)
{
	//m_table.reset();
	ifstream file("wordlist.txt");
	string word;
	int i = 0;
	if (!file) {
		return false;
	}
	while (getline(file, word)) {
		//If a line of the word list file
		//	contains a character
		//	that is not a letter or an apostrophe,
		//	ignore that line; otherwise, that line is to be considered one of the words in the word list
		//m_table.associate(i, word);
		i++;
	}
	return true;
}

bool WordListImpl::contains(string word) const
{
	return false; // This compiles, but may not be correct
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
	return vector<string>();  // This compiles, but may not be correct
}

//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
	return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
	return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
	return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
	m_impl = new WordListImpl;
}

WordList::~WordList()
{
	delete m_impl;
}

bool WordList::loadWordList(string filename)
{
	return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
	return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
	return m_impl->findCandidates(cipherWord, currTranslation);
}

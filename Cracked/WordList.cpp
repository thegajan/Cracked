#include "provided.h"
#include <string>
#include <vector>
#include <fstream>
#include "MyHash.h"
using namespace std;

class WordListImpl
{
public:
	WordListImpl() {}
	bool loadWordList(string filename);
	bool contains(string word) const;
	vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
	bool validWord(string s) const;
	MyHash<string, string> m_table;
};

bool WordListImpl::validWord(string s) const{
	int length = s.length();
	for (int i = 0; i < length; i++) {
		if (!isalpha(s[i]) && s[i] != '\'')
			return false;
	}
	return true;
}

bool WordListImpl::loadWordList(string filename) //O(W)
{
	m_table.reset();
	ifstream file("wordlist.txt");
	string word;
	int i = 0;
	if (!file) {
		return false;
	}
	while (getline(file, word)) {
		if (validWord(word)) {
			m_table.associate(word, word);
			//i++;
		}
	}
	return true;
}

bool WordListImpl::contains(string word) const
{
	int length = word.length();
	for (int i = 0; i < length; i++) word[i] = tolower(word[i]);
	const string* p = m_table.find(word);
	if (p != nullptr)
		return true;
	return true;
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

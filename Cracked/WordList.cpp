#include "provided.h"
#include <string>
#include <vector>
#include <fstream>
#include "MyHash.h"
#include <algorithm>
using namespace std;

class WordListImpl
{
public:
	bool loadWordList(string filename);
	bool contains(string word) const;
	vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
	bool validWord(const string& s) const;
	string wordPattern(const string& word) const;
	MyHash<string, vector<string>> m_table;
};

string WordListImpl::wordPattern(const string& word) const{
	string s = word;
	for (int i = 0; i < s.size(); i++) {
		char rep = 'A';
		replace(s.begin(), s.end(), s[i], rep);
		rep++;
	}
	return s;
}

bool WordListImpl::validWord(const string& s) const {
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
			string codedWord = wordPattern(word);
			vector<string>* codedWords = m_table.find(codedWord);
			if (codedWords == nullptr)
				m_table.associate(codedWord, vector<string>{word});
			else {
				codedWords->push_back(word);
				m_table.associate(codedWord, *codedWords);
			}
			//i++;
		}
	}
	return true;
}

bool WordListImpl::contains(string word) const //O(1)
{
	int length = word.length();
	for (int i = 0; i < length; i++) word[i] = tolower(word[i]);
	const vector<string>* p = m_table.find(wordPattern(word));
	for(int j = 0; j < p->size(); j++)
		if ((*p)[j] == word)
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

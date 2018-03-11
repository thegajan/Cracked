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
	string lower(const string& s) const;
	bool validWord(const string& s) const;
	bool validTranslation(const string& translation, const string& word) const;
	string wordPattern(const string& word) const;
	MyHash<string, vector<string>> m_table;
};

string WordListImpl::lower(const string& s) const {
	int length = s.length();
	string word = s;
	for (int i = 0; i < length; i++) word[i] = tolower(word[i]);
	return word;
}

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

bool WordListImpl::validTranslation(const string& translation, const string& word) const {
	int length = translation.length();
	if (length != word.length())
		return false;
	for (int i = 0; i < length; i++) {
		if (!isalpha(translation[i]) && translation[i] != '\'' && translation[i] != '?')
			return false;
		else if (isalpha(translation[i]) && !isalpha(word[i]))
			return false;
		else if (translation[i] == '?' && !isalpha(word[i]))
			return false;
		else if (translation[i] == '\'' && word[i] != '\'')
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
	const vector<string>* p = m_table.find(wordPattern(lower(word)));
	for(int j = 0; j < p->size(); j++)
		if ((*p)[j] == word)
			return true;
	return true;
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const //O(Q)
{
	if (!validWord(cipherWord) || !validTranslation(currTranslation, cipherWord))
		return vector<string>();

	string cipherPattern = wordPattern(cipherWord);
	const vector<string>* possibleCipher = m_table.find(cipherPattern);

	vector<string> specificCipher;

	for (int i = 0; i < possibleCipher->size(); i++) {
		string possWord_O = (*possibleCipher)[i];
		string possWord = lower((*possibleCipher)[i]);
		bool valid = true;
		for (int j = 0; j < possWord.length(); j++) {
			if (isalpha(currTranslation[j]) && possWord[j] != currTranslation[j])
				valid = false;
			else if (currTranslation[j] == '?' && !isalpha(possWord[j]))
				valid = false;
			else if (currTranslation[j] == '\'' && possWord[j] != '\'')
				valid = false;
		}
		if (valid)
			specificCipher.push_back(possWord_O);
	}
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

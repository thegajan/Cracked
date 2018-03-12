#include "provided.h"
#include <string>
#include "MyHash.h"
using namespace std;

class TranslatorImpl
{
public:
	TranslatorImpl();
	~TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
	string upper(const string& s) const;
	char* newTable();
	char* top() const;
	bool validate(const string& ct, const string& pt) const;
	vector<char*> m_stack;
};

TranslatorImpl::TranslatorImpl() {
	newTable();
}

TranslatorImpl::~TranslatorImpl() {
	for (int i = 0; i < m_stack.size(); i++)
		m_stack.pop_back();
}

string TranslatorImpl::upper(const string& s) const {
	int length = s.length();
	string word = s;
	for (int i = 0; i < length; i++) word[i] = toupper(word[i]);
	return word;
}

bool TranslatorImpl::validate(const string& ct, const string& pt) const {
	int ctLength = ct.length();
	int ptLength = pt.length();
	if (ctLength != ptLength)
		return false;
	for (int i = 0; i < ctLength; i++) {
		if (!isalpha(ct[i]) || !isalpha(pt[i]))
			return false;
		char currMap = top()[ct[i]-65];
		if (currMap != pt[i] && currMap != '?')
			return false;
		for (int j = 0; j < 26; j++) {
			if (top()[j] == pt[i] && top()[j] != ct[i])
				return false;
		}
	}
	return true;
}

char* TranslatorImpl::top() const{
	return m_stack.back();
}

char* TranslatorImpl::newTable() {
	char* newTable = new char[26];
	for(int i = 0; i < 26; i++){
		newTable[i] = '?';
	}
	if (m_stack.size() > 0) {
		for (int i = 0; i < 26; i++) {
			newTable[i] = top()[i];
		}
	}
	m_stack.push_back(newTable);
	return newTable;
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
	ciphertext = upper(ciphertext);
	plaintext = upper(plaintext);
	if (!validate(ciphertext, plaintext))
		return false;
	char* newMap = newTable();
	for (int i = 0; i < ciphertext.length(); i++) {
		if (ciphertext[i] == '\'')
			continue;
		newMap[ciphertext[i] - 65] = plaintext[i];
	}
	return true;
}

bool TranslatorImpl::popMapping()
{
	if (m_stack.size() == 0)
		return false;
	m_stack.pop_back();
	return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
	string mapText = "";
	for (int i = 0; i < ciphertext.length(); i++) {
		if (!isalpha(ciphertext[i]))
			mapText += ciphertext[i];
		else 
			if(islower(ciphertext[i]))
				mapText += tolower(top()[toupper(ciphertext[i]) - 65]);
			else
				mapText += top()[toupper(ciphertext[i]) - 65];
	}
	return mapText;
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}

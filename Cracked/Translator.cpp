#include "provided.h"
#include <string>
#include "MyHash.h"
using namespace std;

class TranslatorImpl
{
public:
	TranslatorImpl();
	bool pushMapping(string ciphertext, string plaintext);
	bool popMapping();
	string getTranslation(const string& ciphertext) const;
private:
	string top() const { return m_stack.back(); };
	string upper(const string& s) const;
	void newTable();
	bool validate(string ct, string pt) const;
	vector<string> m_stack;
};

TranslatorImpl::TranslatorImpl() {
	newTable();
}

string TranslatorImpl::upper(const string& s) const {
	int length = s.length();
	string word = s;
	for (int i = 0; i < length; i++) word[i] = toupper(word[i]);
	return word;
}

bool TranslatorImpl::validate(string ct, string pt) const {
	int ctLength = ct.length();
	int ptLength = pt.length();
	ct = upper(ct);
	pt = upper(pt);
	if (ctLength != ptLength)
		return false;
	for (int i = 0; i < ctLength; i++) {
		if (!isalpha(ct[i]) || !isalpha(pt[i]))
			return false;
		char currMap = top()[ct[i] - 65];
		if (currMap != pt[i] && currMap != '?')
			return false;
		for (int j = 0; j < 26; j++) {
			if (top()[j] == pt[i] && j != ct[i] - 65)
				return false;
		}
	}
	return true;
}

void TranslatorImpl::newTable() {
	string s;
	if (m_stack.size() > 0) {
		s = top();
	}
	else {
		for (int i = 0; i < 26; i++)
			s += '?';
	}
	m_stack.push_back(s);
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
	ciphertext = upper(ciphertext);
	plaintext = upper(plaintext);
	if (!validate(ciphertext, plaintext))
		return false;
	newTable();
	string newMap = top();
	for (int i = 0; i < ciphertext.length(); i++) {
		if (ciphertext[i] == '\'')
			continue;
		newMap[ciphertext[i] - 65] = plaintext[i];
	}
	m_stack.back() = newMap;
	return true;
}

bool TranslatorImpl::popMapping()
{
	if (m_stack.size() == 1)
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
			if (islower(ciphertext[i]))
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

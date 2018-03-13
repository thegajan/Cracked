#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class DecrypterImpl
{
public:
	DecrypterImpl();
	bool load(string filename);
	vector<string> crack(const string& ciphertext);
private:
	int transStatus(const vector<string>& s) const;
	int numUnkown(const string& s) const;
	string chosen(const vector<string>& tokenList) const;
	void recCrack(string cipher_message, vector<string>& output);
	WordList m_word;
	Translator m_translation;
	Tokenizer m_token;
};

DecrypterImpl::DecrypterImpl()
	:m_token(" ,;:.!()[]{}-\"#$%^&")
{}

bool DecrypterImpl::load(string filename) //O(W)
{
	return m_word.loadWordList("wordlist.txt");
}

int DecrypterImpl::numUnkown(const string& s) const {
	int num = 0;
	string translation = m_translation.getTranslation(s);
	for (int i = 0; i < s.length(); i++)
		if (translation[i] == '?')
			num++;
	return num;
}

string DecrypterImpl::chosen(const vector<string>& tokenList) const{
	string chosen = tokenList[0];
	for (int i = 1; i < tokenList.size(); i++) {
		if (numUnkown(chosen) < numUnkown(tokenList[i]))
			chosen = tokenList[i];
	}
	return chosen;
}

int DecrypterImpl::transStatus(const vector<string>& s) const {
	int numDecoded = 0;
	for (int i = 0; i < s.size(); i++) {
		if (numUnkown(s[i]) != s[i].length())
			numDecoded++;
	}
	return numDecoded;
}

void DecrypterImpl::recCrack(string cipher_message, vector<string>& output) {
	vector<string> tokenized = m_token.tokenize(cipher_message);
	string chosenToken = chosen(tokenized);
	string chosenTranslation = m_translation.getTranslation(chosenToken);
	vector<string> matches = m_word.findCandidates(chosenToken, chosenTranslation);
	if (matches.empty())
		return;
	for (int i = 0; i < matches.size(); i++) {
		m_translation.pushMapping(chosenToken, matches[i]);
		vector<string> tempTranslation;
		for (int j = 0; j < tokenized.size(); j++) {
			tempTranslation.push_back(m_translation.getTranslation(tokenized[j]));
		}
		int status = transStatus(tempTranslation);
		if (status == 0) {
			m_translation.popMapping();
		}
		else if (status == tempTranslation.size()) {
			output.push_back(m_translation.getTranslation(cipher_message));
			m_translation.popMapping();
		}
		else
			recCrack(cipher_message, output);
	}
	m_translation.popMapping();
	return;
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
	vector<string> output;
	recCrack(ciphertext, output);
	return output; 
}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
	m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
	delete m_impl;
}

bool Decrypter::load(string filename)
{
	return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
	return m_impl->crack(ciphertext);
}

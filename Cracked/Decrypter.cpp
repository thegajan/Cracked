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
	return m_word.loadWordList(filename);
}

int DecrypterImpl::numUnkown(const string& s) const {
	int num = 0;
	for (int i = 0; i < s.length(); i++)
		if (s[i] == '?')
			num++;
	return num;
}

string DecrypterImpl::chosen(const vector<string>& tokenList) const{
	string chosen = tokenList[0];
	for (int i = 0; i < tokenList.size(); i++) {
		if (numUnkown(m_translation.getTranslation(chosen)) <= numUnkown(m_translation.getTranslation(tokenList[i])))
			chosen = tokenList[i];
	}
	return chosen;
}

void DecrypterImpl::recCrack(string cipher_message, vector<string>& output) {
	vector<string> tokenCipher = m_token.tokenize(cipher_message);
	string chosenCipherWord = chosen(tokenCipher);
	string chosenCipherWordTranslation = m_translation.getTranslation(chosenCipherWord);

	vector<string> cipherWordMatches = m_word.findCandidates(chosenCipherWord, chosenCipherWordTranslation);

	if (cipherWordMatches.empty()){
		m_translation.popMapping();
		return;
	}

	for (int i = 0; i < cipherWordMatches.size(); i++) {

		bool push = m_translation.pushMapping(chosenCipherWord, cipherWordMatches[i]);
		if (!push) {
			continue;
		}

		vector<string> translatedtokenCipher = m_token.tokenize(m_translation.getTranslation(cipher_message));

		int complete = 0;
		int incorrect = 0;
		for (int j = 0; j < translatedtokenCipher.size(); j++) {
			if (numUnkown(translatedtokenCipher[j]) == 0 && !m_word.contains(translatedtokenCipher[j]))
				incorrect++;
			else if (numUnkown(translatedtokenCipher[j]) == 0 && m_word.contains(translatedtokenCipher[j]))
				complete++;
		}

		if (incorrect > 0 || complete == 0) {
			m_translation.popMapping();
			continue;
		}
		else if (complete == translatedtokenCipher.size()) {
			output.push_back(m_translation.getTranslation(cipher_message));
			m_translation.popMapping();
			continue;
		}
		else {
			recCrack(cipher_message, output);
		}
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

#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const std::string& s) const;
private:
	vector<char> m_seperate;
};

TokenizerImpl::TokenizerImpl(string separators) //O(P)
{
	for (int i = 0; i < separators.size(); i++) {
		m_seperate.push_back(separators[i]);
	}
}

vector<string> TokenizerImpl::tokenize(const std::string& s) const //O(SP)
{
	vector<string> tokenize;
	string temp;
	for (int i = 0; i < s.size(); i++) {
		bool isPunt = false;
		for (int j = 0; j < m_seperate.size(); j++) {
			if (s[i] == m_seperate[j])
				isPunt = true;
		}
		if (!isPunt)
			temp = temp + s[i];
		else {
			if(temp.length() > 0)
				tokenize.push_back(temp);
			temp = "";
		}
	}
	return tokenize;
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
    return m_impl->tokenize(s);
}

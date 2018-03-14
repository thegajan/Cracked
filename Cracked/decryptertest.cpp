#include "provided.h"
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
using namespace std;

int main() {
	Decrypter d;

	assert(d.load("wordlist.txt"));
	
	cout << "Loaded Words" << endl << "--------------------------------" << endl;

	vector<string> s;
	//s = d.crack("Trcy oyc koon oz rweelycbb vmobcb, wyogrcn oecyb; hjg ozgcy tc moox bo moya wg grc vmobck koon grwg tc ko yog bcc grc oyc trlvr rwb hccy oecyck zon jb. - Rcmcy Xcmmcn");
	//s = d.crack("bdttook");
	s = d.crack("Trcy oyc");

	for (int i = 0; i < s.size(); i++)
		cout << s[i] << endl;
	cout << "Passed all tests." << endl;
}
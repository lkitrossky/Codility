using namespace std;
#include <vector>
#include <string>
#include <algorithm>
#include <list>
#include <map>
#include <iostream>
string solution(string& S, int K);
static ostream& operator<<(ostream& os, const vector< char >& v);
static ostream& operator<<(ostream& os, const string& v);
static ostream& operator<<(ostream& os, const list< char >& v);

/*
	Silver solution
	https://app.codility.com/programmers/challenges/great_code_off2021/
	https://app.codility.com/cert/view/certPV3RF3-RV7GY8CUYV78C5CK/
	https://app.codility.com/cert/view/certPV3RF3-RV7GY8CUYV78C5CK/details/
	O(N**2) or O(N * min(K, N))
	The idea is as follows.
	The original string turned into connected list L
	The result string is called "result"
	We do iterations on ABC till the first letter every time passing from the beginning and possibly to end
	And if there is a characters < the starting and in the range, erase it and concatenate to results
	The current values are: L and remaining perturbations howManyLeft, current position
	After the pass ended there are two possiblities - some letters passed or none.
	If none remove the first and pass to result.
	In both cases return result + call for the leftover.
	The problem: we pass N^2 times.
	

*/
string solution(string& S, int K) {
	string result;
	map<char, int > amountsofChar;
	for (char x = 'a'; x <= 'x'; x++)
		amountsofChar[x] = 0;
	for (auto x: S)
		amountsofChar[x]++;
	list<char> L(S.begin(), S.end());	
	int howManyLeft = K;
	if (K < 1 || S.length() < 2)
		return S;
	char startingChar = S.at(0);
	for (char ch = 'a'; ch < startingChar; ch++) {
		if (amountsofChar[ch] <= 0)
			continue;
		if (L.empty())
			break;
		if (0 == howManyLeft) {
			string leftOvers(L.begin(), L.end());
			//result += leftOvers;
			break;
		}
		int position = 0;
		auto iter = L.begin();
		while (iter != L.end()) {
			if (howManyLeft < position) 
				break; // current character ch cannot be moved, go to the next from ABC
			if (*iter == ch) { // found one ch to pass to result
				result.push_back(ch);		// pass
				iter = L.erase(iter);		// remove from L
				howManyLeft -= position;	// this is movement to the beginning
			}
			else {
				iter++; //move in the list
				position++;
			}
		}
	}
	if (L.size() == S.length()) { //this round left untouched, remove the first
		result.push_back(*L.begin());
		L.erase(L.begin());
	}


	string leftOvers(L.begin(), L.end());
	//result += leftOvers;
	return result + solution(leftOvers, howManyLeft);
}


void test(const char * pS, int K, const char*  expected) {
	string S(pS);
	cout << S << " with K = " << K<< " ";
	string stringSolution = solution(S, K);
	cout << " solution: " << stringSolution << endl;
	if (stringSolution != string(expected))
		cout << " FAILED, expected: " << expected << endl;
	else
		cout << " OK" << endl;
}
int main() {
	cout << "Hello, fellowship!" << endl;
	test("decade", 4, "adcede");
	test("bbbabbb", 2, "babbbbb");
	test("abracadabra", 15, "aaaaabrcdbr");
	return 0;
}
static ostream& operator<<(ostream& os, const vector< char >& v) {
	for (auto iter = v.begin(); iter != v.end(); iter++)
		os << (*iter) << " ";
	return os;
}
static ostream& operator<<(ostream& os, const string& v) {
	os << v.c_str();
	return os;
}
static ostream& operator<<(ostream& os, const list< char >& v) {
	for (auto iter = v.begin(); iter != v.end(); iter++)
		os << (*iter);
	return os;
}
/*A string S is given. In one move, any two adjacent letters can be swapped. 
For example, given a string "abcd", it's possible to create "bacd", "acbd" or "abdc" in one such move. 
What is the lexicographically minimum string that can be achieved by at most K moves?
Write a function:
string solution(string &S, int K);
that, given a string S of length N and an integer K, returns the lexicographically minimum string that can be achieved by at most K swaps of any adjacent letters.
Examples:
1. Given S = "decade" and K = 4, your function should return "adcede". Swaps could be:
decade -> dceade,
dceade -> dcaede,
dcaede -> dacede,
dacede -> adcede.
2. Given S = "bbbabbb" and K = 2, your function should return "babbbbb". The swaps are:
bbbabbb -> bbabbbb,
bbabbbb -> babbbbb.
3. Given S = "abracadabra" and K = 15, your function should return "aaaaabrcdbr".
Write an efficient algorithm for the following assumptions:
	N is an integer within the range [1...100,000];
	string S consists only of lowercase letters ('a'-'z');
	K is an integer within the range [0...1,000,000,000]
*/
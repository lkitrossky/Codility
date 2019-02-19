#include <vector>
#include <string>
#include <algorithm>
using namespace std;
/*
https://app.codility.com/programmers/challenges/strontium2019/
Gold
https://app.codility.com/cert/view/certSK8ZGB-PGBTG4HNHG96643K/

Codility confirms that a person who identified to us on-line as:
Levi Kitrossky
was awarded
Codility Golden Award for the Strontium 2019 Challenge
This is the 56th Codility Golden Award granted this month.
Review detailed assessment
AWARD TYPE:Codility Golden Award (Strontium 2019)
EXPIRATION DATE:2019-02-01 17:00:00 GMT
CHALLENGE POSTED:2019-01-19 17:00:00 GMT
CHALLENGE SOLVED:2019-01-23 09:11:43 GMT
TIME SINCE POSTED:3 days
ASSESSMENT:correct functionality and scalability

*/

static const int ARRAYS_LEN = ('z' - 'a' + 1);

static vector<unsigned int> beginningsAsymmetric(ARRAYS_LEN,0), endingsAsymmetric(ARRAYS_LEN, 0), middles(ARRAYS_LEN, 0);
static unsigned int  inside = 0;
class AcquiredData {
public:
	pair<int, unsigned int> biggestSymmetricBeginning;	//(number of the word, length of the sequence in the beginning, the longest net)
	pair<int, unsigned int> secondSymmetricBeginning;	//the same but the second longest net
	pair<int, unsigned int> biggestSymmetricEnding;		//the same but towards the end
	pair<int, unsigned int> secondSymmetricEnding;
	AcquiredData():biggestSymmetricBeginning(-1, 0), secondSymmetricBeginning(-1, 0), biggestSymmetricEnding(-1, 0), secondSymmetricEnding(-1, 0) {}
	void updateSymmetric(int number, bool begin, unsigned int length);
	unsigned int getMaxSymmetric();
	unsigned int getMaxSymmetricBeginning() { return biggestSymmetricBeginning.second; }
	unsigned int getMaxSymmetricEnding() { return biggestSymmetricEnding.second; }
	void resetAcquiredSymmetricalData();
};
void AcquiredData::resetAcquiredSymmetricalData() {
	biggestSymmetricBeginning = pair<int, unsigned int>(-1, 0);
	secondSymmetricBeginning = pair<int, unsigned int>(-1, 0); 
	biggestSymmetricEnding = pair<int, unsigned int>(-1, 0); 
	secondSymmetricEnding = pair<int, unsigned int>(-1, 0);
}
void AcquiredData::updateSymmetric(int number, bool begin, unsigned int length) {
	if (begin) {
		if (-1 == biggestSymmetricBeginning.first) {
			biggestSymmetricBeginning.first = number;
			biggestSymmetricBeginning.second = length;
			return;
		}
		if (0 <= biggestSymmetricBeginning.first && length > biggestSymmetricBeginning.second) {
			secondSymmetricBeginning.first = biggestSymmetricBeginning.first;
			secondSymmetricBeginning.second = biggestSymmetricBeginning.second;
			biggestSymmetricBeginning.first = number;
			biggestSymmetricBeginning.second = length;
			return;
		}
		if (0 <= biggestSymmetricBeginning.first &&  length <= biggestSymmetricBeginning.second && -1 == secondSymmetricBeginning.first) {
			secondSymmetricBeginning.first = number;
			secondSymmetricBeginning.second = length;
			return;
		}
		if (0 <= biggestSymmetricBeginning.first &&  length <= biggestSymmetricBeginning.second && 0 <= secondSymmetricBeginning.first && length > secondSymmetricBeginning.second) {
			secondSymmetricBeginning.first = number;
			secondSymmetricBeginning.second = length;
			return;
		}
	}
	else {
		if (-1 == biggestSymmetricEnding.first) {
			biggestSymmetricEnding.first = number;
			biggestSymmetricEnding.second = length;
			return;
		}
		if (0 <= biggestSymmetricEnding.first && length > biggestSymmetricEnding.second) {
			secondSymmetricEnding.first = biggestSymmetricEnding.first;
			secondSymmetricEnding.second = biggestSymmetricEnding.second;
			biggestSymmetricEnding.first = number;
			biggestSymmetricEnding.second = length;
			return;
		}
		if (0 <= biggestSymmetricEnding.first && length <= biggestSymmetricEnding.second && -1 == secondSymmetricEnding.first) {
			secondSymmetricEnding.first = number;
			secondSymmetricEnding.second = length;
			return;
		}
		if (0 <= biggestSymmetricEnding.first && length <= biggestSymmetricEnding.second && 0 <= secondSymmetricEnding.first && length > secondSymmetricEnding.second) {
			secondSymmetricEnding.first = number;
			secondSymmetricEnding.second = length;
			return;
		}
	}
}
unsigned int AcquiredData::getMaxSymmetric() {   //biggest from two symmetrical, symmetrical means the same character opens and close a word
	unsigned int res = 0;
	if (-1 == biggestSymmetricBeginning.first || -1 == biggestSymmetricEnding.first)
		return 0;
	if (biggestSymmetricBeginning.first != biggestSymmetricEnding.first)
		return biggestSymmetricBeginning.second + biggestSymmetricEnding.second;
	if (-1 == secondSymmetricBeginning.first && -1 == secondSymmetricEnding.first)
		return max(biggestSymmetricBeginning.second, biggestSymmetricEnding.second);
	if(0 <= secondSymmetricBeginning.first)
		res = secondSymmetricBeginning.second + biggestSymmetricEnding.second;
	if (0 <= secondSymmetricEnding.first)
		res = max(res, biggestSymmetricBeginning.second + secondSymmetricEnding.second);
	return res;
}
static vector<AcquiredData> aqDataSymmetric(ARRAYS_LEN);
void updateAquiredData(char c, int number, bool begin, unsigned int length) {
	aqDataSymmetric.at(c - 'a').updateSymmetric(number, begin, length);
}
unsigned int getMaxSymmetric(unsigned int i) {
	return aqDataSymmetric.at(i).getMaxSymmetric();
}
unsigned int getMaxSymmetricBeginning(unsigned int i) { return aqDataSymmetric.at(i).getMaxSymmetricBeginning(); }
unsigned int getMaxSymmetricEnding(unsigned int i) { return aqDataSymmetric.at(i).getMaxSymmetricEnding();}
void resetAcquiredSymmetricalData() {
	for (unsigned int i = 0; i < ARRAYS_LEN; i++)
		aqDataSymmetric.at(i).resetAcquiredSymmetricalData();
}
//here we suppose that the word is stripped from leading and trailing characters
// aaadfdffgfgfvv -> dfdffgfgf
//Parameters: word, stripped
//Return length of the longest subsequence of one character, for dfdffgfgf - 2
unsigned int insideLength1(const string &w) {
	unsigned int len = w.length();
	if (len < 2)
		return len;
	unsigned res = 0;
	char currentChar = w.at(0);
	unsigned int resCurrent = 1;
	for (unsigned int i = 1; i < len; i++) {
		if (currentChar == w.at(i)) {
			resCurrent++;
		}
		else {
			res = max(res, resCurrent);
			currentChar = w.at(i);
			resCurrent = 1;
		}
	}
	res = max(res, resCurrent);
	return res;
}

void handleWord(unsigned int number, const string &w) {
	const char starting = w.at(0);
	unsigned int len = w.length();
	unsigned int beginLen = 1;
	for (unsigned int i = 1; i < len; i++) {
		if (starting == w.at(i)) {
			beginLen++;
		}
		else {
			break;
		}
	}
	if (len == beginLen) {
		middles.at(starting - 'a') += beginLen;
		return;
	}
	const char ending = w.at(len - 1);
	unsigned int endLen = 1;
	for (unsigned int i = len - 2; i >= 0; i--) {
		if (ending == w.at(i)) {
			endLen++;
		}
		else {
			break;
		}
	}
	if (starting != ending) {
		beginningsAsymmetric.at(starting - 'a') = max(beginLen, beginningsAsymmetric.at(starting - 'a'));
		endingsAsymmetric.at(ending - 'a') = max(endingsAsymmetric.at(ending - 'a') , endLen);
		inside = max(inside, insideLength1(w.substr(beginLen, len - beginLen - endLen)));
		return;
	}
	else {

		updateAquiredData(starting, number, true , beginLen);
		updateAquiredData(starting, number, false, endLen  );
		inside = max(inside, insideLength1(w.substr(beginLen, len - beginLen - endLen)));
	}
}

int solution(vector<string> &words) {
	unsigned int res = 0;
	for (unsigned int i = 0; i < words.size(); i++) {
		handleWord(i, words.at(i));
	}
	for (int i = 0; i < ARRAYS_LEN; i++) {
		res = max(res, beginningsAsymmetric.at(i) + endingsAsymmetric.at( i) + middles.at(i));	// aaaaaaaaxx  aaa  xxaaaaaaa
		res = max(res, getMaxSymmetricBeginning(i) + endingsAsymmetric.at(i) + middles.at(i));			// aaxaa aaaa xxaaa
		res = max(res, beginningsAsymmetric.at(i) + middles.at(i) + getMaxSymmetricEnding(i));			// xxaaaa  aaaa aaxaa
		res = max(res, getMaxSymmetric(i) + middles.at(i));
	}
	res = max(res, inside);
	return res;
}

#include <iostream>

void test(vector<string> &words, int expected) {
	beginningsAsymmetric.resize(0);  endingsAsymmetric.resize(0); middles.resize(0);
	beginningsAsymmetric.resize(ARRAYS_LEN);  endingsAsymmetric.resize(ARRAYS_LEN); middles.resize(ARRAYS_LEN);
	inside = 0;
	resetAcquiredSymmetricalData();
	int real = solution(words);
	if (real != expected)
		cout << "Error! ";
	cout << "Expected: " << expected << " Real: " << real << endl;
}

int main() {
    
	string a2[] = { string("aabaa"), string("aba"), string("a") };
	vector<string> s2(a2, a2 + 3);
	test(s2, 4);
	//                         10           11                     10           12             
	string a7[] = { string("aaaaaaaaaabaaaaaaaaaaa"), string("aaaaaaaaaabaaaaaaaaaaaa") };
	vector<string> s7(a7, a7 + 2);
	test(s7, 22);
	//                      10           11                     10              20                            15              15
	string a6[] = { string("aaaaaaaaaabaaaaaaaaaaa"), string("aaaaaaaaaabaaaaaaaaaaaaaaaaaaaa"), string("aaaaaaaaaaaaaaabaaaaaaaaaaaaaaa") };
	vector<string> s6(a6, a6 + 3);
	test(s6, 35);

	string a1[] = { string("aabb"), string("aaaa"), string("bbab") };
	vector<string> s1(a1, a1 + 3);
	test(s1, 6);
	string a5[] = { string("aaddddddda"), string("aa"), string("aa"), string("add") };
	vector<string> s5(a5, a5 + 4);
	test(s5, 7);
	string a4[] = { string("addddddda"), string("abbbbbbbbbbbbbbba"), string("cc"), string("dd") };
	vector<string> s4(a4, a4 + 4);
	test(s4, 15);
	string a3[] = { string("dd"), string("bb"), string("cc"), string("dd") };
	vector<string> s3(a3, a3 + 4);
	test(s3, 4);
	
	
	return 0;
}

/*

An array of N words is given. Each word consists of small letters ('a' - 'z'). Our goal is to concatenate the words in such a way as to obtain a single word with the longest possible substring composed of one particular letter. Find the length of such a substring.

Write a function:

int solution(vector<string> &words);

that, given an array words containing N strings, returns the length of the longest substring of a word created as described above.

Examples:

1. Given N=3 and words=["aabb", "aaaa", "bbab"], your function should return 6. One of the best concatenations is words[1] + words[0] + words[2] = "aaaaaabbbbab". The longest substring is composed of letter 'a' and its length is 6.

2. Given N=3 and words=["xxbxx", "xbx", "x"], your function should return 4. One of the best concatenations is words[0] + words[2] + words[1] = "xxbxxxxbx". The longest substring is composed of letter 'x' and its length is 4.

3. Given N=4 and words=["dd", "bb", "cc", "dd"], your function should return 4. One of the best concatenations is words[0] + words[3] + words[1] + words[2] = "ddddbbcc". The longest substring is composed of letter 'd' and its length is 4.

Write an efficient algorithm for the following assumptions:

N is an integer within the range [1...100000];
all the words are non-empty and consist only of lowercase letters (a-z);
S denotes the sum of the lengths of words; S is an integer within the range [1...100000].


*/
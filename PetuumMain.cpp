#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <iostream>
using namespace std;
char findSecondChar(const string &S, char first, unsigned char pos = 0) {
	for (auto i = pos; i < S.length(); i++) {
		if (S.at(i) != first)return S.at(i);
	}
	return '0';
}
void swap(char &f, char &s) {
	char t = f;
	f = s;
	s = t;
}
void printStack(stack<int> s);
class Position {
public:
	Position() :first('0'), second('0'), aggregated(0){}
	unsigned int getNumber(char f, char s) {
		if (f == s)return 0;
		if (f != first && f != second)return 0;
		if (s != first && s != second)return 0;
		return aggregated;
	}
	bool setNumber(char f, char s, unsigned int n) {
		if (f == s)return false;
		first = f;
		second = s;
		aggregated = n;
		return true;
	}
private:
	char first, second;
	unsigned int aggregated;
};
// https://app.codility.com/cert/view/certNMMZ78-XHQDKFUMG4YRJZ4R/details/
// many errors in calc and performance
//N = 100, S contains two different letters. WRONG ANSWER  got 4 expected 6
int solution(string &S) {
	unsigned int res = 0;
	unsigned int N = S.length();
	if (N < 2)return 0;
	if (2 == N) {
		if (S.at(0) != S.at(1))
			return 2;
		else
			return 0;
	}
	char first = S.at(0);
	char secon = findSecondChar(S, first);
	if (secon == '0')
		return 0;
	vector<Position>positions(N, Position());
	stack<int> firstStack;
	stack<int> seconStack;
	firstStack.push(-1);
	firstStack.push(0);
	seconStack.push(-1);
	seconStack.push(0);
	for (unsigned int i = 1; i < N; i++) {
		char current = S.at(i);
		if (current == first || current == secon) { 
			if (!seconStack.empty() && seconStack.top() == current) {
				seconStack.push(i);
			}
			else {
				seconStack = stack<int>();
				seconStack.push(i - 1);  
				seconStack.push(i);
			}
		}
		if (current == first) { 
			firstStack.push(i);
		}
		else if (current == secon) {
			firstStack.pop();
			if (firstStack.empty()) { 
				swap(first, secon);   
				firstStack.push(i - 1);  
				firstStack.push(i); 
			}
			else { //fragment found
				unsigned int previous = 0;
				if(firstStack.top() >= 0 && firstStack.top() < (int)N)
					previous = positions.at(firstStack.top()).getNumber(first, secon); //preceding framgent can be good
				unsigned int fragmentRes = i - firstStack.top() + previous;
				positions.at(i).setNumber(first, secon, fragmentRes); //remember this fragment
				res = max(res, fragmentRes); 
			}
		}
		else {  //new char, not first nor secon
			firstStack = seconStack;
			seconStack = stack<int>();
			seconStack.push(i - 1);
			seconStack.push(i);
			if(firstStack.empty()){
				secon = findSecondChar(S, current, i);
				if ('0' == secon)
					break; //nothing to do, stack empty and not second available
				firstStack.push(i - 1);
				firstStack.push(i);
			}
			else {
				first = secon;
				secon = current;
				firstStack.pop();
				if (firstStack.empty()) {
					swap(first, secon);
					firstStack.push(i - 1);  //like -1
					firstStack.push(i);
				}
				else { //fragment found
					unsigned int previous = positions.at(firstStack.top()).getNumber(first, secon); //preceding framgent can be good
					unsigned int fragmentRes = i - firstStack.top() + previous;
					positions.at(i).setNumber(first, secon, fragmentRes); //remember this fragment
					res = max(res, fragmentRes); 
				}
			}
		}
		/*
		std::cout << "#" << i << " Res: " << res << " First: " << first << " Second: " << secon << " Current:" << current << endl;
		std::cout << "firstStack: ";
		printStack(firstStack);
		std::cout << endl;
		std::cout << "secondStack: ";
		printStack(seconStack);
		std::cout << endl << endl;
		*/
	}
	return res;
}


// utilities, driver
void test(string & s, int expected) {
	int res = solution(s);
	if (res == expected) {
		std::cout << "Passed, string: " << s.c_str() << " as expected:" << expected << endl;
	}
	else {
		std::cout << "failed!! String: " << s.c_str() << " expected:" << expected<< " Calculated: " <<res << endl;
		int x;  cin >> x;
	}
}
void test(const char * ss, int expected) { return test(string(ss), expected); }
string randomstring(unsigned int len, char ex1, char ex2) {
	string sres;
	while (sres.length() < len) {
		char c = 'a' + rand() % ('z' - 'a' + 1);
		while(c == ex1 || c == ex2)
			c = 'a' + rand() % ('z' - 'a' + 1);
		sres += string(1,c);
	}
	return sres;
}
void testsOneCharacter();
void testsTwoCharacter() {
	cout << "Tests with two characters\n";
	test("ab", 2);
	test("abba", 4);
	test("abbaba", 6);
	test("abababa", 6);
	test("ababab", 6);
	test("aaabb", 4);
	test("aabbab",6);
	test("aabbbbbbbbbbbbbbbbbbbbbbaa", 4);
	test("aabbbbbbbbbbbbbbbbbbbbbbaaa", 6);
	test("aabbbbbbbbbbbbbbbbbbbbaabbaa", 8);
	vector<int> tv(100, 0);
	string ts(100, '0');
	for (int i = 0; i < 100; i++) {
		tv[i] = i;
	}
	for (int i = 0; i < 50; i++) {
		unsigned int posa;
		1 == tv.size()? posa = 0 : posa = rand() % tv.size();
		ts.at(tv[posa]) = 'a';
		tv.erase(tv.begin() + posa);
		1 == tv.size() ? posa = 0 : posa = rand() % tv.size();
		ts.at(tv[posa]) = 'b';
		tv.erase(tv.begin() + posa);
	}
	cout << ts.c_str() << endl;
	test(ts, 100);
}
void testsThreeCharacters() {
	test("cabbacc", 4);
	test("cabbadbab", 4);
	test("abababc", 6);
	test("cababab", 6);
}
void testMultipleCharacters(){
	test("keywcofqkdmzoizgqipzyixrydwsovabbaocyhzqjzdhtrrrllktchfnctpirnwonydwwvuuqcimcnlhqodemkmunmxiom", 4);
	string s8("abba");
	for (int i = 0; i < 200; i++)
		test(randomstring(30, 'a', 'b') + s8 + randomstring(60, 'a', 'b'), 4);
}
int main() {
	//testsOneCharacter();
	testsTwoCharacter();
	testsThreeCharacters();
	//testMultipleCharacters();
	
	return 0;
}
void printStack(stack<int> s) {
	while (!s.empty()) {
		cout << s.top() << " ";
		s.pop();
	}
}
void testsOneCharacter(){
	cout << "Tests with one character\n";
	test("a", 0);
	test("aa", 0);
	test("aaa", 0);
	test("aaaaaaa", 0);
}

//unsuccessful
int howMany(const string &S, int startInclusive, int endExclusive) {
	int res = 0;
	endExclusive = min(endExclusive, (int)S.length());
	startInclusive = max(startInclusive, 0);
	if (startInclusive >= endExclusive)
		return 0;
	if (S.length() < 1)
		return 0;
	vector<int> dict('z' - 'a' + 1, 0);
	for (int i = startInclusive; i < endExclusive; i++)
		dict[S.at(i) - 'a']++;
	for (auto & i : dict) {
		if (i > 0)res++;
	}
	return res;
}
int maxBalanced(const string &S) {
	if (howMany(S, 0, S.length()) != 2)
		return 0;
	unsigned int res = 0;
	const unsigned int N = S.length();
	std::stack< pair<char, unsigned int> > pstask;
	char first = S.at(0);
	char secon = findSecondChar(S, first);
	pstask.push(pair<char, unsigned int>(first, 0));
	for (unsigned int i = 1; i < N; i++) {
		if (pstask.empty() || S.at(i) == pstask.top().first)
			pstask.push(pair<char, unsigned int>(S.at(i), i));
		else {
			pstask.pop();
			if (pstask.empty())
				res = i + 1;
			else
				res = max(res, i - pstask.top().second);
		}
	}
	return res;
}
vector<vector<int> > precalcVector;
int solution2(string &S) {
	int res = 0;

	unsigned int N = S.length();
	if (N < 2)return 0;
	if (2 == N) {
		if (S.at(0) != S.at(1))
			return 2;
		else
			return 0;
	}
	precalcVector = vector<vector<int> >(N + 1, vector<int>('z' - 'a' + 1, 0));
	for (unsigned int i = 0; i < N; i++) {
		precalcVector[i + 1] = precalcVector[i];
		precalcVector[i + 1][S.at(i) - 'a']++;
	}

	bool bExactlyTwo = false;
	unsigned int leftInclusive = 0;
	unsigned int rightExclusive = 1;
	while (rightExclusive <= N) {
		int howManyUniqs = howMany(S, leftInclusive, rightExclusive);
		if (howManyUniqs < 2) {
			bExactlyTwo = false;
			rightExclusive++;
		}
		if (2 == howManyUniqs) {
			bExactlyTwo = true;
			rightExclusive++;
		}
		if (howManyUniqs > 2) {
			if (bExactlyTwo) {
				res = max(res, maxBalanced(S.substr(leftInclusive, rightExclusive - leftInclusive - 1)));
			}
			bExactlyTwo = false;
			leftInclusive++;
		}
	}
	int howManyUniqs = howMany(S, leftInclusive, rightExclusive);
	if (2 == howManyUniqs) {
		res = max(res, maxBalanced(S.substr(leftInclusive, rightExclusive - leftInclusive - 1)));
	}
	return res;
}
int solution3(string &S) {
	unsigned int res = 0;

	unsigned int N = S.length();
	if (N < 2)return 0;
	if (2 == N) {
		if (S.at(0) != S.at(1))
			return 2;
		else
			return 0;
	}
	char first = S.at(0);
	char secon = findSecondChar(S, first);
	if (secon == '0')
		return 0;
	stack<int> firstStack; //this stack is like () algo, but order of brackets not important, first, secon
	stack<int> seconStack; //this is only for the secons, after third comes it turns the first
	firstStack.push(-1);
	unsigned int fsLength = 0;
	unsigned int fsLengthSegment = 0;
	for (unsigned int i = 0; i < N; i++) {
		char current = S.at(i);
		if (current == first) { //a,a,a case, we prevent any number enter stack except index of the first
			firstStack.push(i);
		}
		else if (current == secon) {
			//we suppose the stack is not empty, if second comes to empty, it turns first
			firstStack.pop();
			if (firstStack.empty()) { // situation like a,b,b - now b will be first
				fsLength += fsLengthSegment;  //fragment like a,b,a,b or aabb finished, we need to take it length
				fsLengthSegment = 0;  //and add to over all good streak of a,b in any order
				swap(first, secon);  //b turns first 
				firstStack.push(i - 1);  //prepare stack as in the beginning
				firstStack.push(i); // 
			}
			else { //good fragment with a,b, a, b or aabb continues
				fsLengthSegment = max(fsLengthSegment, i - firstStack.top()); //update the length of the segment
																			  // or just add to fsLength and zero? : fsLength += fsLengthSegment; fsLengthSegment = 0;
																			  //something wrong here. Consider: ababab aaaa abab  - 6
																			  //                                ababab aaaa abab bbbb - 12
			}
			if (seconStack.empty()) {
				seconStack.push(i - 1);
				seconStack.push(i);
			}
			else {
				seconStack.push(i);
			}
		}
		else {
			res = max(res, fsLength + fsLengthSegment);
			fsLength = 0;
			fsLengthSegment = 0;
			first = secon;
			secon = current;
			firstStack = seconStack;
			seconStack = stack<int>();
			seconStack.push(i - 1);
			seconStack.push(i);
			firstStack.pop();
			if (firstStack.empty()) {
				swap(first, secon);
				firstStack.push(i);
			}
			else {
				fsLength = i - firstStack.top();
				res = max(res, fsLength);
			}
		}
		std::cout << "#" << i << " Res: " << res << " First: " << first << " Second: " << secon << " Current:" << current << " fsLength:" << fsLength << " fsLengthSegment:" << fsLengthSegment << endl;
		std::cout << "firstStack: ";
		printStack(firstStack);
		std::cout << endl;
		std::cout << "secondStack: ";
		printStack(seconStack);
		std::cout << endl << endl;
	}
	res = max(res, fsLength + fsLengthSegment);
	return res;
}

//https://app.codility.com/programmers/challenges/grand2018/
// https://app.codility.com/cert/view/cert2C83GE-PA5S8ZRCMA6P2X33/details/
//This time there are no odd results, but still there errors
/*
N = 100, S contains two different letters
got 4 expected 6
small_segments N = 100.
got 24 expected 42
Something wrong with stack?
Find the previous stack problem. Sulphur?
And also timeouts. Detected time complexity: O(N**2)
Need to think about. Possibly stack inside the solution and not as a separate function
*/
// https://www.geeksforgeeks.org/length-of-the-longest-valid-substring/


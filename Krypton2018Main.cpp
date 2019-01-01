#include <vector>
#include <algorithm>
using namespace std;

int findPower(int K, int Q) { //how many times K can be divided by Q?  K = k*Q**n; find n
	int res = 0;
	while (0 == K%Q && K > 0) {
		K = K / Q;
		res++;
	}
	return res;
}
pair<int, int> findPower(int K, int Q1, int Q2) { //the same but for two
	return pair<int, int>(findPower(K, Q1), findPower(K, Q2));
}


typedef pair<int, int> Pair;  //first is powers of 2, second - powers of 5
typedef pair<Pair, Pair> TPairs;  //firs it best for two, the second - best for five
Pair operator+(const Pair& p1, const Pair& p2) {
	return Pair(p1.first+p2.first, p1.second+p2.second);
}

bool comparison1(Pair p1, Pair p2) {  
	if (p1.first != p2.first)
		return p1.first < p2.first;
	if (p1.second != p2.second)
		return p1.second < p2.second;
	return true;
}
bool comparison2(Pair p1, Pair p2) {
	if (p1.second != p2.second)
		return p1.second < p2.second;
	if (p1.first != p2.first)
		return p1.first < p2.first;
	return true;
}
/*
 Th emain idea is that to every point one can com eonly from below or right. So, we can just start from the right bottom corner 
 and go left up.
 First we get rid of big numbers, every number reduced to a pair of powers : two and five. lIke: 10-> (1,1), 15->(0,1), 250->(1,3), 17-> (0,0)
 We need to keep two variants though: the best by 2 and from the the best by power of 5 and vice versa. The best means the lowest.
 The we build another array, wich has on every point a pair of pairs: (best by power of 2, between them the best by power of 5), (best by power of 2 between the same power of 5, best by power of 5)
 On every position we take the best possible from the right and below.
 A special treatment is for 0, we cannot use (0,0) becaus ethen we can arrive to total zero answer while passing (0,0), while it must be 1. So, if we meet 0 of A we put above and left (1,1), the the final answer is corrected, to 1
 if necessary, because it cannot be > 1. 

*/
int solution(vector< vector<int> > &A) {
	const int N = A.at(0).size();
	bool zeroFound = false;
	Pair zeroPair(0, 0);
	vector<Pair > zeroVector(N, zeroPair);
	vector< vector< pair<int, int> > > AP (N, zeroVector);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			AP.at(i).at(j) = findPower(A.at(i).at(j), 2, 5);
		}
	}

	vector< vector< TPairs > > APP (N, vector< TPairs >(N)); //this one to put result of calculation

	for (int i = N - 1; i >= 0; i--) {
		for (int j = N - 1; j >= 0; j--) {
			if (0 == A.at(i).at(j)) {
				APP.at(i).at(j) = TPairs(Pair(1,1), Pair(1, 1));
				continue;
			}
			if (N - 1 == i && N - 1 == j) {
				APP.at(i).at(j) = TPairs(AP.at(i).at(j), AP.at(i).at(j));
			}
			if (N - 1 == i && N - 1 > j) {
				APP.at(i).at(j) = TPairs(AP.at(i).at(j) + APP.at(i).at(j+1).first, AP.at(i).at(j) + APP.at(i).at(j + 1).second);
			}
			if (N - 1 > i && N - 1 == j) {
				Pair f = AP.at(i).at(j) + APP.at(i + 1).at(j).first;
				Pair s = AP.at(i).at(j) + APP.at(i + 1).at(j).second;
				APP.at(i).at(j) = TPairs(f, s);
			}
			if (N - 1 > i && N -1 > j) {
				
				Pair f1 = AP.at(i).at(j) + APP.at(i).at(j + 1).first;
				Pair f2 = AP.at(i).at(j) + APP.at(i+1).at(j).first;
				Pair s1 = AP.at(i).at(j) + APP.at(i).at(j + 1).second;
				Pair s2 = AP.at(i).at(j) + APP.at(i + 1).at(j).second;
				Pair f = comparison1(f1, f2) ? f1 : f2;
				Pair s = comparison2(s1, s2) ? s1 : s2;
				APP.at(i).at(j) = TPairs(f, s);
			}
		}
	}

	int res1 = min(APP.at(0).at(0).first.first, APP.at(0).at(0).first.second);
	int res2 = min(APP.at(0).at(0).second.first, APP.at(0).at(0).second.second);
	int res = min(res1, res2);


	//if there is a zero, the result is not bigger than 1, just poass 0 for it.
	for (auto & a1 : A) {
		for (auto & a2 : a1) {
			if (0 == a2) {
 				zeroFound = true;
				break;
			}
		}
		if (zeroFound)
			break;
	}
	if (zeroFound)
		res = std::min(res, 1);
	return res;
}



/////////////////////////////////////
/// drivers ///////////////////////////


/*

https://app.codility.com/cert/view/certQJBUEC-S427WM5SYSHKRUX3/details/
For example, for the input [[0]] the solution returned a wrong answer (got 0 expected 1).
Performance is OK
failures:
[[0]]  must give 1, returns 0. 

Corrected, now:
https://app.codility.com/cert/view/certKUUT2P-JF8Q3B9YPEASU94H/    #41
https://app.codility.com/cert/view/cert6FMAFX-E8QSN2R4W7ANEWJU/    #45 with explanation


Codility confirms that a person who identified to us on-line as:
Levi Kitrossky
was awarded
Codility Golden Award for the Krypton 2018 Challenge
This is the 41st Codility Golden Award granted this month.
Review detailed assessment
AWARD TYPE:Codility Golden Award (Krypton 2018)
EXPIRATION DATE:2020-10-20 17:00:00 GMT
CHALLENGE POSTED:2018-10-20 17:00:00 GMT
CHALLENGE SOLVED:2018-10-22 09:23:17 GMT
TIME SINCE POSTED:1 day
ASSESSMENT:correct functionality and scalability

*/


#include <iostream>

ostream & operator<<(ostream & os, const Pair & p) {
	os << p.first << " " << p.second;
	return os;
}

void testPower() {
	cout << "(1,2)+(3,4):" << Pair(1, 2) + Pair(3, 4) << endl;
	cout << "findPower(25, 5): " << findPower(25, 5) << endl;
	cout << "findPower(250, 5):" << findPower(250, 5) << endl;
	cout << "findPower(0, 5):" << findPower(0, 5) << endl;
	cout << "findPower(11, 5):" << findPower(11, 5) << endl;
}

void test( vector< vector<int> > A, int e) {
	int res = solution(A);
	if (res != e) {
		cout << "Error! Expected: " << e << " Calc: " << res << endl;
	}
}

int main() {


	vector< vector<int> > A4;
	A4.push_back(vector<int>({ 0 }));
	test(A4, 1);

	vector< vector<int> > A3;
	A3.push_back(vector<int>({ 10,1,10,1 }));
	A3.push_back(vector<int>({ 1,1,1,10 }));
	A3.push_back(vector<int>({ 10,1,10,1 }));
	A3.push_back(vector<int>({ 1,10,1,1 }));
	test(A3, 2);

	vector< vector<int> > A2;
	A2.push_back(vector<int>({ 2,10,1,3 }));
	A2.push_back(vector<int>({ 10,5,4,5 }));
	A2.push_back(vector<int>({ 2,10,2,1 }));
	A2.push_back(vector<int>({ 25,2,5,1 }));
	test(A2, 1);

	vector< vector<int> > A1;
	A1.push_back(vector<int>({10,10,10}));
	A1.push_back(vector<int>({ 10,0,10 }));
	A1.push_back(vector<int>({ 10,10,10 }));
	test(A1, 1);
	return 0;
}
//FutureMobilityCodilityChallenge2018
//https://app.codility.com/programmers/custom_challenge/future_mobility2018/
// 100% Only performance
// https://app.codility.com/demo/results/trainingFKHYTY-MTM/



#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;
#define MODULO 1000000007

long long solutionFragment(vector<int> &C) {  //this fragment has movement in one direction only, no last of first 0
	unsigned long long res = 0;
	const unsigned int N = C.size();
	if (C.at(0) < 0) {
		for (unsigned i = 0; i < N; i++) {
			C.at(i) = -C.at(i);
		}
	}
	unsigned long long doubleMoves = 0;
	unsigned long long firstIntegral = C.at(0);
	unsigned long long secondIntegral = C.at(0);
	unsigned long long oddCandidates = 0;
	unsigned long long evenCandidates = C.at(0);
	for (unsigned i = 1; i < N; i++) {
		firstIntegral += C.at(i);
		secondIntegral += firstIntegral;
		if (C.at(i) >= 0) {
			if (1 == i % 2) {
				oddCandidates += C.at(i);
				doubleMoves += evenCandidates;
			}
			else {
				evenCandidates += C.at(i);
				doubleMoves += oddCandidates;
			}
		}
		else {
			unsigned long long decrease = -C.at(i);
			if (1 == i % 2) {
				if (oddCandidates >= decrease) {
					oddCandidates -= decrease;
				}
				else {
					evenCandidates -= (decrease - oddCandidates);
					oddCandidates = 0;
				}
				doubleMoves += evenCandidates;
				doubleMoves = doubleMoves;
			}
			else {
				if (evenCandidates >= decrease) {
					evenCandidates -= decrease;
				}
				else {
					oddCandidates -= (decrease - evenCandidates);
					evenCandidates = 0;
				}
				doubleMoves += oddCandidates;
				doubleMoves = doubleMoves;
			}
		}
	}
	unsigned long long singleMoves = secondIntegral;
	res = (singleMoves - doubleMoves) % MODULO;
	return res;
}
int sign(long long i) {
	if (0 == i)return 0;
	return (int)(abs(i) / i);
}
void cutInFrgaments(vector<int> _C, vector<vector<int> > &out_fragments) {
	unsigned int N = _C.size();
	unsigned int startingZeros = 0;
	for (unsigned int i = 0; i < N; i++) {
		if (0 == _C.at(i))
			startingZeros++;
		else
			break;
	}
	unsigned int endingZeros = 0;
	for (unsigned int i = 0; i < N; i++) {
		if (0 == _C.at(N - 1 - i))
			endingZeros++;
		else
			break;
	}
	if (startingZeros == N)
		return;
	if (startingZeros + endingZeros >= N-1)
		return;
	vector<int> C(_C.begin() + startingZeros, _C.end() - endingZeros);
	if (C.empty()) return;
	long long sum = C.at(0);
	int currentSign = sign(sum);
	for (unsigned int i = 1; i < C.size(); i++) {
		sum += C.at(i);
		if (currentSign != sign(sum) && 0 != sign(sum)) {
			vector<int> add(C.begin(), C.begin() + i); 
			add.push_back(C.at(i) - sum);
			out_fragments.push_back(add);
			C.at(i) = sum;
			cutInFrgaments(vector<int>(C.begin() + i, C.end()), out_fragments);
			return;
		}
		if (0 == sum) {
			vector<int> add(C.begin(), C.begin() + i + 1);
			out_fragments.push_back(add);
			cutInFrgaments(vector<int>(C.begin() + i + 1, C.end()), out_fragments);
			return;
		}
	}
}
vector<vector<int> > cutInFrgaments(vector<int> C) {
	
	vector<vector<int> > fragments;
	cutInFrgaments(C, fragments);
	return fragments;
}
long long solution(vector<int> &C) {
	unsigned long long res = 0;
	vector<vector<int> > fragments = cutInFrgaments(C);
	for (auto &f : fragments) {
		res = (res + solutionFragment(f)) % MODULO;;
	}
	return res;
}
int solution(vector<int> &A, vector<int> &B) {
	unsigned int N = A.size();
	vector<int> C(N);
	for (unsigned int i = 0; i < N; i++) {
		C.at(i) = A.at(i) - B.at(i); 
	}
	if (0 != accumulate(C.begin(), C.end(), 0))
		return -1;
	long long res = solution(C) % MODULO;
	return (int)res;
}



////////////////////////////////////////////////////////////////////
//   Driver and tests
//My error was to try as Selenium https://app.codility.com/programmers/challenges/selenium2018/
// Selenium Gold medal   https://app.codility.com/cert/view/certGZEKZJ-BDVHM9F6ETVBVEUP/
// 

#include <iostream>
bool test(vector<int> &A, vector<int> &B, int expected, bool printAll = false, bool doNotPrint = true) {
	int res = solution(A, B);
	if ((expected != res && !doNotPrint) || printAll) {
		for (size_t i = 0; i < A.size(); i++) {
			cout << A.at(i) << " ";
		}
		cout << endl;
		for (size_t i = 0; i < B.size(); i++) {
			cout << B.at(i) << " ";
		}
		cout << endl;
		if (expected != res) {
			cout << "Error! Expected: " << expected << "  " << " Result: " << res << endl;
			return false;
		}
		else
			cout << "Expected: " << expected << "  ";
	}
	cout << " Result: " << res << endl;
	return true;
}

// https://app.codility.com/demo/results/trainingPVVXBZ-QBR/

/*

tiny
N <= 10.WRONG ANSWER
got 21 expected 18
1. 0.001 s WRONG ANSWER,  got 21 expected 18
2. 0.001 s WRONG ANSWER,  got 6 expected 23

small
N = 1000.WRONG ANSWER
got 1497976 expected 1407591
1. 0.001 s WRONG ANSWER,  got 1497976 expected 1407591
2. 0.001 s WRONG ANSWER,  got -1015078 expected 2647642

shuffle
Array B is shuffled array A, N~1000. WRONG ANSWER
got 40914801 expected 425100566
1. 0.001 s WRONG ANSWER,  got 40914801 expected 425100566
2. 0.001 s OK
concentrated
Move single stone from every tower to some chosen tower, N = 1000.
got 127919 expected 134980

*/
/*
The idea is that tower with K sends into k adjacent empty and we can have a formula
*/
int concentrateFormula(int k) {
	int p = k / 2;
	if (2 * p == k) {
		return p*(p + 1);
	}
	else {
		return (p + 1)*(p + 1);
	}
}
bool concentrateTest() {
	int length = 1 + rand() % 1000;
	int pos = rand() % length;
	int calc = concentrateFormula(pos) + concentrateFormula(length - pos - 1);
	vector<int> A(length, 1);
	vector<int> B(length, 0);
	B.at(pos) = length;
	if (!test(A, B, calc, false, false)) {
		cout << "concentrateTest failed with length: " << length << " Position: " << pos << endl;
		return false;
	}
	return true;
}

void EasyTests() {
	vector<int> A03(1000, 1);
	vector<int> B03(1000, 0);

	B03[0] = 0;
	B03[999] = 0;
	B03[500] = 1000;
	int exp = concentrateFormula(500) + concentrateFormula(499);
	test(A03, B03, exp % MODULO, false, false);



	B03[999] = 0;
	B03[0] = 1000;
	B03[500] = 0;
	test(A03, B03, 250000 % MODULO, false, false);

	B03[0] = 0;
	B03[999] = 1000;
	B03[500] = 0;
	test(A03, B03, 250000 % MODULO, false, false);

	vector<int> Asmall = { 1, 1, 1 };
	vector<int> Bsmall = { 0, 0, 3 };
	test(Asmall, Bsmall, 2, false, false);

	for (int i = 0; i < 200; i++) {
		if (!concentrateTest())return;
	}

	vector<int> A02{ 1000000000,1000000000,1000000000,0,0,0 };
	vector<int> B02{ 0,0,0, 1000000000,1000000000,1000000000 };

	test(A02, B02, 999999972, false, false);

	vector<int> A51(51, 1);
	vector<int> B51(51, 0);
	B51[5] = 51;
	test(A51, B51, 538, false, false);
	//return;
	//return;
	
	
	
	//return;

	vector<int> A03b(100000, 1);
	vector<int> B03b(100000, 0);
	B03b[0] = 100000;

	test(A03b, B03b, 2500000000 % MODULO, false, false);

	

	vector<int> A09{ 2,2,2,0,2 };
	vector<int> B09{ 1,3,3,0,1 };
	test(A09, B09, 2, false, false);

	vector<int> A08{ 2,2,0,0,2,2 };
	vector<int> B08{ 1,1,0,0,3,3 };
	test(A08, B08, 4, false, false);


	vector<int> A07{ 2,2,0,2,2 };
	vector<int> B07{ 1,1,0,3,3 };
	test(A07, B07, 3, false, false);

	vector<int> A06{ 2,2,2,2 };
	vector<int> B06{ 1,1,3,3 };
	test(A06, B06, 2, false, false);

	vector<int> A05{ 1,2,2,2,2 };
	vector<int> B05{ 1,1,3,1,3 };
	test(A05, B05, 2, false, false);

	vector<int> A04{ 2,2,2,2 };
	vector<int> B04{ 1,3,1,3 };
	test(A04, B04, 2, false, false);

	

	vector<int> A03a(10000, 1);
	vector<int> B03a(10000, 0);
	B03a[0] = 10000;

	test(A03a, B03a, 25000000 % MODULO, false, false);


	

	vector<int> A01{ 0,0,2,1,8,8,2,0 };
	vector<int> B01{ 8,5,2,4,0,0,0,2 };

	test(A01, B01, 31, false, false);

	vector<int> A00{ 1,1,2,4,3 };
	vector<int> B00{ 2,2,2,3,1 };
	int expected00 = -1;
	test(A00, B00, expected00, false, false);

	vector<int> A{ 1,1,2,4,3 };
	vector<int> B{ 2,2,2,3,2 };
	int expected = 3;
	test(A, B, expected, false, false);
}

int main()
{
	EasyTests();
	return 0;

}

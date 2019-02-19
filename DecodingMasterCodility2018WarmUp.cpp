// DecodingMasterCodility2018WarmUp.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
////////////////////////////////////////////////////////////////////////////
//   MissingInteger warm up for 
//   https://app.codility.com/programmers/challenges/decoding_master/
//   https://app.codility.com/demo/results/demoRFN4YW-E7G/   100% form the first time, 15 min. But the thinking was done before.
///////////////////////////////////////////////////////////////////////////
#include <vector>
#include <algorithm>
using namespace std;
int solution(vector<int> &A) {
	const int N = A.size();
	int res = N+1;
	vector<bool> check(N+1, false);
	for (auto &a : A) {
		if (a > 0 && a < N + 1) {
			check.at(a) = true;
		}
	}
	for (int i = 1; i < N + 1; i++) {
		if (!check.at(i)) {
			res = i;
			break;
		}
	}
	return res;
}
//////////////////////////////////////////////////////////////////////////
#include <iostream>
void test(vector<int> &A, int expected) {

	int res = solution(A);
	if (res != expected) {
		cout << "Error! Expected: " << expected << " calc: " << res << endl;
		for (auto & a : A) {
			cout << a << " ";
		}
		cout << endl;
	}

}

int main()
{
	vector<int> VA, VB;
	int expected = 0;

	int A0[] = { -1, -3 };
	VA = vector<int>(A0, A0 + 2);
	expected = 1;
	test(VA, expected);

	int A[] = {1, 3, 6, 4, 1, 2};
	VA = vector<int>(A, A + 6);
	expected = 5;
	test(VA, expected);

	int A1[] = { 1, 2, 3 };
	VA = vector<int>(A1, A1 + 3);
	expected =	4;
	test(VA, expected);

    return 0;
}


// Cobalt2018.cpp : Defines the entry point for the console application.
//  https://app.codility.com/programmers/challenges/
//  https://app.codility.com/programmers/custom_challenge/cobaltum2018/
/*
You have two sequences A and B consisting of integers, both of length N, and you would like them to be (strictly) increasing, i.e. for each K (0 <= K < N - 1), A[K] < A[K + 1] and B[K] < B[K + 1]. Thus, you need to modify the sequences, but the only manipulation you can perform is to swap an arbitrary element in sequence A with the corresponding element in sequence B. That is, both elements to be exchanged must occupy the same index position within each sequence.

For example, given A = [5, 3, 7, 7, 10] and B = [1, 6, 6, 9, 9], you can swap elements at positions 1 and 3, obtaining A = [5, 6, 7, 9, 10], B = [1, 3, 6, 7, 9].

Your goal is make both sequences increasing, using the smallest number of moves.

Write a function:

class Solution { public int solution(int[] A, int[] B); }

that, given two zero-indexed arrays A, B of length N, containing integers, returns the minimum number of swapping operations required to make the given arrays increasing. If it is impossible to achieve the goal, return -1.

For example, given:

A[0] = 5        B[0] = 1
A[1] = 3        B[1] = 6
A[2] = 7        B[2] = 6
A[3] = 7        B[3] = 9
A[4] = 10       B[4] = 9
your function should return 2, as explained above.

Given:

A[0] = 5        B[0] = 2
A[1] = -3       B[1] = 6
A[2] = 6        B[2] = -5
A[3] = 4        B[3] = 1
A[4] = 8        B[4] = 0
your function should return -1, since you cannot perform operations that would make the sequences become increasing.

Given:

A[0] = 1        B[0] = -2
A[1] = 5        B[1] = 0
A[2] = 6        B[2] = 2
your function should return 0, since the sequences are already increasing.

Assume that:

N is an integer within the range [2..100,000];
each element of arrays A, B is an integer within the range [-1,000,000,000..1,000,000,000];
A and B have equal lengths.
Complexity:

expected worst-case time complexity is O(N);
expected worst-case space complexity is O(N), beyond input storage (not counting the storage required for input arguments).
*/
/*
Ideas
Impossible cases
1. Find max (A, B) , if its index is not N-1
2. Find minimum (A,B) - if it is not with index 0
3. If for some k:  A(k), B(k) both > A(k+1) or B(k+1): (A[k] > A[k+1] && B[k] > A[k+1]) || (A[k] > B[k+1] && B[k] > B[k+1])

*/

#include "stdafx.h"

////////////////////////////////////////////////////////////////   START ////////////////////////////////////////////////////////////////
#include <iostream>     // std::cout
#include <algorithm>    // std::min_element, std::max_element
#include <vector>
#include <iterator> // for ostream_iterator
#include <numeric>
using namespace std;
template<class T> void printVector(const vector<T>& res);
//silver solution https://app.codility.com/cert/view/certCV3D93-S3TUX2BJ8MNF9XMK/
int solutionSilver(vector<int> &A, vector<int> &B) {
	int N = A.size();
	vector<int> mins, maxs;
	for (int i = 0; i < N; i++) {
		mins.push_back(min(A.at(i), B.at(i)));
		maxs.push_back(max(A.at(i), B.at(i)));
	}

	for (int i = 1; i < N; i++) {
		if (maxs.at(i - 1) >= maxs.at(i))  //one of the A[i] or B[i] will be higher or equal then the subsequent
			return -1;
		if (mins.at(i - 1) >= mins.at(i))  //one of the A[i] or B[i] will be higher or equal then the subsequent
			return -1;
	}

	for (int i = 1; i < N; i++) {
		if (maxs.at(i - 1) < mins.at(i)) {   //here we can cut the problem in two smaller, because any exchanges of A[i-1]<->B[i-1], A[i]<->B[i] do not cross the line
			vector<int> A1(A.begin(), A.begin() + i);
			vector<int> B1(B.begin(), B.begin() + i);
			vector<int> A2(A.begin() + i, A.end());
			vector<int> B2(B.begin() + i, B.end());
			return solutionSilver(A1, B1) + solutionSilver(A2, B2);  //the problem is restricted to two smaller problems
		}
	}

	int counterBiggerA = 0;
	for (int i = 0; i < N; i++) {
		if (A.at(i) > B.at(i))
			counterBiggerA++;   
	}

	return min(counterBiggerA, N - counterBiggerA); // if bigger A is a minority, exchange bigger A with B to make all A smaller
	
}

//Golden solution!
// https://app.codility.com/cert/view/certP5CR6D-QC26Y3CYSQ5BDRWV/ 
int solutionHelper(vector<int> &A, vector<int> &B, int left, int right) {
	// we arrive to the place where there is the foll wing property for all i = 1,...N-1
	// mins(i-1) < mins(i) <= max(i-1) < max(i)
	//it also true that all A[i] != B[i], because it leads to impossible or cut as before
	// at this place we need to set all A > B or all A < B depending what is shorter
	if (right == left + 1)
		return 0;
	int counterBiggerA = 0;
	int N = right - left;
	for (int i = left; i < right; i++) {
		if (A.at(i) > B.at(i))
			counterBiggerA++;
	}
	return min(counterBiggerA, N - counterBiggerA); //if bigger A is a minority, exchange bigger A with B to make all A smaller or to the opposite
}
int solution(vector<int> &A, vector<int> &B) {
	int N = A.size();
	vector<int> mins, maxs;
	for (int i = 0; i < N; i++) {
		mins.push_back(min(A.at(i), B.at(i)));
		maxs.push_back(max(A.at(i), B.at(i)));
	}

	for (int i = 1; i < N; i++) {
		if (maxs.at(i - 1) >= maxs.at(i))  //one of the A[i] or B[i] will be higher or equal then the subsequent
			return -1;
		if (mins.at(i - 1) >= mins.at(i))  //one of the A[i] or B[i] will be higher or equal then the subsequent
			return -1;
	}

	vector <int> leftEnd, rightEnd;
	int currentLeftEnd = 0;

	for (int i = 1; i < N; i++) {
		if (maxs.at(i - 1) < mins.at(i)) {   //here we can cut the problem in two smaller, because any exchanges of A[i-1]<->B[i-1], A[i]<->B[i] do not cross the line
			leftEnd.push_back(currentLeftEnd);
			rightEnd.push_back(i);
			currentLeftEnd = i;
		}
	}
	leftEnd.push_back(currentLeftEnd);
	rightEnd.push_back(N);

	int res = 0;
	int cutsNum = (int)leftEnd.size();

	for (int i = 0; i < cutsNum; i++) {
		res += solutionHelper(A, B, leftEnd.at(i), rightEnd.at(i));
	}
	return res;
}
////////////////////////////////////////////////////////////////   END  ////////////////////////////////////////////////////////////////////
void checkPrintOld(vector<int> &A, vector<int> &B, int expected  = -1) {
	int sol = solution(A, B);
	if (sol != expected) {
		printVector<int>(A);
		printVector<int>(B);
		cout << "Obtained: " << sol << " Expected: " << expected << endl;
	}
}

void checkPrint(vector<int> &A, vector<int> &B, int expected = -1) {
	int sol = solution(A, B);
	expected = solutionSilver(A, B);
	if (sol != expected ) {
		printVector<int>(A);
		printVector<int>(B);
		//if (sol != expected)cout << "Error!!" << endl;
		cout << "Obtained: " << sol << " Expected: " << expected << endl;
	}
}

int main()
{
/*
1 65 111 152 214 274 338 316 433
0 64 70 139 231 310 311 430 370
Obtained: 0 Expected: 1
0 43 50 222 287 148
1 92 181 93 142 334
Obtained: 0 Expected: 2
0 8 16 74 128 334 184
1 32 124 162 250 174 344
Obtained: 0 Expected: 1

*/
	int A10[] = {  274, 338, 316, 433 };
	int B10[] = {  310, 311, 430, 370 };
	checkPrint(vector<int>(A10, A10 + 4), vector<int>(B10, B10 + 4));
	//return 0;

	vector<int>testA, testB;
	int testN, testNLimit = 10, numLimit = 100;
	int amountTests = 10;
	for (int i = 0; i < amountTests; i++) {
		testN = 1 + rand() % testNLimit;
		testA.resize(0);
		testB.resize(0);
		int currentA = 1;
		int currentB = 0;
		for (int j = 0; j < testN; j++) {
			testA.push_back(currentA);
			currentA += rand() % numLimit + 1;
			testB.push_back(currentB);
			currentB += rand() % numLimit + 1;
		}
		for (int i = 0; i < testN; i++) {
			int sw = testA.at(i);
			if (0 == rand() % 2) {
				testA.at(i) = testB.at(i);
				testB.at(i) = sw;
			}
		}
		checkPrint(testA, testB); //these tests always give -1
	}
	return 0;
	for (int i = 0; i < amountTests; i++) {
		testN = 1 + rand() % testNLimit;
		testA.resize(0);
		testB.resize(0);
		for (int j = 0; j < testN; j++) {
			testA.push_back(rand() % numLimit);
			testB.push_back(rand() % numLimit);
		}
		checkPrint(testA, testB); //these tests always give -1
	}


	return 0;
	int A1[] = {5,3,7,7,10};
	int B1[] = {1,6,6,9,9};
	checkPrint(vector<int>(A1, A1 + 5), vector<int>(B1, B1 + 5), 2);

	int A2[] = { 5,-3, 6, 4, 8};
	int B2[] = { 2, 6,-5, 1, 0 };
	checkPrint(vector<int>(A2, A2 + 5), vector<int>(B2, B2 + 5), -1);


	int A3[] = {  1, 5, 6 };
	int B3[] = { -2, 0, 2};
	checkPrint(vector<int>(A3, A3 + 3), vector<int>(B3, B3 + 3), 0);

	int A4[] = { 10, 5, 6 };
	int B4[] = { -2, 0, 2 };
	checkPrint(vector<int>(A4, A4 + 3), vector<int>(B4, B4 + 3), -1);

    return 0;
}
template<class T> void printVector(const vector<T>& res) {
	if (res.empty()) {
		cout << "Empty vector" << endl;
		return;
	}
	std::copy(res.begin(), res.end(), ostream_iterator<T>(cout, " "));
	cout << endl;
}

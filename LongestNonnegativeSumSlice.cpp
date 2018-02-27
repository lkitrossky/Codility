// LongestNonnegativeSumSlice.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
/*
https://app.codility.com/programmers/task/longest_nonnegative_sum_slice/
https://app.codility.com/programmers/challenges/ferrum2018/
https://app.codility.com/cert/view/certE5WJ8G-3Z7KPJXPNKUGV8MQ/   silver by caterpillar, 81% scalability problem
https://app.codility.com/cert/view/cert78Y9HB-N7H6FRHHD4QQA9ZV/   silver by caterpillar, 81% scalability problem
Codility confirms that a person who identified to us on-line as:
Levi Kitrossky was awarded
Codility Silver Award for the Ferrum 2018 Challenge
This is the 19th Codility Silver Award granted this month.
Review detailed assessment
AWARD TYPE:Codility Silver Award (Ferrum 2018)
EXPIRATION DATE:2020-01-20 17:00:00 GMT
CHALLENGE POSTED:2018-01-20 17:00:00 GMT
CHALLENGE SOLVED:2018-01-22 23:39:02 GMT
TIME SINCE POSTED:2 days
ASSESSMENT:correct functionality, problems with scalability

*/
//////////////////////////////////////////////////////////////
#include<vector>
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;
template<class T> void printVector(const vector<T>& res);

int solutionSilver(vector<int> &A) {
	int res = 0;
	int N = A.size();
	vector<int>sums(N,0);
	sums.at(0) = A.at(0);
	for (int i = 1; i < N; i++) {
		sums.at(i) = sums.at(i - 1) + A.at(i);
	}
	//caterpillar
	int left = 0, right = 0;
	int returnToRight = left;
	while (left < N) {
		right = max(returnToRight, left + res - 1);
		while (right < N) {
			int s = sums.at(right) - sums.at(left) + A.at(left);
			if (s >= 0) {
				returnToRight = right;
				res = std::max(res, right - left + 1);
			}
			right++;
		}
		left++;
	}
	return res;
}
/*
Performance seemingly OK, bu tsome results are wrong
sorted_asc
array with negative sum, sorted ascendingly
got 0 expected 20

tricky_sequence
array in form [1]*a + [-1]*b + [1]*c, a + c = b 
got 10 expected 20


*/
int solution(vector<int> &A) {
	int res = 0;
	int N = A.size();
	vector<int>sums(N, 0);
	sums.at(0) = A.at(0);
	for (int i = 1; i < N; i++) {
		sums.at(i) = sums.at(i - 1) + A.at(i);
	}
	vector<int> leftMaxV;

/*
	leftMaxV.push_back(0);
	for (int i = 1; i < N; i++) {
		//if (sums.at(i) > sums.at(leftMaxV.back()))  //with this commented out correct but not scalable enough. 90% that is increase
		if (sums.at(i) > sums.at(leftMaxV.back()))  
			leftMaxV.push_back(i);
	}
*/
	/*
	
	tricky_sequence failure and also time performance
array in form [1]*a + [-1]*b + [1]*c, a + c = b  WRONG ANSWER 
got 10 expected 20
	
	*/

	leftMaxV.resize(0);
	leftMaxV.push_back(0);
	int leftMi = sums.at(0);
	for (int i = 1; i < N; i++) {
		//fails with 
		if (sums.at(i-1) < leftMi) {
			leftMaxV.push_back(i);
			leftMi = sums.at(i - 1);
		}
	}


	vector<int> rightMaxV;   
	rightMaxV.push_back(N-1);
	for (int i = N-2; i >= 0; i--) {
		if (sums.at(i) > sums.at(rightMaxV.back()))
			rightMaxV.push_back(i);
	}
	
	//printVector<int>(sums);
	//printVector<int>(leftMaxV);
	//printVector<int>(rightMaxV);
	

	for (auto aul = leftMaxV.begin(); aul != leftMaxV.end(); aul++) {
		for (auto aur = rightMaxV.begin(); aur != rightMaxV.end(); aur++) {
			int dist = *aur - *aul + 1;
			if (dist <= res)
				break;
			int s = sums.at(*aur) - sums.at(*aul) + A.at(*aul);
			if (s >= 0) {
				res = dist;
			}
		}
	}
	return res;
}
//////////////////////////////////////////////////////////////////////////////////////////
template<class T> void printVector(const vector<T>& res) {
	if (res.empty()) {
		cout << "Empty vector" << endl;
		return;
	}
	std::copy(res.begin(), res.end(), ostream_iterator<T>(cout, " "));
	cout << endl;
}
void check(vector<int> &A) {
	int solSilver = solutionSilver(A);
	int solNew = solution(A);
	if (solNew != solSilver) {
		cout << "ERROR!" << endl;
		printVector<int>(A);
		cout << "Solution new: " << solNew << endl;
		cout << "Solution silver: " << solSilver << endl;
	}
}
int main()
{
	int G[] = { -1, 1, - 1, 0 };
	vector<int> GV(G, G + sizeof(G) / sizeof(int));
	//check(GV);
	//return 0;
	int G1[] = { -1, - 1, - 1, 1 };
	vector<int> G1V(G1, G1 + sizeof(G1) / sizeof(int));
	check(G1V);
	//return 0;
	int scale = 4;
	int attemps = 200;
	for (int j = 0; j < attemps; j++) {
		vector<int> FV;
		for (int i = 0; i < scale; i++)
			FV.push_back(rand() % 3 - 1);
		check(FV);
	}
	//return 0;
/*
ERROR!
-1 1 -1 0
Solution new: 2
Solution silver: 3
ERROR!
-1 1 -1 0
Solution new: 2
Solution silver: 3
Press any key to continue . . .

ERROR!
-1 1 -1 1 -1 0
Solution new: 4
Solution silver: 5
ERROR!
0 -1 1 -1 0 0
Solution new: 3
Solution silver: 4
Press any key to continue . . .


ERROR!
0 -1 0 -1 1 -1 0 0 0 0
Solution new: 4
Solution silver: 6
ERROR!
-1 1 0 0 1 0 -1 -1 0 -1
Solution new: 7
Solution silver: 8
ERROR!
-1 1 1 -1 1 -1 0 -1 0 0
Solution new: 7
Solution silver: 9


ERROR!
-1 0 0 -1 -1 1 1 1 -1 1 0 0 -1 0 0 -1 0 1 1 1 -1 0 -1 0 1 -1 1 -1 0 -1 0 -1 1 1 1 0 -1 1 -1 1 -1 0 1 0 1 1 1 0 -1 0 1 1 1 1 -1 0 1 0 -1 -1 0 0 -1 -1 -1 0 -1 1 1 1 1 -1 0 0 -1 0 -1 0 -1 -1 1 -1 0 0 1 -1 1 0 -1 1 -1 1 0 0 1 0 -1 -1 0 -1 -1 0 0 0 0 -1 -1 0 1 -1 -1 1 1 0 -1 1 0 0 -1 -1 1 -1 1 -1 0 1 -1 0 -1 1 0 -1 -1 0 1 -1 -1 0 0 1 1 1 1 1 1 -1 1 0 1 1 1 1 -1 -1 0 1 0 -1 0 -1 -1 0 1 0 0 -1 -1 -1 1 1 1 1 1 -1 0 -1 -1 -1 -1 0 1 -1 -1 0 0 1 1 1 1 -1 1 1 0 -1 1 -1 0 -1 0 0
Solution new: 194
Solution silver: 195
ERROR!
-1 1 1 -1 1 -1 0 -1 0 0 -1 1 0 0 -1 -1 1 1 0 0 -1 1 0 0 1 1 -1 1 1 -1 1 1 -1 0 1 1 0 -1 0 -1 0 0 0 -1 1 0 -1 1 0 -1 -1 0 1 0 0 1 -1 -1 1 -1 -1 0 -1 0 0 -1 0 0 -1 -1 1 -1 0 1 -1 -1 0 1 1 -1 0 -1 1 0 0 0 0 1 -1 0 1 1 1 -1 1 0 -1 1 0 -1 0 -1 0 -1 1 -1 1 1 -1 0 1 -1 -1 1 0 -1 -1 0 1 1 -1 1 1 -1 1 0 1 1 -1 -1 0 0 1 0 -1 -1 0 -1 0 -1 -1 1 -1 -1 -1 -1 -1 -1 1 1 1 0 1 0 0 -1 0 0 0 0 0 1 0 1 -1 1 0 -1 0 -1 0 -1 1 0 0 -1 0 1 0 -1 0 0 1 -1 1 0 1 -1 1 1 0 1 -1 -1 -1 0 -1 1 -1 -1
Solution new: 120
Solution silver: 127

*/

/*

ERROR!
-1 1 1 -1 1 -1 0 -1 0 0 -1 1 0 0 -1 -1 1 1 0 0 -1 1 0 0 1 1 -1 1 1 -1 1 1 -1 0 1 1 0 -1 0 -1 0 0 0 -1 1 0 -1 1 0 -1 -1 0 1 0 0 1 -1 -1 1 -1 -1 0 -1 0 0 -1 0 0 -1 -1 1 -1 0 1 -1 -1 0 1 1 -1 0 -1 1 0 0 0 0 1 -1 0 1 1 1 -1 1 0 -1 1 0 -1
Solution new: 84
Solution silver: 88
ERROR!
-1 -1 -1 -1 0 -1 1 0 0 -1 -1 0 0 -1 -1 0 0 1 -1 1 -1 0 0 1 0 0 1 -1 -1 0 0 1 0 -1 -1 -1 -1 -1 1 0 -1 0 1 -1 -1 1 -1 -1 -1 1 -1 -1 1 0 1 1 1 0 1 0 -1 0 -1 0 0 1 0 1 -1 1 0 1 -1 -1 1 1 -1 -1 0 0 0 0 0 -1 1 -1 0 -1 -1 -1 -1 0 1 -1 -1 0 -1 0 0 1
Solution new: 44
Solution silver: 47
ERROR!
1 -1 0 -1 1 -1 -1 -1 -1 0 1 0 0 0 -1 0 1 1 -1 1 0 1 -1 1 0 1 -1 1 1 0 -1 -1 1 1 1 1 0 0 -1 -1 1 0 0 0 0 -1 -1 1 -1 1 -1 0 0 0 1 0 -1 1 -1 0 1 -1 0 -1 0 -1 0 1 1 -1 1 -1 0 -1 -1 0 1 0 0 -1 0 -1 0 0 1 0 -1 0 -1 0 -1 0 1 1 0 1 1 1 0 -1
Solution new: 93
Solution silver: 95
ERROR!
-1 0 1 -1 -1 0 1 -1 0 1 -1 0 0 1 -1 -1 -1 -1 1 -1 0 -1 0 0 -1 0 0 0 1 1 1 1 1 0 0 -1 0 1 1 1 -1 -1 0 -1 -1 -1 1 -1 1 1 1 0 -1 0 -1 1 -1 0 1 0 0 1 1 -1 -1 0 1 -1 1 0 -1 -1 -1 -1 0 0 -1 1 1 -1 1 -1 1 -1 1 0 -1 -1 0 -1 -1 0 -1 0 1 -1 -1 -1 1 1
Solution new: 66
Solution silver: 68
ERROR!
-1 0 1 -1 0 -1 0 0 -1 -1 -1 -1 1 0 -1 1 1 1 0 -1 0 0 0 -1 0 -1 1 1 0 -1 1 0 1 -1 -1 0 -1 -1 1 1 1 0 -1 1 -1 -1 -1 0 1 0 -1 -1 1 1 1 0 0 0 -1 1 -1 -1 0 0 -1 -1 -1 -1 -1 1 1 1 -1 1 -1 1 1 -1 0 1 1 -1 1 -1 0 1 1 1 -1 -1 -1 0 1 1 1 -1 1 -1 0 0
Solution new: 86
Solution silver: 88
ERROR!
0 -1 1 1 0 0 -1 1 -1 0 1 -1 0 -1 -1 1 0 1 -1 0 0 1 0 0 0 0 0 -1 1 0 0 -1 1 1 -1 0 0 1 -1 -1 1 -1 -1 0 -1 -1 0 0 0 1 1 -1 0 -1 -1 -1 0 0 -1 0 0 1 -1 -1 -1 0 1 0 -1 1 1 0 -1 0 1 1 0 0 -1 0 1 -1 0 0 0 1 -1 1 0 -1 0 0 1 1 1 -1 1 1 0 1
Solution new: 57
Solution silver: 85
ERROR!
1 0 0 1 -1 0 -1 -1 -1 1 -1 1 0 -1 0 -1 0 0 -1 -1 1 0 1 -1 1 -1 1 0 -1 0 0 -1 0 1 -1 0 1 0 1 0 1 1 -1 1 -1 -1 -1 1 -1 0 1 0 1 -1 0 -1 1 0 1 0 -1 1 1 -1 1 -1 -1 -1 0 -1 -1 0 0 -1 1 -1 -1 -1 -1 1 1 1 0 0 1 0 1 -1 0 1 1 0 -1 1 1 0 1 1 -1 0
Solution new: 90
Solution silver: 91
ERROR!
-1 1 -1 1 1 0 1 -1 0 0 0 0 -1 0 1 -1 1 -1 0 1 -1 0 -1 0 1 1 0 0 1 -1 -1 1 0 -1 -1 -1 0 -1 -1 0 0 0 -1 1 0 1 0 1 1 0 1 -1 -1 -1 -1 0 1 1 1 -1 0 -1 -1 1 -1 0 0 -1 1 1 -1 0 -1 0 0 0 0 1 0 1 -1 -1 -1 -1 -1 0 -1 -1 1 -1 1 0 1 0 1 -1 -1 -1 -1 0
Solution new: 51
Solution silver: 58
ERROR!
-1 0 -1 -1 1 -1 -1 1 0 1 1 0 -1 -1 0 0 0 0 0 1 0 1 0 1 1 -1 -1 0 -1 -1 1 0 -1 0 0 -1 0 0 0 -1 0 1 -1 0 0 -1 -1 0 -1 -1 -1 -1 1 0 0 0 -1 -1 0 -1 -1 -1 1 0 -1 0 0 0 1 -1 -1 0 1 1 0 1 0 -1 0 0 -1 1 0 -1 -1 0 -1 -1 1 0 -1 -1 0 -1 0 1 0 -1 1 -1
Solution new: 29
Solution silver: 35
ERROR!
-1 -1 1 -1 1 -1 -1 1 -1 0 -1 -1 1 1 0 0 1 1 0 1 1 1 0 0 -1 1 -1 1 1 -1 -1 1 1 -1 0 1 -1 -1 0 -1 -1 -1 1 1 -1 1 -1 0 0 -1 -1 1 0 1 1 0 0 -1 0 0 -1 1 -1 0 1 1 -1 -1 0 -1 1 -1 -1 -1 0 -1 -1 0 -1 0 1 -1 -1 -1 0 0 1 -1 -1 0 -1 -1 1 1 -1 1 -1 0 0 -1
Solution new: 66
Solution silver: 69
ERROR!
1 1 0 1 -1 -1 0 0 -1 1 0 1 0 0 0 1 -1 -1 -1 -1 1 -1 1 0 0 0 0 -1 1 -1 -1 1 -1 -1 -1 0 0 1 1 -1 0 1 1 1 -1 0 0 0 1 -1 0 -1 -1 0 1 0 -1 1 1 -1 1 -1 -1 0 1 0 0 -1 1 0 1 -1 -1 0 -1 1 0 -1 -1 0 -1 0 1 -1 0 -1 1 -1 1 1 0 -1 -1 0 -1 0 1 0 0 0
Solution new: 49
Solution silver: 51
ERROR!
1 1 -1 0 1 1 -1 0 0 0 -1 0 -1 0 0 0 1 1 1 -1 0 -1 0 0 -1 -1 -1 1 1 1 0 0 1 -1 -1 1 -1 1 1 0 -1 -1 -1 1 0 0 0 -1 -1 1 -1 0 -1 1 -1 0 0 1 -1 1 1 0 1 0 -1 1 0 0 -1 0 -1 0 1 -1 -1 1 -1 -1 1 1 -1 -1 -1 0 1 1 1 1 -1 1 -1 0 -1 -1 0 -1 1 1 0 1
Solution new: 68
Solution silver: 73
ERROR!
1 -1 0 -1 -1 1 1 -1 -1 0 -1 1 0 -1 0 -1 1 -1 1 0 1 -1 -1 0 0 1 -1 1 1 -1 -1 0 -1 -1 1 0 0 1 -1 -1 0 0 0 -1 -1 -1 -1 -1 1 -1 0 0 0 0 -1 1 0 1 1 1 1 1 -1 -1 0 1 0 -1 0 1 1 0 -1 0 1 0 1 1 -1 0 -1 1 -1 -1 1 0 1 1 -1 0 0 0 1 -1 0 -1 0 -1 1 -1
Solution new: 70
Solution silver: 77
ERROR!
0 -1 0 1 0 -1 -1 0 -1 1 -1 1 0 1 1 0 0 -1 1 -1 0 -1 0 0 -1 -1 1 1 0 -1 0 0 -1 0 0 -1 0 -1 -1 1 0 -1 1 -1 0 1 1 1 1 0 1 -1 1 0 0 1 1 0 -1 1 -1 0 1 1 -1 1 0 0 0 -1 1 -1 1 -1 0 1 0 0 -1 -1 1 -1 -1 -1 1 0 0 0 1 0 -1 -1 0 -1 -1 1 1 0 -1 1
Solution new: 88
Solution silver: 91
ERROR!
-1 1 0 -1 0 1 1 -1 -1 -1 1 1 1 1 0 -1 0 0 -1 -1 1 1 -1 -1 -1 -1 1 0 0 1 1 1 -1 1 1 0 0 -1 1 0 0 1 -1 -1 -1 0 1 0 0 0 -1 0 0 -1 -1 -1 1 -1 1 -1 1 1 -1 -1 -1 -1 -1 1 0 -1 -1 0 0 -1 -1 -1 -1 -1 0 -1 0 0 0 0 -1 1 -1 0 1 1 -1 0 -1 -1 0 1 0 1 0 1
Solution new: 58
Solution silver: 61
Press any key to continue . . .


*/
	scale = 100;
	attemps = 100;
	for (int j = 0; j < attemps; j++) {
		vector<int> FV;
		for (int i = 0; i < scale; i++)
			FV.push_back(rand() % 3 - 1);
		check(FV);
	}
	return 0;
	int D[] = { 1, -1, -1, 1 };
	vector<int> DV(D, D + sizeof(D) / sizeof(int));
	check(DV);

	vector<int> EV;
	scale = 100000;
	for (int i = 0; i < scale; i++)
		EV.push_back(1);
	for (int i = 0; i < 2* scale; i++)
		EV.push_back(-1);
	for (int i = 0; i < scale; i++)
		EV.push_back(1);
	check(EV);

	return 0;

	int C[] = { -1, -1, -1, -1, -1, 0,0,0, 1 };
	vector<int> CV(C, C + sizeof(C) / sizeof(int));
	check(CV);
	return 0;

	int A[] = {-1,-1, 1,-1, 1, 0, 1, -1, -1 };
	vector<int> AV(A, A + 9);
	printVector<int> (AV);
	cout << "Solution new: " << solution(AV) << endl;
	cout << "Solution silver: " << solutionSilver(AV) << endl;
	int B[] = { 1, 1, -1, -1, -1, -1, -1, 1, 1 };
	vector<int> BV(B, B + 9);
	printVector<int>(BV);
	cout << "Solution new: " << solution(BV) << endl;
	cout << "Solution silver: " << solutionSilver(BV) << endl;
	check(BV);


    return 0;
}


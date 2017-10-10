#include "stdafx.h"

// SlalomSkiing.cpp : Defines the entry point for the console application.
//
/*
https://codility.com/programmers/lessons/90-tasks_from_indeed_prime_2015_challenge/slalom_skiing/
Lesson 90 of challenges
Ambitious

The main idea is to deal in two parts.
Part 1
Movement in one direction means finding the length of the longest increasing sequence inside of array of integers.
Keep all the time the ordered list of final point of sequences with the accumulated value.
The order is by number only. A new number is added with the accumulated of the immediately previous incremented by one, if none previous: take 0 + 1
If the next is the same - eliminate it.
Sample:

10(4)                10(4)                      10(4)  10(4)                      11(5)
8(2)  add 6 =>  8(2)    add 7 =>     8(2) => 7(2)   add 11 =>   10(4)
6(1)                     7(2)    6(1)                7(2)
6(1)                        6(1)
This operation is O(n*log(n))

Part 2
Add turns
The same but keep three arrays, the second works with mirrored values of the first (right barrier - values). Each member has accumulation of its previous OR the previous of the fist array (maximum).
The third has the same values but check maximum of the seond or own


*/




//////////////////////////////////////// start
#include <vector>
#include <iostream>
#include <iterator> // for ostream_iterator
#include <algorithm>
using namespace std;


bool pred(const pair<long long, int> &a, const pair<long long, int> &b) { return a.first < b.first; }

/*
https://codility.com/demo/results/trainingJQ33YP-55K/
the first solution is correct functional but performance is 20%, overall 69%
Detected time complexity:
O(N**2) as above for solution2
*/

/*

https://codility.com/demo/results/trainingDN55AN-AAM/
100%   The improvement is in findLISBetter towards findLIS. It is a known in the field difference in search of the max length of increasing subsequence
See
https://stackoverflow.com/questions/2631726/how-to-determine-the-longest-increasing-subsequence-using-dynamic-programming
https://en.wikipedia.org/wiki/Longest_increasing_subsequence#Efficient_algorithms


*/
vector<long long> prepareArray(vector<int> &AI) {
	/*
	The idea is that instead of turns we just reflect symmetrically the field, so the movement after the turn is the moving in the reflection
	The axis of reflection is the right barrier for the first turn plus 1 or any posiive number or its mirror image again for the right turn
	The only problem that gates cannot be on the same index, so we first triple index and use remainders 0,1,2 to put gates the rightest, the middle and the leftest
	This order ensures that we do not return after the turn
	*/
	const static long long limit = 1000000000 + 1;
	int N = AI.size();
	vector<long long>AL(3 * N);
	for (int i = 0; i < N; i++) {
		long long t1 = 3 * (long long)AI.at(i);
		long long t2 = 2 * 3 * limit - t1;
		long long t3 = 4 * 3 * limit - t2;
		AL.at(3 * i + 0) = t3;
		AL.at(3 * i + 1) = t2;
		AL.at(3 * i + 2) = t1;
	}
	return AL;
}
int findLISBetter(vector<long long>AL) {
	vector< long long> res1;
	for (auto au = AL.begin(); au != AL.end(); au++) {
		auto po1 = upper_bound(res1.begin(), res1.end(), *au);
		if (res1.end() == po1) {
			res1.push_back(*au);
		}
		else
			*po1 = *au;
	}
	return res1.size();
}
int findLIS(vector<long long>AL) {
	vector< pair<long long, int> > res1;
	for (auto au = AL.begin(); au != AL.end(); au++) {
		pair<long long, int> np1(*au, 1);
		auto po1 = upper_bound(res1.begin(), res1.end(), np1, pred);
		if (!res1.empty() && po1 != res1.begin())
			np1.second = (po1 - 1)->second + 1;
		po1 = res1.insert(po1, np1);
		if (!res1.empty() && po1 != res1.end() && (po1 + 1) != res1.end()) {
			if (po1->second >= (po1 + 1)->second)
				res1.erase(po1 + 1);
		}
	}
	return res1.back().second;
}
int solution(vector<int> &AI) {
	vector<long long>AL = prepareArray(AI);
	int res = findLISBetter(AL);
	return res;
}


///////////////////////////////////////  end

ostream& operator<<(ostream& os, pair<int, int>  &p) {
	os << p.first << "\t" << p.second << endl;
	return os;
}
ostream& operator<<(ostream& os, pair<long long, int>  &p) {
	os << p.first << "\t" << p.second << endl;
	return os;
}
template<class T> void printVector(const vector<T>& res1);
int solution2(vector<int> &AI);
int main()
{
	int A[13];
	A[0] = 15;
	A[1] = 13;
	A[2] = 5;
	A[3] = 7;
	A[4] = 4;
	A[5] = 10;
	A[6] = 12;
	A[7] = 8;
	A[8] = 2;
	A[9] = 11;
	A[10] = 6;
	A[11] = 9;
	A[12] = 3;
	vector<int> VA(A, A + 13);
	//printVector<int>(VA);
	cout << "Solution must be 8, currently they are: " << solution(VA) << " (new) " << solution2(VA) << " (old) " << endl;
	VA.resize(2);
	VA.at(0) = 1;
	VA.at(1) = 5;
	//printVector<int>(VA);
	cout << "Solution must be 2, currently they are: " << solution(VA) << " (new) " << solution2(VA) << " (old) " << endl;
	return 0;
}

template<class T> void printVector(const vector<T>& res1) {
	if (res1.empty()) {
		cout << "Empty vector" << endl;
		return;
	}
	std::copy(res1.begin(), res1.end(), ostream_iterator<T>(cout, " "));
	cout << endl;
}
/*

solution2
https://codility.com/demo/results/trainingX47836-T56/
the first solution is correct functional but performance is 20%, overall 69%
Detected time complexity:
O(N**2)

A  medium_random
long subsequence hidden in random array, N <= 250 V OK
A  huge_random
long subsequence hidden in random array, N <= 100,000 X TIMEOUT ERROR
running time: 2.23 sec., time limit: 0.35 sec.
A  huge_monotonic
huge monotonic sequence with additional random elements, N <= 100,000 X TIMEOUT ERROR
running time: 0.52 sec., time limit: 0.34 sec.
A  huge_shuffled_monotonic
shuffled huge monotonic sequences, N <= 100,000 X TIMEOUT ERROR
running time: 0.38 sec., time limit: 0.26 sec.
A  huge_concatenated_monotonic
concatenated huge monotonic sequences, N <= 100,000 X TIMEOUT ERROR
running time: 2.81 sec., time limit: 0.34 sec.


*/
int solution2(vector<int> &AI) {
	const static long long limit = 1000000000 + 1;
	//const static long long limit = 19 + 1;
	int N = AI.size();
	vector<long long>AL(N);
	for (int i = 0; i < N; i++) {
		AL.at(i) = (long long)AI.at(i);
	}
	vector< pair<long long, int> > res1, res2, res3;
	for (auto au = AL.begin(); au != AL.end(); au++) {
		pair<long long, int> np1(*au, 1);
		pair<long long, int> np2(2 * limit - np1.first, 1);
		pair<long long, int> np3(4 * limit - np2.first, 1);
		auto po1 = upper_bound(res1.begin(), res1.end(), np1, pred);
		auto po2 = upper_bound(res2.begin(), res2.end(), np2, pred);
		auto po3 = upper_bound(res3.begin(), res3.end(), np3, pred);
		int accumlated1 = res1.empty() ? 0 : res1.back().second;
		int accumlated2 = res2.empty() ? 0 : res2.back().second;
		if (!res1.empty() && po1 != res1.begin())
			np1.second = (po1 - 1)->second + 1;
		po1 = res1.insert(po1, np1);
		if (!res1.empty() && po1 != res1.end() && (po1 + 1) != res1.end()) {
			if (po1->second >= (po1 + 1)->second)
				res1.erase(po1 + 1);
		}
		if (AL.begin() != au) {
			if (!res2.empty() && po2 != res2.begin())
				np2.second = (po2 - 1)->second + 1;
			np2.second = max(accumlated1 + 1, np2.second);
			po2 = res2.insert(po2, np2);
			if (!res2.empty() && po2 != res2.end() && (po2 + 1) != res2.end()) {
				auto po2a = po2 + 1;
				while (po2a != res2.end()) {
					if (po2->second >= po2a->second) {
						res2.erase(po2a);
						po2a = po2 + 1;
					}
					else {
						po2a++;
					}

				}
			}
		}
		if ((AL.begin() + 1) < au) {
			if (!res3.empty() && po3 != res3.begin())
				np3.second = (po3 - 1)->second + 1;
			np3.second = max(accumlated2 + 1, np3.second);
			po3 = res3.insert(po3, np3);
			if (!res3.empty() && po3 != res3.end() && (po3 + 1) != res3.end()) {
				auto po3a = po3 + 1;
				while (po3a != res3.end()) {
					if (po3->second >= po3a->second) {
						res3.erase(po3a);
						po3a = po3 + 1;
					}
					else {
						po3a++;
					}

				}
			}
		}
	}
	//for (auto au = res1.begin(); au != res1.end(); au++)
	//     cout << *au << endl;
	//cout <<"Max gates: "<< res1.back().second << endl;
	if (!res3.empty() && !res2.empty() && !res1.empty())
		return max(res1.back().second, max(res2.back().second, res3.back().second));
	if (!res2.empty() && !res1.empty())
		return max(res1.back().second, res2.back().second);
	return res1.back().second;
}

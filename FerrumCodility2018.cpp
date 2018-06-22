/*
Codility confirms that a person who identified to us on-line as: Levi Kitrossky
was awarded Codility Golden Award for the Ferrum 2018 Challenge
This is the 57th Codility Golden Award granted this month.
*/
// https://app.codility.com/cert/view/certV5W2CV-RPMJJ5X36C88BZ88/
// 
//////////////////////////////////////////////////////////////
#include<vector>
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;
template<class T> void printVector(const vector<T>& res);

int solution(vector<int> &A) {
	int res = 0;
	int N = A.size();
/*
The idea
1. What can be the left end of the sequence? If 1 it to its left, then we can expand it to the left side. The same if 1,1 or 1,0 or even -1,1. 
More exact if some fragment with non-negative sum is adjacent from left. So, any left end with such a neighbour is not a candidate to the left end. 
Translating it into math it means there is no partial sum 0...k to the left of it, even non-adjacent. More strict, let us for each n find partial sum
value(0), ..., value(n-1) (exclusive to left). For 0th it is 0, the name is exclusiveLeftSum. 
So, the candidates for left end compose a subsequence with monotonous strict decrease of exclusiveLeftSum. Since the first is 0, all of functions
will be negative.
2. Similarly for the right end we need to take for every n and inclusive sum value(0)+..+value(n) . All candidates for the right end must have a decreasing
inclusiveLeftSum  towarsd the last one which is the sum of all. 
3. For every fragment starting with l and ending with r inclusive, it is sum is weight(l,r) = inclusiveLeftSum(r) - exclusiveLeftSum(l)
4. NOw pay attention that both values of candidates for left and righ end decreasing. 
5. Since they are sorted we can go from 0 to the last in left candidate and every time find a right candidate with bigger index and bigger value. 
Substitute result by weight(l,r) if it is bigger.
Pay attention that both lists of candidates are not empty 0 and N-1 are present in at least one list
*/	
	vector<int> inclusiveLeftSum(N,0), exclusiveLeftSum(N,0);
	inclusiveLeftSum.at(0) = A.at(0);
	exclusiveLeftSum.at(0) = 0;
	for (int i = 1; i < N; i++) {
		exclusiveLeftSum.at(i) = exclusiveLeftSum.at(i - 1) + A.at(i - 1);
		inclusiveLeftSum.at(i) = inclusiveLeftSum.at(i - 1) + A.at(i);
	}
	/*
	cout << "Input" << endl;
	printVector<int>(A);
	cout << "Inclusive:" << endl;
	printVector<int>(inclusiveLeftSum);
	cout << "Exclusive:" << endl; 
	printVector<int>(exclusiveLeftSum);
	*/
	vector<int> leftCandidates;
	leftCandidates.push_back(0);
	for (int i = 1; i < N; i++) {
		if (exclusiveLeftSum.at(i) < exclusiveLeftSum.at(leftCandidates.back()))
			leftCandidates.push_back(i);
	}
	vector<int> rightCandidates;
	rightCandidates.push_back(N - 1);
	for (int i =  N - 2; i >= 0; i--) {
		if (inclusiveLeftSum.at(i) > inclusiveLeftSum.at(rightCandidates.front()))
			rightCandidates.insert(rightCandidates.begin(),i);
	}
	/*
	cout << "Lefts:" << endl;
	printVector<int>(leftCandidates);
	cout << "Rights:" << endl;
	printVector<int>(rightCandidates);
	*/
	int lefts = leftCandidates.size();
	int rights = rightCandidates.size();
	int leftIndex = 0, rightIndex = 0;
	while (leftIndex < lefts && rightIndex < rights) {
		int leftReal = leftCandidates.at(leftIndex);
		int rightReal = rightCandidates.at(rightIndex);
		/*
		cout << "index in left:" << leftIndex << " Left: " << leftReal << " Index in right: " << rightIndex << " right: " << rightReal << endl;			
		cout<<	" exclusiveLeftSum: " << exclusiveLeftSum.at(leftReal) << " inclusiveLeft: " << inclusiveLeftSum.at(rightReal) << endl;
		*/
		if (rightReal < leftReal) {
			rightIndex++;
			continue;
		}
		if (inclusiveLeftSum.at(rightReal) >= exclusiveLeftSum.at(leftReal) ) {
			res = max(res, rightReal - leftReal + 1);
			rightIndex++;
		}
		else {
			leftIndex++;
		}
		//cout << "Current res: " << res << endl;
	}
	return res;
}
//////////////////////////////////////////////////////////////////////////////////////////

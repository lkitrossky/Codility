// EvenSumsGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
/*
///////////////////// IDEA     /////////////////////////
Let us designate  O - odd number,  E - even number
First list trivial cases of the first wins:

One every move one must take even: even amount of odds and any of evens. So, overall sum evenness is the same.
Even amount of odds. The first takes all. And any other move will loose because it remains even.
ALL FURTHER EXAMPLE are for odd amount of odds.
First of all exclude cases of only one number: 0,0 for odd, and NO SOLUTION for even
Then pass over all array and create vector<int> oddsPositions, evenPositions;

Of the most interest are the first even and the last even.
First deal with a case of only one odd:
E...EOE...E, where E..E only even any number from 0 to 100 000.
If the postion is symmetrical the second always makes a symmetrical move to taking evens. At the end single O remains for the first and he looses, so: NO SOLUITION
If it is not symmetrical, the first player diminishes the longest even sequence and makes the position symmetrical amd wins:
EEEEOE - EEE => EOE
EEOEEEEEEE - EEEEE => EEOEE
EEEO - EEE => O
OEEE - EEE => O

Now we seek the best as start,finish. 
If you find start2,finish2: start2<start, replace start,finish by start2,finish2
Else If you find start2,finish2: start2==start, finish2 < finish replace start,finish by start2,finish2 
Leave start, finish and continue

Overall picture is
E..EOX..XOE..E   enough to deal with the first and the last odds. 
First suppose that O is not at 0 and not at the last place, that is E..E not empty
N - all figures, OON - number of odds > 1, ON(i) - position of O number i in the original array

During the first search we try to leave the last odd only in a symmetrical position in evens.
The first search: i from 0 to the first odd inclusive, j is from the OON(ON-2) inclusive to OON(ON-1) exclusive. That is we try to leave only last O and set a symmetrical position
Trying to find i,j: i + OON(last) - j - 1  == N - 1 - OON(last).  It means that we leave symmetrical
EEOEOEOEEE is EE OEO EOEEE =>  EEEOEEE, i=2, j = 4, OON=3, N = 10, OON() = 2,4,6. So the equation gives 2+6-4-1 == 10 - 1 - 6, correct
The search is sped up: if i > start, just stop. If i = start and j > finish, go to next i

The next search deals with attempts to leave only first odd in a symmetrical position.
i starts with odd(first)+1 and the last i is odd(second)
j starts with odd(last) and goes till N-1
We seeks i, j :  O(first) = i - O(first) + N - 1 - j, in simple words amount of evens before odd(first) must be equal to sum of evens from odd(first) to i and after j
The search is sped up: if i > start, just stop. If i = start and j > finish, go to next i

Now one must check two special cases that can give better results:
If the last odd is the last number, that is O(last) == N - 1, try to improve the acquired result by 0, N - 2 leaving only the last odd number alone. It happens. My last bug towards the full solution
If the last odd is the first number, that is O(first) == 0, try to improve the acquired result by 1, N - 1 leaving only the first odd number alone. This case usually covered already
Return start, finish
https://codility.com/demo/results/trainingFD32YT-2N3/

100% all

*////////////////////////////////////////////////////////
/////////////////////  START   /////////////////////////
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
#include <string>
#include <stdio.h>
#include <string.h>


using namespace std;
template<class T> void printVector(const vector<T>& res);

string finalReturn(int answerStart, int answerFinish, int N)
{
	char answerBuffer[100];
	memset(answerBuffer, 0, sizeof answerBuffer);
	if (answerStart >= N || answerFinish >= N)
		return string("NO SOLUTION");
	snprintf(answerBuffer, sizeof answerBuffer - 1, "%d,%d", answerStart, answerFinish);
	return string(answerBuffer);
}
string solution(vector<int> &A) {
	int N = A.size();
	vector<int> oddsPositions, evenPositions;
	for (int i = 0; i < N; i++) {
		if (0 == A.at(i) % 2)
			evenPositions.push_back(i);
		else
			oddsPositions.push_back(i);
	}
	int amountEven = evenPositions.size();
	int amountOdds = oddsPositions.size();

	//trivial guard cases
	if (0 == amountOdds % 2)
		return finalReturn(0, N - 1, N);
	if (1 == amountOdds && 0 == amountEven) {
		return string("NO SOLUTION");
	}
	if (1 == amountOdds && 0 < amountEven && 1 == (N % 2) && oddsPositions.front() == (N - 1) / 2) {
		return string("NO SOLUTION"); //symmetrical case
	}

	int amountStartingEven = oddsPositions.at(0);
	int amountEndingEven = N - 1 - oddsPositions.at(0);

	//important case of one odd
	if (1 == amountOdds) {
		if (amountEndingEven > amountStartingEven)  //EEOEEE
			return finalReturn(oddsPositions.front() + 1, N - 1 - amountStartingEven, N);
		if (amountEndingEven < amountStartingEven)  //EEEOEE
			return finalReturn(0, amountStartingEven - amountEndingEven - 1, N);
		if (amountEndingEven == amountStartingEven)  //EEOEE
			return string("NO SOLUTION"); //symmetrical case again
	}
	int answerStart = N;
	int answerFinish = N;

	//odds are >= 2

	//use the last odd
	int leftEndingingEven = N - 1 - oddsPositions.back();
	int leftStartingEven = 0;
	for (int i = 0; i <= oddsPositions.front(); i++) {
		if (i > answerStart)
			break;
		for (int j = oddsPositions.at(oddsPositions.size() - 2); j < oddsPositions.back(); j++) {
			if (i == answerStart && j > answerFinish)
				break;
			if (0 == (N - (j - i) - 1) % 2)
				continue;
			leftStartingEven = i + oddsPositions.back() - j - 1;
			if (leftEndingingEven == leftStartingEven) {
				if ((i < answerStart) || (i == answerStart && j < answerFinish)) {
					answerStart = i;
					answerFinish = j;
				}
			}
		}
	}
	//use the first odd
	leftEndingingEven = 0;
	leftStartingEven = oddsPositions.front();
	for (int i = oddsPositions.front() + 1; i <= oddsPositions.at(1); i++) {
		if (i > answerStart)
			break;
		for (int j = oddsPositions.back(); j < N; j++) {
			if (i == answerStart && j > answerFinish)
				break;
			if (0 == (N - (j - i) - 1) % 2)
				continue;
			leftEndingingEven = i - oddsPositions.front() - 1 + N - 1 - j;
			if (leftEndingingEven == leftStartingEven) {
				if ((i < answerStart) || (i == answerStart && j < answerFinish)) {
					answerStart = i;
					answerFinish = j;
				}
			}
		}
	}

	//possible improvement if the sequence ends with odd - - NOT NECESSARY
	if (N - 1 == oddsPositions.back()) {  
		if ((0 < answerStart) || (0 == answerStart && N - 2 < answerFinish)) {
			answerStart = 0;
			answerFinish = N - 2;
		}
	}
	//possible improvement if the sequence starts with odd  - - NOT NECESSARY
	if (0 == oddsPositions.front()) {   
		if ((1 < answerStart) || (1 == answerStart && N - 1 < answerFinish)) {
			answerStart = 1;
			answerFinish = N - 1;
		}
	}
	return finalReturn(answerStart, answerFinish, N);
}
/////////////////////  END   /////////////////////////

void check(int caseX[], int size, string expected ) {
	vector<int> testV;
	testV = vector<int>(caseX, caseX + size);
	
	string actual2 = solution(testV);
	printVector<int>(testV);
	cout << (expected != actual2 ? "Error!! " : "") << "Expected:" << expected.c_str() << " Actual: " << actual2.c_str() << endl;
}

int main()
{
	cout << "EvenSumsGame started" << endl;
	string expected("");

	int caseError2[] = { 8, 3, 2, 2, 2, 2 };
	expected = "2,4";
	check(caseError2, sizeof(caseError2) / sizeof(int), expected);


	int caseError[] = { 4, 2, 2, 2, 1, 6, 6 };
	expected = string("0,1");
	check(caseError, sizeof(caseError) / sizeof(int), expected);

	int caseTry9[] = { 2,7,4,7,6,7,8,7,10,7,12 };
	expected = string("0,7");
	check(caseTry9, sizeof(caseTry9) / sizeof(int), expected);

	int caseTry8[] = { 7,7,7,7,7 };
	expected = string("0,3");
	check(caseTry8, sizeof(caseTry8) / sizeof(int), expected);
	                   
	int caseTry7[] = { 2,7,7,7,4,7,7,7,6,7,7,7,8 };
	expected = string("1,10");
	check(caseTry7, sizeof(caseTry7) / sizeof(int), expected);

	int caseTry6[] = { 2,7,7,7,4,7,7,7,6 };
	expected = string("0,8");
	check(caseTry6, sizeof(caseTry6) / sizeof(int), expected);

	int caseTry5[] = { 2, 7, 7, 7, 4 };
	expected = string("1,2");
	check(caseTry5, sizeof(caseTry5) / sizeof(int), expected);

	int caseTry4[] = { 2, 7, 7, 7, 4, 6, 8 };
	expected = string("2,5");
	check(caseTry4, sizeof(caseTry4) / sizeof(int), expected);

	int caseTry3[] = { 2, 4, 7, 6, 7, 7, 8 };
	expected = string("1,4");
	check(caseTry3, sizeof(caseTry3) / sizeof(int), expected);


	int caseTry2[] = { 2, 4, 7, 6, 7, 7, 8 };
	expected = string("1,4");
	check(caseTry2, sizeof(caseTry2) / sizeof(int), expected);

	int caseTry1[] = { 2, 4, 6, 7, 7, 7, 8 };
	expected = string("1,4");
	check(caseTry1, sizeof(caseTry1) / sizeof(int), expected);

	
	int case1[] = { 1,1 };

	expected = string("0,1");
	check(case1, sizeof(case1) / sizeof(int), expected);
	int case2[] = { 1 };
	expected = string("NO SOLUTION");
	check(case2, sizeof(case2) / sizeof(int), expected);
	int caseA[] = { 4, 5, 3, 7, 2 };
	expected = string("1,2");
	check(caseA, sizeof(caseA) / sizeof(int), expected);
	return 0;
}
template<class T> void printVector(const vector<T>& res) {
	if (res.empty()) {
		cout << "Empty vector" << endl;
		return;
	}
	copy(res.begin(), res.end(), ostream_iterator<T>(std::cout, " "));
	cout << endl;
}

/*
https://codility.com/programmers/task/even_sums_game/
https://codility.com/programmers/challenges/scandium2016/
Compute a winning move in a game in which you remove slices with even sums of elements.
Even sums is a game for two players. Players are given a sequence of N positive integers and take turns alternately. In each turn, a player chooses a non-empty slice (a subsequence of consecutive elements) such that the sum of values in this slice is even, then removes the slice and concatenates the remaining parts of the sequence. The first player who is unable to make a legal move loses the game.
You play this game against your opponent and you want to know if you can win, assuming both you and your opponent play optimally. You move first.
Write a function:
string solutionOld(vector<int> &A);
that, given an array A consisting of N integers, returns a string of format "X,Y" where X and Y are, respectively, the first and last positions (inclusive) of the slice that you should remove on your first move in order to win, assuming you have a winning strategy. If there is more than one such winning slice, the function should return the one with the smallest value of X. If there is more than one slice with the smallest value of X, the function should return the shortest. If you do not have a winning strategy, the function should return "NO SOLUTION".
For example, given the following array:
A[0] = 4 A[1] = 5 A[2] = 3 A[3] = 7 A[4] = 2
the function should return "1,2". After removing a slice from positions 1 to 2 (with an even sum of 5 + 3 = 8), the remaining array is [4, 7, 2]. Then the opponent will be able to remove the first element (of even sum 4) or the last element (of even sum 2). Afterwards you can make a move that leaves the array containing just [7], so your opponent will not have a legal move and will lose. One of possible games is shown on the following picture:

Note that removing slice "2,3" (with an even sum of 3 + 7 = 10) is also a winning move, but slice "1,2" has a smaller value of X.
For the following array:
A[0] = 2 A[1] = 5 A[2] = 4
the function should return "NO SOLUTION", since there is no strategy that guarantees you a win.
Assume that:
N is an integer within the range [1..100,000];
each element of array A is an integer within the range [1..1,000,000,000].
Complexity:
expected worst-case time complexity is O(N);
expected worst-case space complexity is O(N), beyond input storage (not counting the storage required for input arguments).
Elements of input arrays can be modified.
*/

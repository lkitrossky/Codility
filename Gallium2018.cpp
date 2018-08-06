// Gallium2018.cpp : Defines the entry point for the console application.
// https://app.codility.com/programmers/challenges/gallium2018/
// Gold https://app.codility.com/cert/view/certFVJ4J6-9G6ZGA72C46SF8AF/
// Details: https://app.codility.com/cert/view/certFVJ4J6-9G6ZGA72C46SF8AF/details/
/*

Codility confirms that a person who identified to us on-line as:
Levi Kitrossky
was awarded
Codility Golden Award for the Gallium 2018 Challenge
This is the 56th Codility Golden Award granted this month.
Review detailed assessment: https://app.codility.com/cert/view/certFVJ4J6-9G6ZGA72C46SF8AF/details/
AWARD TYPE:Codility Golden Award (Gallium 2018)
EXPIRATION DATE:2020-06-16 17:00:00 GMT
CHALLENGE POSTED:2018-06-16 17:00:00 GMT
CHALLENGE SOLVED:2018-06-19 21:46:20 GMT
TIME SINCE POSTED:3 days
ASSESSMENT:correct functionality and scalability

*/

#include "stdafx.h"  

/*
More clean:
Codility Golden Award for the Gallium 2018 Challenge
This is the 60th Codility Golden Award granted this month.
https://app.codility.com/cert/view/cert7UAU92-VEVE9SD9MJSSA6UP/   cert
https://app.codility.com/cert/view/cert7UAU92-VEVE9SD9MJSSA6UP/details/  details

*/

//////////////////////////////////////////////////////////////
//	START OF SOLUTION TO PUT
/////////////////////////////////////////////////////////////
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <cmath> 
using namespace std;

//Utility functions
/* How many times inpout can be divided by divisor?
What is the degree of divisir in the input?

*/
int degree(int input, int divisor) {
	int res = 0;
	while (0 == input % divisor) {
		input /= divisor;
		res++;
	}
	return res;
}
//take three numbers from the array and find trailing zeroes number of their multiplication
//instead of the original numbers we already have powers of 2 and  in them 5, other things are not important
int zeroes(vector<int> &twos, vector<int> & fives, int f, int s, int t) {
	int tw = twos.at(f) + twos.at(s) + twos.at(t);
	int fiv = fives.at(f) + fives.at(s) + fives.at(t);
	return min(tw, fiv);
}
//the same but without array, just pairs themselves
int zeroes2(const pair<int,int>& f, const pair<int, int>& s, const pair<int, int>& t) {
	return min(f.first + s.first + t.first, f.second + s.second + t.second);
}
//Utility class to create triads of pairs of powers
class Triangle {
	public:
		Triangle() {};
		Triangle(pair<int, int>_first, pair<int, int>_second, pair<int, int>_third):
			first(_first), second(_second), third(_third) 	{};
		pair<int, int>first, second, third;
};
//as zeroes2 but on a triad
int zeroes3(const Triangle & triangle ) {
	return zeroes2(triangle.first, triangle.second, triangle.third);
}
//switch to print or not print intermediate results, nice trick
const bool PRINT = false;
//Print outs utility functions, switched by PRINT
void printA(const vector<vector<int > > & a) {
	if (!PRINT)return;
	if (a.empty()) {
		cout << "printA Empty vector\n";
		return;
	}
	for (auto au1 = a.begin(); au1 != a.end(); au1++) {
		for (auto au2 = au1->begin(); au2 != au1->end(); au2++) {
			cout << *au2 << " ";
		}
		cout << endl;
	}
}
void printA(const vector<vector<pair<int, int> > > & a) {
	if (!PRINT)return;
	if (a.empty()) {
		cout << "printA Empty vector\n";
		return;
	}
	for (auto au1 = a.begin(); au1 != a.end(); au1++) {
		for (auto au2 = au1->begin(); au2 != au1->end(); au2++) {
			cout << "(" << au2->first << "," << au2->second << ") ";
		}
		cout << endl;
	}
}
void printV(const vector<int> & a, int len, int start) {
	if (!PRINT)return;
	if (a.empty()) {
		cout << "printV Empty vector\n";
		return;
	}
	for (auto au2 = a.begin() + start; au2 != a.begin() + start + len; au2++) {
		cout << *au2 << " ";
	}
	cout << endl;
}
void printV(const vector<pair<int, int>> & a, int len, int start) {
	if (!PRINT)return;
	if (a.empty()) {
		cout << "printV Empty vector\n";
		return;
	}
	for (auto au2 = a.begin() + start; au2 != a.begin() + start + len; au2++) {
		cout << "(" << au2->first << "," << au2->second << ") ";
	}
	cout << endl;
}
void printV2(const vector< int> & a, int len, int start) {
	if (!PRINT)return;
	if (a.empty()) {
		cout << "printV2 Empty instances vector\n";
		return;
	}
	cout << "instances" << endl;
	for (auto au2 = a.begin() + start; au2 != a.begin() + start + len; au2++) {
		cout << *au2 << " ";
	}
	cout << endl;
}
//this template function was a main difficult idea
/*

We have an array of N objects, every object i has also a number (instances) ri
We want to list all possible combinations of length k, it can have duplicates
Done with recursion, difficult
Template Parameter: A - any object
Parameters:
const vector<A> & a,				- the array to work upon, all value are now unique
const vector<int> instances,		- how many times every number appears, must be the same length
int lenCombination,					- how long is the combination
int start,							- usually we work on all array but recursion needs to work not from the beginning too
int lenFragment						- from star and which length?

Return value: we need to return combinations as <vector<A>, all of them are in vector<vector<A> >
*/
template < class A> vector<vector<A> > 
	bringList(const vector<A> & a, const vector<int> instances, int lenCombination, int start, int lenFragment) {
		vector<vector<A> > res;
		int N = a.size();
		printV(a, lenFragment, start);
		printV2(instances, lenFragment, start);
		if (PRINT)cout << "lenCombination: " << lenCombination << " start: " << start << " lenFragment:" << lenFragment << endl;
		if (lenCombination < 1 || N < 1 || start >= N || start < 0 || lenFragment < 1 || start + lenFragment > N) {
			if(PRINT)cout << "Incorrect values\n";
			return res;
		}
		// we need lenCombination from one number, only if has enough instances
		if (1 == lenFragment) {
			if (instances.at(start) >= lenCombination) {
				vector<A>duple(lenCombination, a.at(start));
				res.push_back(duple);
			}
			printA(res);
			return res;
		}
		//the first number is no more, so just move the next
		if (0 == instances.at(start)) {
			res = bringList<A >(a, instances, lenCombination, start + 1, lenFragment - 1);
			printA(res);
			return res;
		}
		//for length one of the combination just collect all uniques
		if (1 == lenCombination) {
			for (int i = start; i < start + lenFragment; i++) {
				if (instances.at(i) > 0) {
					vector<A>single(1,a.at(i));
					res.push_back(single);
				}
			}
			return res;
		}
		//after all particular cases we in the main place
		//seek with the number in start position or without it?
		// do both and then concatenate
		vector<int> instances2;
		instances2.insert(instances2.end(),instances.begin(), instances.end());
		instances2.at(start)--;
		//do with so, the lenCombination decremented, needed less one
		vector<vector<A> > withstarting = bringList<A>(a, instances2, lenCombination - 1, start, lenFragment);
		if (PRINT)cout << "withstarting before inserts\n";
		printA(withstarting);
		//now put the starting number in places
		for (size_t i = 0; i < withstarting.size(); i++) {
			withstarting.at(i).insert(withstarting.at(i).begin(), a.at(start));
		}
		if (PRINT)cout << "withstarting after inserts\n";
		printA(withstarting);

		//do witout the staring number, lenCombination intact, but the fragment is short, left end moves to the right
		vector<vector<A> > withoutstarting = bringList<A>(a, instances, lenCombination, start + 1, lenFragment - 1);
		printA(withoutstarting);
		res = withstarting;
		res.insert(res.end(), withoutstarting.begin(), withoutstarting.end());
		printA(res);
		return res;
		set   < vector<A> > mySet;
		for (auto au : res) {
			if (mySet.end() == mySet.find(au))
				mySet.insert(au);
		}
		//concatenate both with and without
		vector<vector<A> > res2(mySet.size());
		int i = 0;
		for (auto au : mySet) {
			res2.at(i++) = au;
		}
		return res2;
}
	//The same as above but it starts all the recurion, so less parameters.
template < class A> vector<vector<A> > bringList(const vector<A> & a, const vector<int> instances, int len) {
	printV(a, 0, a.size());
	printV2(instances, 0, a.size());
	vector<vector<A> > res = bringList<A>(a, instances, len, 0, (int)a.size());
	return res;
	set   < vector<A> > mySet;
	for (auto au : res) {
		if (mySet.end() == mySet.find(au))
			mySet.insert(au);
	}
	vector<vector<A> > res2(mySet.size());
	int i = 0;
	for (auto au : mySet) {
		res2.at(i++) = au;
	}
	return res2;
}


/*

There are several ideas here
1. Numbers are big and they are many. But in essence we need only how many powers of 2 and 5 are in them
2. So just pass over all array and transform into two: poers of 2 and 5 in every number
3. Add another array: how many combinations (power2, power5)? They can be a lot of duplicates, just keep number separately
4. Pairs p2, p5 are easily organize into the table, 2 cannot be in power more than 29, and 5 - more than 12
5. So we have a table 12*29 and in cells there numbers of instances
6. We can take list tiqads from the table as above and find maxmum of trailing zeroes, it is a silver solution
7. In order to make gold we add another non-standard idea called restriction. Suppose there is some pair in the table which have severl occupied cells below.
The we may mever use it, becuase we can always substitute by a more effectibe cell. 
Example: 2,3 can be neglected becaue it can be replaced by a bigger pair below. ENough that there be several cells with overall instance >=3
Otherwise they are maybe all in usage. This frees us to take all the table, we deal almost inclusevly with the the bottom right rich region
(2,3)
(2,4)
(2,5)
(2,6)
8. This is almost enough but to make it still more effective to arrive to gold, anothert restriction is done like:
(2,3) (postion retricted from below, now it restrics also to the left)
9. We keep the table intact but create an additional one with only acquired instances from below, we use to filter values before 
finding all combinations.


More on ideas 1 and 2:
5^MAX_POWER5 + 1=244140625
5^13= MAX_POWER5 + 120703MAX_POWER5 + 15 > 1000000000
2^MAX_POWER2 + 1 = 1073741824 > 1000000000
So, in all numbers <=1000000000 there are no more than 5^MAX_POWER5 + 1 [0,MAX_POWER5 + 1] and no more than 2^29 [0,29]
Also some cominations give > 10^9 and one can be thrown: 0,0
So, there are a lot of copies but order is of no importance.
After we reduce all numbers into pairs (x2, x5) - how many divisors, we can just keep table
MAX_POWER5 + 1*MAX_POWER2 + 1  with numbers of copies, it takes O(N) to fill it.  We pass on 0,0 it will be 0 always
We will also keep map to say how many copies have power p2 of 2 and separately p5 powers of 5
Keep also unique pairs
The brute force is to choose 3 from 360 with copies
360!/(357!*3!)  instead of 360 - number of unique combinations <= MAX_POWER5 + 10*359*358 - too much

*/

int solution(vector<int> &A) {
	const int MAX_POWER2 = 29;
	const int MAX_POWER5 = 12;
	int res = 0;
	int N = A.size();
	vector<int> twos(N, 0), fives(N, 0);
	vector<int> line(MAX_POWER2 + 1, 0);
	vector < vector<int> >table(MAX_POWER5 + 1, line);
	set< int> uniquePower2Copies, uniquePower5Copies;  //keep all powers met as unique
	vector<int>countOfPowers2(MAX_POWER2 + 1, 0), countOfPowers5(MAX_POWER5 + 1, 0); //keep counters on every 
	vector<int>maxPerColumn(MAX_POWER2 + 1, 0);
	vector<int>maxPerLine(MAX_POWER5 + 1, 0);
	int max2 = 0, max5 = 0;
	//create the table explained above
	for (int i = 0; i < N; i++) {
		twos.at(i) = degree(A.at(i), 2);
		fives.at(i) = degree(A.at(i), 5);
		maxPerColumn.at(twos.at(i)) = max(maxPerColumn.at(twos.at(i)), fives.at(i));
		maxPerLine.at(fives.at(i)) = max(maxPerLine.at(fives.at(i)), twos.at(i));
		max2 = max(max2, twos.at(i));
		max5 = max(max5, fives.at(i));
		if (uniquePower2Copies.end() == uniquePower2Copies.find(twos.at(i)))
			uniquePower2Copies.insert(twos.at(i));
		if (uniquePower5Copies.end() == uniquePower5Copies.find(fives.at(i)))
			uniquePower5Copies.insert(fives.at(i));
		table.at(fives.at(i)).at(twos.at(i))++;
		countOfPowers2.at(twos.at(i))++;
		countOfPowers5.at(fives.at(i))++;
	}


	//Restriction, see 8 and 9 above
	vector<int> rline(MAX_POWER2 + 1, 0);
	vector < vector<int> >rtable(MAX_POWER5 + 1, line); //we keep acquired instances, if > 3 no sense in this value, always can use bigger
	for (int i = MAX_POWER2; i >=0; i--) {
		int acquired = 0;
		for (int j = MAX_POWER5; j >= 0; j--) {
			rtable.at(j).at(i) = acquired; // for restriction 8
			if (i < MAX_POWER2)
				rtable.at(j).at(i) = max(acquired, rtable.at(j).at(i + 1));   // for restriction 9 from side
			acquired = table.at(j).at(i);
		}
	}

	//another idea for restriction not used: by lines, possible not bringing new things
	/*
	for (int i = MAX_POWER5; i >= 0; i--) {
		int acquired = 0;
		for (int j = MAX_POWER2; j >= 0; j--) {
			rtable.at(i).at(j) = max(rtable.at(i).at(j),table.at(i).at(j) + acquired);
			if (j < MAX_POWER5)
				rtable.at(i).at(j) = max(rtable.at(i).at(j), rtable.at(i).at(j + 1));
			acquired = rtable.at(i).at(j);
		}
	}
	*/
	
	//fill the a list of pairs and a list of instance numbers
	vector<pair<int, int> >vectorFromTable;
	vector<int>instancesFromTable;
	for (int i = 0; i < MAX_POWER2 + 1; i++) {
		for (int j = 0; j < MAX_POWER5 + 1; j++) {
			if(table.at(j).at(i) > 0 && rtable.at(j).at(i) <= 3){
				vectorFromTable.push_back(pair<int, int>(i, j));
				instancesFromTable.push_back(table.at(j).at(i));
			}
		}
	}
	//now bring the lst with retrictions and pass of all it
	vector<vector<pair<int, int>> >threesList = bringList<pair<int, int>>(vectorFromTable, instancesFromTable, 3);
	for (auto au = threesList.begin(); au != threesList.end(); au++){
		Triangle triangle(au->at(0), au->at(1), au->at(2));
		res = max(res, zeroes3(triangle));
	}
	return res; 
}
///////////////////////////////////////////////////////
//	END OF SOLUTION TO PUT
///////////////////////////////////////////////////////

/*
https://app.codility.com/cert/view/certGV8DTF-RVAVWZ4XRRJ3G9HC/
Silver award can be used as a checker, brute force from some moment
*/
int solutionSilver(vector<int> &A) {
	int res = 0;
	int N = A.size();
	vector<int> twos(N, 0), fives(N, 0);
	for (int i = 0; i < N; i++) {
		twos.at(i) = degree(A.at(i), 2);
		fives.at(i) = degree(A.at(i), 5);
	}
	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			for (int k = j + 1; k < N; k++) {
				res = max(res, zeroes(twos, fives, i, j, k));
			}
		}
	}
	return res;
}

//automatic tester vs. Silver
// Nice idea: once failed print out non-random part for testing
void check(vector<int> &X, int expected) {
	int N = X.size();
	int answ = solutionSilver(X);
	if (answ != expected)
		cout << "//Error!! Expected " << expected << " got " << answ << " length: "<<X.size()<< endl;
	//else
		//cout << "OK, answer is as expected "<< expected << endl;
	answ = solution(X);
	if (answ != expected) {
		cout << "\n//Error in the candidate solution!! Expected " << expected << " got " << answ << " length: " << X.size() << endl;
		cout << "int atest[] = {";
		for (int i = 0; i < N; i++) {
			if (i < N - 1) {
				cout << X.at(i) << ", ";
				if (4 == i % 5)
					cout << endl;
			}
			else
				cout << X.at(i);
		}
		cout << "};\n";
		cout << "vector<int> vtest2(atest, atest + sizeof(atest) / sizeof(int));" << endl;
		cout << "int expected5 = solutionSilver(vtest2);" << endl;
		cout <<"check(vtest2, expected5);" << endl;
	}
	//else
		//cout << "OK, candidate answer is as expected " << expected << endl;
}
//randomized check scenario

void randomCheck(int len, int pow2, int pow5) {
	int rland = rand() % len + 3;
	//int rland = 10;
	vector<int>vtest(rland, 0);
	for (auto au = vtest.begin(); au != vtest.end(); au++) {
		int t = (int)pow(2, rand() % pow2);
		int f = (int)pow(5, rand() % pow5);
		*au = (t*f) % 1000000000;
		if (*au < 0)
			*au += 1000000000;

	}
	int expected4 = solutionSilver(vtest);
	check(vtest, expected4);
}
int main()
{

	
	//this is the non-random test automatically generated from the failed random test
	//Error in the candidate solution!! Expected 4 got 2 length: 5
/*	int atest[] = { 25, 10, 25, 10, 32 };
	vector<int> vtest2(atest, atest + sizeof(atest) / sizeof(int));
	int expected5 = solutionSilver(vtest2);
	check(vtest2, expected5);
	return 0;
	*/

	//half manual tests
	int A1[] = { 7, 15, 6, 20, 5, 10 };
	int A2[] = { 25, 10, 25, 10, 32 };
	vector<int> VA1(A1, A1 + 6);
	vector<int> VA2(A2, A2 + 5);
	int expected1 = 3;
	check(VA1, expected1);
	int expected2 = 4;
	check(VA2, expected2);
	int A3[] = { 25, 125, 32 };
	vector<int> VA3(A3, A3 + sizeof(A3) / sizeof(int));
	int expected3 = solutionSilver(VA3);
	check(VA3, expected3);
	
	//running random test
	// If failed it will generate non random failed test, nice idea
	srand((unsigned int)time(NULL));
	for (int ii = 0; ii < 10; ii++) {
		if (PRINT)cout << "Printing\n";
		cout << "#" << ii << endl;
		randomCheck(50, 29, 12);
	}
	
	return 0;
}

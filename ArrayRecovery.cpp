// ArrayRecovery.cpp : Defines the entry point for the console application.
// https://codility.com/programmers/lessons/92-tasks_from_indeed_prime_2016_college_coders_challenge/array_recovery/
//see Readme.txt for the full text


#include "stdafx.h"


#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
#include <vector>
#include <iostream>
#include <limits.h>
#include <math.h> 
using namespace std;


//this function does modular multiplication of integers, but take long long to avoid overload
//  i*j modulo mod
long long mult(long long i, long long j, long long mod){
	i = i%mod;
	j = j%mod;
	return ((i*j)%mod);
}
//multiplies from startingValue till i inclusive
long long ReducedFactorial(long long startingValue,long long i, long long mod){
	if(1 >= i)return 1;
	if(startingValue == i) return startingValue;
	if(startingValue > i) return 1;
	long long  res = 1;
	for(long long  j = startingValue; j <= i; j++){
		res = mult(j, res, mod);
	}
	return res;
}

//extended recursive Euclid algorithm, used to calculate modular inverse
// a,b - input, x,y - output
//from https://stackoverflow.com/questions/35226781/finding-binomial-coefficient-for-large-n-and-k-modulo-m
long long  xGCD(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long  x1, y1, gcd = xGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - (long long)(a / b) * y1;
    return gcd;
}
//calculates modular inverse that is from an integer a produces b:  a*b = 1 modulo mod
// used to produce a modular ratio of two integers, the denominator is first must be inversed and then
//used as a factor instead of division
//from https://stackoverflow.com/questions/35226781/finding-binomial-coefficient-for-large-n-and-k-modulo-m
long long inverse(long long a, long long mod) {
    long long  x, y;
    xGCD(a, mod, x, y);
    return (x+mod)%mod;
}

/*

	Main and the moist difficult part of the problem
	how many non-increasing sequences startting no more
	than a limit and all positive , that is > 0
	It depends on a length of sequence, that is "steps" below
	Every number n: limit>=x>0  and every A[i]<=A[i-1] from i>0
	the formula is 
	from = steps+limit-1   
	Answer is a binomial coefficient steps/from, where from = steps+limit-1
	That is  from!/[(from-steps!)*steps!] for this formula we need all modular operations above
	
	Example for 2 steps with limit 4: how many non-increase sequences of two numbers starts <= 4 and ends > 0
	Possible values are 1,2,3,4
	4,4 4
	4,4,3
	4,4,2 
	4,4,1       4
	4,3,3
	4,3,2
	4,3,1		3
	4,2,2
	4,2,1		2
	4,1,1		1   Overall 10
	3,3,3
	3,3,2
	3,3,1
	3,2,2
	3,2,1
	3,1,1       6  Overall 16
	2,2,2
	2,2,1
	2,1,1		3  Overall 19
	1 1,1       1  Overall 20   which is (4+3-1)!/(3!3!) = 6!/3!3! = 20
	Table version 
				Length
			0	1	2	3	4	5	
Limit	1   1	1	1	1	1	1			length C length
		2	1	2	3	4	5	6			length C length+1
		3	1	3	6	10	15	21			length C length+2
		4	1	4	10	20	35	56			length C length+3
		5	1	5	15	35	70	126			length C length+4
		6	1	6	21	56	126 252			length C length+5
One can easily see Pascal triangle in the diagonal representation or a set of binomial coefficients
*/
int NonIncreaseVariants(int steps, int limit, long long mod){
	long long  from = steps + limit - 1;   //from can be equal to MODULO in this case the answer will be 0!
	long long big = steps > from-steps ? steps : from - steps;
	long long small = steps < from-steps ? steps : from - steps;
	long long  numerator = ReducedFactorial(big+1, from, mod);
	long long  denominator   = ReducedFactorial(1, small,mod);
	long long  denominator_inverted   = (inverse(denominator,mod)  + mod)%mod;
	long long  check_unit = mult(denominator, denominator_inverted, mod);
	if(1 != check_unit) {
		throw "inverse failed";
	}
	long long  res = mult(numerator, denominator_inverted, mod);
	return (int)res;
}

//The same as above but not > 0 rather > floor_limit
int NonIncreaseVariants(int steps, int ceiling_limit, int floor_limit, long long mod){
		return NonIncreaseVariants( steps, ceiling_limit - floor_limit, mod);
}

//Take increasing sequence v and find the first number more than the given value
//if none use limit
int BringUpper(vector<int>& v, int value, int limit){
	auto upper_p = upper_bound(v.begin(), v.end(), value);
	bool fNotFound = (upper_p == v.end());
	int upper = fNotFound? limit : *upper_p;
	return upper;
}

//Insert a value to an increasing sequence, discarding all values that are bigger or equal
//Here we in essence keep endings of the non-increasing subsequences of the original problem
//But this seuquence must only increase. 
//The idea is that the last screen the previous unless they are smaller
void AddToIncreasingEndsOfDecrease(int input, vector<int> &output){
	auto upper_p = upper_bound(output.begin(), output.end(), input);
	if(upper_p != output.end())
		output.erase(upper_p, output.end());
	if(output.empty() || output.back()<input)
		output.push_back(input);
}
//split into decreasing sequences and them take ends and leave only increasing order
//The idea is that if we move from left to roght on array B as 
//0,... 100, 100, 4, 100, 100, 3, 100, 100, 5, 100, 6 we leave only ends of non-increasing sequences, like
//  4,3,5,6  others do not influence what is to their right side
//but from 4,3,5  - 4 is also not important, since we meet 3, the latter  "isolates" it from all numbers on the right side of 3
//so while moving from left to roght we leave:
//  4
//	3
//  3,5,6
//the following numbers will be all compared to 3,5,6, one of the will ne the upper bound
vector<int >increasingEndsOfDecrease(vector<int> &B, int M){

	vector<int>increasingEndsOfDecrease(1,M+1);
	for(auto au = B.begin(); au != B.end(); au++){
		if(B.end() == au+1 || *au >= *(au + 1) || 0 == *au)
			continue;
		AddToIncreasingEndsOfDecrease(*au, increasingEndsOfDecrease);
	}
	return increasingEndsOfDecrease;
}


const long long MODULO = 1000000007;
/*

	The main function deals with three types of subsequences:
	constant non-zero, constant zero, non-increasing
	

*/
int solution(vector<int> &B, int M){

	const int N = B.size();
	if(1 >= N)
		return M;
	vector<int>increasingEndsOfDecrease(1,M); //as explained above we leave only ends of decreasing sequences
	//and only in monotone increasing, others are discarded
	//increasingEndsOfDecrease[0] will be always the minimal met number except the initial state as limit
	//increasingEndsOfDecrease[0] used for upper or equal bound for 0 sequences
	//increasingEndsOfDecrease used also to find upper bound for other numbers
	//it growth while we move from left to right when we meet a decrease, we discord part of it, see above
	int res = 1;
	int zeroSequenceLength = 1;		//says of we are inside all zero sequence, B[0] is always 0
	int nonZeroSequenceLength = 0;  //says if we are inside non-zero constant sequence

	//in the loop there are several swithc of state:
	//beginning, end of B found, equal to the previous, more than the previous, less than the previous, 0 met 
	//all these cases are treated by if statements
	for(int i = 1; i < N; i++){  //the main movement starts from 1 sinceB [0] always 0

		if(0 == B.at(i) ){ //case of 0 splits in several subcases
			if(zeroSequenceLength > 0) {  //continuation of zero sequence
				if(N - 1 == i){  //end of array is the end of the sequence
					int factor = NonIncreaseVariants(zeroSequenceLength+1,increasingEndsOfDecrease.at(0),MODULO);
					res = (int)mult(res, factor, MODULO);
					//cout<<"Test case 1"<<endl;
					return res;
				}
				zeroSequenceLength++;   //continue zero sequnce
				//cout<<"Test case 2"<<endl;
				continue;
			}
			else if(nonZeroSequenceLength > 0) {   //end positive constants sequence 
				int upper = BringUpper( increasingEndsOfDecrease, B.at(i-1), M);
				if(B.at(i-1) >= upper)
					return 0;  //impossible to decipher!
				int factor = NonIncreaseVariants(nonZeroSequenceLength, upper, B.at(i-1), MODULO);
				res = (int)mult(res, factor, MODULO);
				AddToIncreasingEndsOfDecrease(B.at(i-1), increasingEndsOfDecrease);
				nonZeroSequenceLength = 0;
				zeroSequenceLength = 1;
				//cout<<"Test case 3"<<endl;
				if(N - 1 == i) {
					//cout<<"Test case 3B"<<endl;
					int upper1 = increasingEndsOfDecrease.at(0);
					int factor = upper1;
					res = (int)mult(res, factor, MODULO);
					return res;
				}
			}
			else {  //the preceding decreasing sequence finished
				int upper = BringUpper( increasingEndsOfDecrease, B.at(i-1), M);
				if(B.at(i-1) >= upper)
					return 0;  //impossible to decipher!
				res = (int)mult(res, upper - B.at(i-1), MODULO);
				AddToIncreasingEndsOfDecrease(B.at(i-1), increasingEndsOfDecrease); 
				zeroSequenceLength = 1;
				if(N - 1 == i) {
					int upper = BringUpper( increasingEndsOfDecrease, B.at(i), M);
					if(B.at(i) >= upper)
						return 0;  //impossible to decipher!
					res = (int)mult(res, upper - B.at(i), MODULO);
					//cout<<"Test case 3A"<<endl;
					return res;
				}
				//cout<<"Test case 4"<<endl;
			}
		} // end of 0 met
		else {  //non-zero case split into several 
			if(zeroSequenceLength > 0) {   //we are after 0,0,0 close the zero sequense.
				//                                          -1 because now we know the last zero ciphers B[i]
				int factor = NonIncreaseVariants(zeroSequenceLength-1,increasingEndsOfDecrease.at(0),B.at(i)-1,MODULO);
				res = (int)mult(res, factor, MODULO);
				zeroSequenceLength = 0;
				AddToIncreasingEndsOfDecrease(B.at(i), increasingEndsOfDecrease); //the previos clear was the smallest til now
				if( N - 1 == i){
					res = (int)mult(res, M - B.at(i), MODULO); //zero on the left isolates it from any previous history
					//the same effect is to do after AddToIncreasingEndsOfDecrease(B.at(i), increasingEndsOfDecrease);
					//cout<<"Test case 5"<<endl;
					return res;
				}
				//cout<<"Test case 6"<<endl;
			}
			else if(nonZeroSequenceLength > 0){ //we are after 3,3,3... close or continue
				if(B.at(i-1) == B.at(i)){ //continue 3,3,3...
					nonZeroSequenceLength++;
					if(N - 1 == i){
						int upper = BringUpper( increasingEndsOfDecrease, B.at(i-1), M);
						if(B.at(i-1) >= upper)
							return 0;  //impossible to decipher!
						res = (int)mult(res, NonIncreaseVariants(nonZeroSequenceLength,upper, B.at(i-1),MODULO), MODULO);
						//cout<<"Test case 7"<<endl;
						return res;
					}
				}
				else if(B.at(i-1) > B.at(i)){
					//calc sequence
					int upper1  = BringUpper( increasingEndsOfDecrease, B.at(i-1), M);
					if(B.at(i-1) >= upper1)
						return 0;  //impossible to decipher!
					res = (int)mult(res, NonIncreaseVariants(nonZeroSequenceLength, upper1, B.at(i-1),MODULO),MODULO);
					nonZeroSequenceLength = 0; 
					AddToIncreasingEndsOfDecrease(B.at(i-1), increasingEndsOfDecrease); 
					if(N - 1 == i){
						int upper = BringUpper( increasingEndsOfDecrease, B.at(i), M);
						if(B.at(i) >= upper)
							return 0;  //impossible to decipher!
						res = (int)mult(res, upper - B.at(i),MODULO);
						//cout<<"Test case 8"<<endl;
						return res;
					}
					//cout<<"Test case 9"<<endl;
				}
				else {
					int upper = BringUpper( increasingEndsOfDecrease, B.at(i-1), M);
					if(B.at(i-1) >= upper)
						return 0;  //impossible to decipher!
					//zeroSequenceLength - 1 : why - 1? Because the increase discoveres the last member of the sequence
					res = (int)mult(res, NonIncreaseVariants(nonZeroSequenceLength - 1,upper, B.at(i)-1,MODULO),MODULO);
					nonZeroSequenceLength = 0; 
					AddToIncreasingEndsOfDecrease(B.at(i-1), increasingEndsOfDecrease); 
					if(N - 1 == i){
						int upper = BringUpper( increasingEndsOfDecrease, B.at(i), M);
						if(B.at(i) >= upper)
							return 0;  //impossible to decipher!
						res = (int)mult(res, upper - B.at(i),MODULO);
						//cout<<"Test case 10"<<endl;
						return res;
					}
				}
			}
			else {
				if(B.at(i-1) == B.at(i)){
					nonZeroSequenceLength = 2;
					if(N - 1 == i) { 
						int upper = BringUpper( increasingEndsOfDecrease, B.at(i), M);
						if(B.at(i) >= upper)
							return 0;  //impossible to decipher!
						int factor = NonIncreaseVariants(nonZeroSequenceLength,upper, B.at(i),MODULO);
						res = (int) mult (res, factor, MODULO);
						//cout<<"Test case 11"<<endl;
						return res;
					}
					//cout<<"Test case 12"<<endl;
				}
				else if(B.at(i-1) < B.at(i)){
					AddToIncreasingEndsOfDecrease(B.at(i-1), increasingEndsOfDecrease);
					if(N - 1 == i) { 
						int upper = BringUpper( increasingEndsOfDecrease, B.at(i), M);
						if(B.at(i) >= upper)
							return 0;  //impossible to decipher!
						res = (int)mult(res, upper - B.at(i), MODULO);
						//cout<<"Test case 13"<<endl;
						return res;
					}
					//cout<<"Test case 14"<<endl;
				}
				else {  //continuation of decrease
					int upper = BringUpper( increasingEndsOfDecrease, B.at(i-1), M);
					if(B.at(i-1) >= upper)
						return 0;  //impossible to decipher!
					res = (int) mult(res, upper - B.at(i-1), MODULO);
					AddToIncreasingEndsOfDecrease(B.at(i-1), increasingEndsOfDecrease); 
					if(N - 1 == i) { 
						int upper = BringUpper( increasingEndsOfDecrease, B.at(i), M);
						if(B.at(i) >= upper)
							return 0;  //impossible to decipher!
						res = (int)mult(res,upper - B.at(i),MODULO);
						//cout<<"Test case 15"<<endl;
						return res;  //finished by  decrease
					}
					//cout<<"Test case 16"<<endl;   //decrease and continue
				}
			}
		}   // end of non-zero case 
	}
	//cout<<"Test case 17"<<endl;
	return res;
}

template<class T> void printVector(const vector<T>& res){
	if(res.empty()){
		cout<<"Empty vector"<<endl;
		return;
	}
	std::copy(res.begin(), res.end(), ostream_iterator<T>(cout, " "));
	cout<<endl;
}

vector<int> cipher(vector<int>& input);
int manualDecipher(vector<int>& input, int M);
void RandomCheck();
int _tmain(int argc, _TCHAR* argv[]){
	try {
		cout<<"ArrayRecovery"<<endl;
		int C0[] = {0, 1, 4, 1, 1, 0};
		vector<int> VC0(C0, C0+6);
		cout<<"{0, 1, 4, 1, 1, 0};  6: "<<solution(VC0, 6)<<" Must be 12"<<endl;
		int C1[] = {0, 0, 1, 5, 5, 1};
		vector<int> VC1(C1, C1+6);
		cout<<"{0, 0, 1, 5, 5, 1};  6: "<<solution(VC1, 6)<<" Must be 24"<<endl;
		//return 0;
		int B0[] = {0, 4, 0, 3, 3};
		vector<int>VB0(B0, B0+5);
		cout<<"{0, 4, 0, 3, 3};  5: "<<solution(VB0, 5)<<" Must be 3"<<endl;
		int B00[] = {0, 2, 2, 4, 4};
		vector<int>VB00(B00, B00+5);
		cout<<"{0, 2, 2, 4, 4};  5: "<<solution(VB00, 5)<<" Must be 2"<<endl;
		//return 0;

		int B1[] = {0, 2, 2};
		cout<<"{0, 2, 2};  4: "<<solution(vector<int>(B1, B1 + sizeof(B1)/sizeof(int)),4)<<" Must be 3"<<endl;
		int B2[] = {0, 3, 5, 6};
		cout<<"{0, 3, 5, 6};  10: "<<solution(vector<int>(B2, B2 + sizeof(B2)/sizeof(int)),10)<<
			" Must be 4"<<endl;
		int B3[] = {0, 0, 0, 0};
		vector<int> VB3(B3, B3+4);
		cout<<"{0,0,0,0}; 10 : "<<solution(VB3, 10)<<" Must be 715"<<endl;
		vector<int> encrypted1 =cipher(VB3);
		//printVector<int>(encrypted1);
		cout<<"Manual decipher: "<<manualDecipher(encrypted1, 10)<<endl;
		long long i1 = 100000;
		long long res1 = ((i1+1)*i1/2);
		long long res2 = res1%MODULO;
		cout<<"{0,0}; 100000 : "<<solution(vector<int>(B3, B3 + 2), 100000)<<" Must be 49965"<<endl;
		cout<<"{0,0}; 100 : "<<solution(vector<int>(B3, B3 + 2), 100)<<" Must be 5050"<<endl;
		cout<<"{0,0,0}; 10 : "<<solution(vector<int>(B3, B3 + 3), 10)<<" Must be 220"<<endl;
		int B4[] = {0,22};
		vector<int>V4(B4, B4 + 2);
		cout<<"{0,22}; 100 : "<<solution(V4, 100)<<" Must be 78"<<endl;
		int B5[] = {0,1,2,3};
		vector<int>V5(B5, B5 + 4);
		cout<<"{0,1,2,3}; 100 : "<<solution(V5, 100)<<" Must be 97"<<endl;
		int B6[] = {0,0,1,2,3};
		vector<int>V6(B6, B6 + 5);
		cout<<"{0,0,1,2,3}; 10 : "<<solution(V6, 10)<<" Must be 70"<<endl;
		int B7[] = {0,0,0,1,2,3};
		vector<int>V7(B7, B7 + 6);
		cout<<"{0,0,0,1,2,3}; 10 : "<<solution(V7, 10)<<" Must be 385"<<endl;
		int B8[] = {0,0,0,0,1,2,3};
		vector<int>V8(B8, B8 + 7);
		cout<<"{0,0,0,0,1,2,3}; 10 : "<<solution(V8, 10)<<" Must be 1540"<<endl;
		cout<<"{0,0,0,0,1,2,3}; 9 : "<<solution(V8, 9)<<" Must be 990"<<endl;
		int B9[] = {0,3,0,1};
		vector<int>V9(B9, B9 + 4);
		cout<<"{0,3,0,1}; 10 : "<<solution(V9, 10)<<" Must be 63"<<endl;
		int B10[] = {0,3,0,0,1};
		vector<int>V10(B10, B10 + 5);
		cout<<"{0,3,0,0,1}; 10 : "<<solution(V10, 10)<<" Must be 189"<<endl;
		int B11[] = {0,0,2,2,0,0};
		vector<int>V11(B11, B11 + 6);
		cout<<"{0,0,2,2,0,0}; 10 : "<<solution(V11, 10)<<" Must be 972"<<endl;
		int B12[] = {0,3,3,3};
		vector<int>V12(B12, B12 + 4);
		cout<<"{0,3,3,3}; 10 : "<<solution(V12, 10)<<" Must be 84"<<endl;
		int B13[] = {0,4,5,5,4};
		vector<int>V13(B13, B13 + 5);
		cout<<"{0,4,5,5,4}; 10 : "<<solution(V13, 10)<<" Must be 15"<<endl;
		int B14[] = {0,4,5,5,6};
		vector<int>V14(B14, B14 + 5);
		cout<<"{0,4,5,5,6}; 10 : "<<solution(V14, 10)<<" Must be 20"<<endl;
		//cout<<"Manual decipher: "<<manualDecipher(V14, 10)<<endl;
		int B15[] = {0,4,5,5,4,0,3};
		vector<int>V15(B15, B15 + 7);
		cout<<"{0,4,5,5,4,0,3}; 10 : "<<solution(V15, 10)<<" Must be 105"<<endl;
		//cout<<"Manual decipher: "<<manualDecipher(V15, 10)<<endl;
		int B16[] = {0,2,4,6,8,6,4,2};  // from 2,4,6,8,10,7,5,3
		vector<int>V16(B16, B16 + 8);
		cout<<"{0,2,4,6,8,6,4,2}; 10 : "<<solution(V16, 10)<<" Must be 16"<<endl;
		//cout<<"Manual decipher: "<<manualDecipher(V16, 10)<<endl;
		int B17[] = {0,2,4,6,8,6,4,2,0};  // from 2,4,6,8,[10-9],[8-7],[6-5],[4-3],[2-1]
		vector<int>V17(B17, B17 + 9);
		cout<<"{0,2,4,6,8,6,4,2,0}; 10 : "<<solution(V17, 10)<<" Must be 32"<<endl;
		int clear[] = {2,4,6,8,9,8,6,4,2};
		cout<<"cipher(2,4,6,8,9,8,6,4,2) = "<<endl;
		vector<int> encrypted =cipher(vector<int>(clear, clear + 9));
		printVector<int>(encrypted);
		//cout<<"Manual decipher: "<<manualDecipher(encrypted, 10)<<endl;
		int B18[] = {0,4,5,5,5,0};  // from 4,5, three of [10,5) = 35, [1,2,3,4]  overal 140
		vector<int>V18(B18, B18 + 6);
		cout<<"{0,4,5,5,5,0}; 10 : "<<solution(V18, 10)<<" Must be 140"<<endl;
		RandomCheck();
		int B19[] = {0,0,0,0,0,0,0,0};  // from 4,5, three of [10,5) = 35, [1,2,3,4]  overal 140
		vector<int>V19(B19, B19 + 8);
		int mod = 1000000000;
		cout<<"{0,0,0,0,0,0,0,0}; " << mod<<" : " <<solution(V19, mod)<<" Must be 0. The final stroke!"<<endl;
		
		return 0;
	}
	catch (string &exc){
		cout<<exc.c_str()<<endl;
		return 0;
	}

	
}

//https://codility.com/programmers/lessons/92-tasks_from_indeed_prime_2016_college_coders_challenge/array_recovery/
// see readme

/*
Results are only 6%  !!!

*/

/*
For the next try corrected many minor logic errors in solution
and also replaced all simple multpilication by mult modular function
13%

*/


/*
Fourth time: correctness 33%

* small_functional 
small functional, 3 <= N <= 5, M <= 20  X WRONG ANSWER 
got 4 expected 44
1. 0.004 s OK
2. 0.004 s OK
3. 0.004 s WRONG ANSWER,  got 4 expected 44
4. 0.004 s OK
5. 0.004 s WRONG ANSWER,  got 0 expected 1
6. 0.004 s WRONG ANSWER,  got 1 expected 12
7. 0.004 s OK


tiny_random 
tiny random sequence  X WRONG ANSWER 
got 0 expected 20
1. 0.004 s WRONG ANSWER,  got 0 expected 20
2. 0.004 s WRONG ANSWER,  got 14 expected 84

special functional, many repetitions  X WRONG ANSWER 
got 0 expected 4
1. 0.004 s WRONG ANSWER,  got 0 expected 4
2. 0.004 s WRONG ANSWER,  got 24 expected 20

est for non decreasing B  X WRONG ANSWER 
got 852390 expected 777483
1. 0.004 s OK
2. 0.004 s OK

*/

/*
Added test cases and check thall all are covered, 50% correctness!  0% Performance, overall 20%

small_functional 
small functional, 3 <= N <= 5, M <= 20  X WRONG ANSWER 
got 2 expected 12
1. 0.004 s OK
2. 0.004 s OK
3. 0.004 s OK
4. 0.004 s OK
5. 0.004 s OK
6. 0.004 s WRONG ANSWER,  got 2 expected 12
7. 0.004 s OK

special functional, many repetitions  X WRONG ANSWER 
got 12 expected 4
1. 0.004 s WRONG ANSWER,  got 12 expected 4
2. 0.004 s WRONG ANSWER,  got 24 expected 20

test for non decreasing B  X WRONG ANSWER 
got 852390 expected 777483
1. 0.004 s OK

*/
/*
try 7

Correctness	Performance	Task score
66%	           22%			40%	

try 8

Tasks in test	
Correctness	Performance	Task score
100%		22%					53%	

Detected time complexity:
O(N*M)

* tiny_N_huge_M 
special test, N <= 4, M >= 100,000,000 X TIMEOUT ERROR 
running time: >6.00 sec., time limit: 0.10 sec.
1. 0.004 s OK
2. 0.004 s OK
3. 6.000 s TIMEOUT ERROR,  running time: >6.00 sec., time limit: 0.10 sec.
4. 5.581 s TIMEOUT ERROR,  running time: 5.58 sec., time limit: 0.10 sec.
* small_random 
small random, N <= 30, M <= 20 ✔OK
1. 0.004 s OK
* medium_random 
medium random, N <= 1,000, M <= 500 ✔OK
1. 0.008 s OK
* large_random 
large random X TIMEOUT ERROR 
running time: >6.00 sec., time limit: 0.22 sec.
1. 6.000 s TIMEOUT ERROR,  running time: >6.00 sec., time limit: 0.22 sec.
* large_bounded_random 
large random: given two functions (f, g), A fulfills f(i) <= A(i) <= f(i) + g(i) X TIMEOUT ERROR 
running time: >6.00 sec., time limit: 0.10 sec.
1. 6.000 s TIMEOUT ERROR,  running time: >6.00 sec., time limit: 0.10 sec.


*/
vector<int> cipher(vector<int>& input) {
	int N = input.size();
	vector<int> res(N, 0);
	for(int i = 1; i < N; i++){
		for(int k = i-1; k >= 0; k--){
			if(input.at(k) < input.at(i)){
				res.at(i) = input.at(k);
				break;
			}
		}
	}
	return res;
}
bool incrementModularArray(vector<int> &digits, int M){
	int N  = digits.size();
	int i = 0;
	while(i < N && digits.at(i) <= M){
		digits.at(i)++;
		if(digits.at(i) <= M)
			return true;
		if(digits.at(i) > M && i == N-1 )
			return false;
		digits.at(i) = 1;
		if(i < N-1){
			i++;
		}
	}
	return true;
}
int manualDecipher(vector<int>& input, int M){
	int res = 0;
	int N  = input.size();
	vector<int>currentDecipher;
	double NM = pow((double)M,(double)N);
	vector<int>digits = input;
	for(auto a = digits.begin(); a!=digits.end();a++ ){(*a)++;}
	digits.at(0)--;
	while(incrementModularArray(digits, M)){
		if(cipher(digits) == input)
			res++;
	}
	return res;
}
void RandomCheck(){
	int loops = 100;
	cout<<"Random loops"<<endl;
	for(int i = 0; i < loops; i++)
	{
		if(0 == i%10)
			cout<<"Loop "<<i<<endl;
		int M = 6;
		int N = 6;
		vector<int>RV(N);
		for(auto au = RV.begin();au!=RV.end();au++){
			*au = 1 + rand()%M;
		}
		vector<int>ERV = cipher(RV);
		int s1 = solution(ERV, M);
		int s2 = manualDecipher(ERV, M);
		//printVector<int> (RV);
		//printVector<int> (ERV);
		//cout<<" sol: "<<s1<<" man: "<<s2<<endl;
		if(s1 != s2){
			cout << "ERROR!!!! M,N = "<< M<< ", " <<N <<" In loop "<<i<<endl;
			printVector<int> (RV);
			printVector<int> (ERV);
			cout<<" sol: "<<s1<<" man: "<<s2<<endl;
		}
	}
}
/*
	9th attempt after simplifying binomial coefficient, reduce first
Correctness	Performance	Task score
100%	    77%			86%
	few_large_groups 
a few large groups  X WRONG ANSWER 
got 1 expected 0
1. 0.004 s OK
2. 0.006 s WRONG ANSWER,  got 1 expected 0
3. 0.012 s OK
4. 0.004 s WRONG ANSWER,  got 625 expected 0

Large_concatenated_monotonic 
concatenated large monotonic sequences, N <= 100,000 X WRONG ANSWER 
got 1 expected 0
1. 0.008 s OK
2. 0.011 s OK
3. 0.005 s WRONG ANSWER,  got 1 expected 0

*/
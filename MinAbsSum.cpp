// MinAbsSum.cpp : Defines the entry point for the console application.
//

//Lesson
//https://codility.com/programmers/lessons/17-dynamic_programming/
//Problem "AMBITIOUS" and too difficult for me, I took the idea from and wrote my code
//https://codility.com/programmers/lessons/17-dynamic_programming/min_abs_sum/
//Solution
//https://codility.com/media/train/solution-min-abs-sum.pdf


#include "stdafx.h"
#include <iostream>
#include <iterator>

//SOLUTION STARTS

#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;


//correctness 83% performance 100% overall 90%
/*
Detected time complexity:
O(N * max(abs(A))**2)
extreme  -  empty and single element RUNTIME ERROR tested program terminated unexpectedly
Added lines
	if(0 == N) return 0;
	if(1 == N) return abs(A.front());

Now it is 100% all

*/
int solution(vector<int> &A){
	int N = A.size();
	if(0 == N) return 0;
	if(1 == N) return abs(A.front());
	vector<int> AABS;
	AABS.resize(N);
	transform(A.begin(),A.end(), AABS.begin(),[](int i){ return abs(i); });
	int sum = accumulate(AABS.begin(), AABS.end(),0);
	int limit = sum/2;
	int biggest = *max_element(AABS.begin(), AABS.end());
	vector<int> count(biggest + 1, 0);
	for(auto au = AABS.begin(); au != AABS.end();au++)
		count.at(*au)++;  //not we have all values of AABS with count, some are 0 count
	int closest_to_half = 0;
	vector<int> candidate(limit+1, -1);
	candidate.at(0) = 1;

	for(int i = 1; i <= biggest; i++){
		if(0 == count.at(i))
			continue; //there is not value like this
		for(auto au = 0; au < (int)candidate.size(); au++){
			if(candidate.at(au) >= 0) //tis candidate found already
				candidate.at(au) = count.at(i);//in cycle i this candidate achieved without i, all i unused
			else {  //this candidate still not reached, can we reach by i from values?
				if(au >= i && candidate.at(au - i) >= 0 )
					candidate.at(au) = candidate.at(au - i) - 1;
			}
			if(candidate.at(au) >= 0  && au <= limit)
				closest_to_half = max(closest_to_half, au);
		} 
	}
	return sum - 2*closest_to_half;
}

//SOLUTION FINISHED


//SLOW SOLUTION
//this solution 1 gives 100% correctness but 40% performance overall 72%
//it is O(N*SUM) although but uses memory only as O(SUM), 
//the site with solutuon thinks it is  O(N*N*M), because S = O(N*M), where M - maximal number of A
int solutionSlow(vector<int> &A){

	int N = A.size();
	vector<int> AABS;
	AABS.resize(N);
	transform(A.begin(),A.end(), AABS.begin(),[](int i){ return abs(i); });
	int sum = accumulate(AABS.begin(), AABS.end(),0);
	int limit = sum/2;
	//int max = *max_element(AABS.begin(), AABS.end());
	int closest_to_half = 0;
	vector<int> candidate(limit+1, 0);
	candidate.at(0) = 1;
	for(int i = 0; i < N;i++){
		vector<int> candidateTemporary(limit+1, 0);
		for(int j = 0;j<limit;j++){
			if(1 == candidate.at(j) ){
				candidateTemporary.at(j) = 1;			//copy possiblity to candiadte too, not to loose them
				if(j+AABS.at(i) <= limit){
					candidateTemporary.at(j+AABS.at(i)) = 1;  //means there is a possibility to get j, so with can have j+AABS(i) too
					closest_to_half = max(closest_to_half, j+AABS.at(i));
				}
			}
		}
		copy(candidateTemporary.begin(), candidateTemporary.end(), candidate.begin()); //enriched by AABS(I)
	}
	return sum - 2*closest_to_half;
}

template<class T> void printVector(const vector<T>& res){
	if(res.empty()){
		cout<<"Empty vector"<<endl;
		return;
	}
	std::copy(res.begin(), res.end(), ostream_iterator<T>(cout, " "));
	cout<<endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	cout<<"MinAbsSum"<<endl;
	int AA[] =  {1, 5, 2, -2};
	vector<int> A(AA, AA + 4);
	printVector<int> (A);
	cout<<"Res: "<<solutionSlow(A)<<"\t"<<solution(A)<<endl;
	vector<int> A2(11,1);
	A2[0] = 16;
	A2[1]=A2[2] = 4;
	printVector<int> (A2);
	cout<<"Res: "<<solutionSlow(A2)<<"\t"<<solution(A2)<<endl;
	vector<int> A3(3,5);
	A3[0] = 6;
	printVector<int> (A3);
	cout<<"Res: "<<solutionSlow(A3)<<"\t"<<solution(A3)<<endl;

	return 0;
}


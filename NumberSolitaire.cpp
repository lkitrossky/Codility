// NumberSolitaire.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//100%  //https://codility.com/demo/results/trainingAMX2K4-V4J/
//https://codility.com/programmers/lessons/17-dynamic_programming/
//This problem is "Respectable" and I solved it totally by myself but in several days with breaks
#include <vector>
#include <algorithm> 
#include <iostream>
using namespace std;
int solution(vector<int> &A){
	int N = A.size();
	if(2 == N ) return A.at(0) + A.at(1);
	int k = N-1;
	vector<int> nextJump;
	while(k >= 0){
		int candidate = A.at(k);
		if(nextJump.size() > 0){
			candidate = candidate + *max_element(nextJump.begin(), nextJump.end());
		}
		if(nextJump.size() >= 6){
			nextJump.erase(nextJump.begin());
		}
		nextJump.push_back(candidate);
		k--;
	}
	return nextJump.back();
}


#include <iterator> // for ostream_iterator
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
	int A[] = {1, -2, 0, 9, -1,-2};
	vector<int> VA(A, A + 6);
	printVector<int>(VA);
	cout<<"Res: "<<solution(VA)<<endl;
	int A2[] = {0, -1, -1, -1, -1, -1, -10, -20, -20, -20, -20, -20, 100, -1000};
	vector<int> VA2(A2, A2 + 14);
	printVector<int>(VA2);
	cout<<"Res2: "<<solution(VA2)<<endl;
	return 0;
}



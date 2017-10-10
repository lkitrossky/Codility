// PassingCars.cpp : Defines the entry point for the console application.
//https://codility.com/programmers/lessons/5-prefix_sums/passing_cars/
//Lesson https://codility.com/programmers/lessons/5-prefix_sums/
//100% painless done in several minutes
#include "stdafx.h"
#include <vector>
using namespace std;

int solution(vector<int> &A) {
    const int MAX_NUMBER = 1000000000;
	const int errorCode = -1;
	int res = 0;
	int foundMoving = 0;
	for(auto au = A.begin(); au != A.end(); au++){
		if(0 == *au) 
			foundMoving++;
		else
			res += foundMoving;
		if(res > MAX_NUMBER)
			return errorCode;
	}
	return res;
}



int _tmain(int argc, _TCHAR* argv[])
{

	return 0;
}


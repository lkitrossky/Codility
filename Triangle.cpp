// Triangle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	try{
		cout<<"Triangle started"<<endl;
		int A[8];
		A[0] = 10;    A[1] = 2;    A[2] = 5;
		A[3] = 1;     A[4] = 8;    A[5] = 20;  A[6] = 0; A[7] = -1;
		vector<int> input1(A, A + sizeof(A)/sizeof(int));
		printVector<int>(input1);
		sort(input1.begin(), input1.end());
		printVector<int>(input1);
		solution(input1);
		A[0] = 10;    A[1] = 50;    A[2] = 5;
		A[3] = 1, A[4] = 0;
		vector<int> input2(A, A + 5);
		solution(input2);
	}
	catch (const out_of_range& e){
		cout<<"Range exception: "<<e.what()<<endl;
	}
	catch(...){
		cout<<"Unknown exception"<<endl;
	}

	return 0;
}

template<class T> void printVector(const vector<T>& res){
	std::copy(res.begin(), res.end(), ostream_iterator<T>(cout, " "));
	cout<<endl;
}


//with includes and using 100%
// https://codility.com/demo/results/trainingGSEYPS-VQD/
// https://codility.com/programmers/lessons/6-sorting/
//Trianlge problem

int solution(vector<int> &A){
	if(A.size() < 3)
		return 0;
	sort(A.begin(), A.end());
	for(auto i = A.begin(); i != A.end()-2; i++){
		if(*i <= 0)
			continue; //no triangles with negative side
		if(*i > *(i+2) - *(i+1)){
			cout<<*i<<" "<<*(i+1)<<" "<<*(i+2)<< endl;
			return 1;
		}
	}
	return 0;
}


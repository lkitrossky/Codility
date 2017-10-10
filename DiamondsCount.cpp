// DiamondsCount.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"

//100% from the first try !! And on time.


#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
#include <vector>
#include <iostream>
#include <limits.h>
#include <math.h> 
using namespace std;


bool mySortFunction(pair<int, int> x1, pair<int, int>x2){
	if( x1.first < x2.first)
		return true;
	if( x1.first == x2.first && x1.second < x2.second)
		return true;
	return false;
}
int solution(vector<int> &X, vector<int> &Y){
	int res = 0;
	int N = X.size();
	vector< pair<int, int> > coordinates(N);
	vector< vector<bool> > map (N, vector<bool>(N,false));
	for(int i =0; i < N; i++){
		coordinates.at(i).first = X.at(i);
		coordinates.at(i).second = Y.at(i);
		map[X.at(i)][Y.at(i)] = true; //these points exits, others not.
	}
	sort(coordinates.begin(), coordinates.end(), mySortFunction);  //in order to prevent repetition we sort them
	for(int i = 0; i < N-1; i++){
		for(int j = i+1; j < N; j++){
			int x1 = coordinates.at(i).first;
			int y1 = coordinates.at(i).second;
			int x2 = coordinates.at(j).first;
			int y2 = coordinates.at(j).second;
			if(x1 == x2 || y1==y2)
				continue;  //must be a diagonal not horizontal or vertical
			int dy1 = y2 - y1;
			int y3 = y2 - 2*dy1;
			int x3 = x2;
			if(false == mySortFunction(pair<int, int>(x2,y2), pair<int, int>(x3,y3)))
				continue;	//prevention of repetition, we must move in the same order
			int dx2 = x2 - x1;
			int x4 = x1 + 2*dx2;
			int y4 = y1;
			if(false == mySortFunction(pair<int, int>(x1,y1), pair<int, int>(x4,y4)))
				continue; 	//prevention of repetition, we must move in the same order
			if(x3 >= N || y3 >= N || x4 >= N || y4 >= N)
				continue;  //candidates for vertices, they must not leave the defined area
			if(x3 <0 || y3 < 0 || x4 < 0 || y4 < 0) //impossible but better be safe than sorry
				continue;  //candidates for vertices, they must not leave the defined area
			//are the candidates are real existing vertices?
			bool condition3 = map[x3][y3];
			bool condition4 = map[x4][y4];
			if(condition3 && condition4){
				res++;
			}
		}
	}

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

int _tmain(int argc, _TCHAR* argv[])
{
	cout<<"Diamond count"<<endl;
	int X1[] = {1, 1, 2, 2, 2, 3, 3}; 
	int Y1[] = {3, 4, 1, 3, 5, 3, 4};

	int X2[] = {1, 2, 3, 3, 2, 1};
	int Y2[] = {1, 1, 1, 2, 2, 2};

	solution(vector<int>(X1, X1+7), vector<int>(Y1, Y1+7));
	solution(vector<int>(X2, X2+6), vector<int>(Y2, Y2+6));

	return 0;
}


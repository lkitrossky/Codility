//https://codility.com/programmers/lessons/99-future_training/polygon_concavity_index/
//future training
//See video https://www.youtube.com/watch?v=0HZaRu5IupM
//Solution is on https://codesays.com/2015/solution-to-polygon-concavity-index-by-codility/
//But I tried my own, that is the turns must be in one direction only
//in the lecture he starts with the lowets point and sort all other point by polar angle, but it is not
//N it is N*log N
//but we need not build full hull!
//enough to find one first point of it: one of the lowest
//the we find one lowest that its precedent or consequent is not the lowest
//The find i-1, i, i+1 - which side it turns. Now every turn in the opposite direction will point to the 
//point we seek
//I had my own formula for this but there are easier way:
//3*3 matrix 
/*
xa, ya, 1
xb, yb, 1
xc, yc, 1
<0 couterclockwise
>0 clockwise
== collinear
*/

#include "stdafx.h"
#include <vector>
using namespace std;

struct Point2D {
  int x;
  int y;
};


//77% correctness 66% Performance. BUT perforamce tests goit wrong results
//still 75% then 835
//finally 100%
int isClockwise(const Point2D a,const Point2D b,const Point2D c){
	double abx = (double)(a.x - b.x);
	double bcy = (double)(b.y - c.y);
	double bcx = (double)(b.x - c.x);
	double aby = (double)(a.y - b.y);
	double res = abx*bcy - bcx*aby;
	if (res < 0) return -1;
	if (res > 0) return  1;
	return 0;
}


vector<int> FindLowestPoints(const vector<Point2D>& A){
	vector<int> res;
	int lowestValue = A.at(0).y;
	res.push_back(0);
	for(auto au = 1; au != A.size(); au++){
		if(A.at(au).y < lowestValue){
			lowestValue = A.at(au).y;
			res.clear();
			res.push_back(au);
		}
		else if(A.at(au).y == lowestValue){
			res.push_back(au);
		}
	}
	return res;
}
int FindRealLowestAngleIndex(const vector<Point2D>& A){
	int N = A.size();
	vector<int> lowestArray = FindLowestPoints(A);
	for(int i = 0; i < (int)lowestArray.size();i++){
		int previous = (0 == lowestArray.at(i))? N-1: lowestArray.at(i)-1 ;
		int next =  (N-1 == lowestArray.at(i))? 0: lowestArray.at(i)+1 ;
		if( 0!= isClockwise(A[previous],A[lowestArray.at(i)], A[next]))
			return lowestArray.at(i);
	}
	return -1;
}

int solution(vector<Point2D>& A){
	int N = A.size();
	int res = -1;
	int real = FindRealLowestAngleIndex(A);
	if(real < 0) return -1;
	int previous = (0 == real)? N-1: real-1 ;
	int next =   (N-1 == real)? 0  : real+1 ; 
	int goodTurn = isClockwise(A.at(previous),A.at(real),A.at(next));
	for(int i = 0; i < (int)A.size(); i++){
		int previous2 = (0 == i)? N-1: i-1;
		int next2 =  (N-1 == i)? 0: i+1 ;
		int turn = isClockwise(A.at(previous2),A.at(i),A.at(next2));
		if(0 == turn)continue;
		if(goodTurn > 0 && turn  > 0)continue;
		if(goodTurn < 0 && turn  < 0)continue;
		return i;
	}
	return res;
}
#include <iostream>
int _tmain(int argc, _TCHAR* argv[])
{
	Point2D A[7];
	A[0].x = -1;  A[0].y =  3;
	A[1].x =  1;  A[1].y =  2;
	A[2].x =  3;  A[2].y =  1;
	A[3].x =  0;  A[3].y = -1;
	A[4].x = -2;  A[4].y =  1;
	A[5].x = -2;  A[5].y =  1;
	A[6].x = -1;  A[6].y =  2;
	//cout<<"4,0,1: "<<isClockwise(A[4], A[0], A[1])<<endl;
	//cout<<"0,1,2: "<<isClockwise(A[0], A[1], A[2])<<endl;
	cout<<"1,2,3: "<<isClockwise(A[1], A[2], A[3])<<endl;
	cout<<"2,3,4: "<<isClockwise(A[2], A[3], A[4])<<endl;
	cout<<"3,4,0: "<<isClockwise(A[3], A[4], A[0])<<endl;
	cout<<"Solution 1: "<< solution(vector<Point2D>(A, A+5))<<endl;
	cout<<"Solution 1: "<< solution(vector<Point2D>(A, A+7))<<endl;
	return 0;
}


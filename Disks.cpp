// Disks.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <limits.h>

int _tmain(int argc, _TCHAR* argv[])
{
	try{
		cout<<"Disks!"<<endl;
		int A[6];
		A[0] = 1;
		A[1] = 5;
		A[2] = 2;
		A[3] = 1;
		A[4] = 4;
		A[5] = 0;
		vector <int> input1(A, A + 6);
		int res = solution(input1);
		printVector<int>(input1);
		cout<<"res: " << res<<endl;
		int amount_intersects = 0;
		for(int i = 0; i < (int)input1.size();i++){
			for( int j = i+1; j < (int)input1.size();j++){
				cout<<i<<"\t"<<j<<"\t"<< inersect(i, j, input1)<<endl;
				amount_intersects += inersect(i, j, input1);
			}
		}
		cout<<"Overall ovelap pairs 1 are: "<<amount_intersects<<endl;
		cout<<"Overall ovelap pairs solution2 are: "<<solution2(input1)<<endl;
		cout<<"Overall ovelap pairs solution are: "<<solution(input1)<<endl;
	}
	catch (const out_of_range& e){
		cout<<"Range exception: "<<e.what()<<endl;
	}
	catch(...){
		cout<<"Unknown exception"<<endl;
	}
	return 0;
}

bool inersect(int i,int j, const vector<int> &A){
	return A.at(i)  >= abs(i-j) - A.at(j); 
}
bool Compare2(tuple<int, int, bool>  tupleCircle1, tuple<int, int, bool>  tupleCircle2){
	if (get<0>(tupleCircle1) != get<0>(tupleCircle2))
		return get<0>(tupleCircle1) < get<0>(tupleCircle2);
	if(get<2>(tupleCircle1) && !get<2>(tupleCircle2))
		return true;
	return get<1>(tupleCircle1) < get<1>(tupleCircle2);
}



bool CompareLeft(pair<int,int> left,pair<int,int> right){
	//first is an intesecton, second - number of circle
	if(left.first == right.first) 
		return left.second < right.second;
	return left.first < right.first;
}
bool CompareRight(pair<int,int> left,pair<int,int> right){
	//first is an intesecton, second - number of circle
	if(left.first == right.first) 
		return left.second > right.second;
	return left.first < right.first;
}
// https://codility.com/programmers/lessons/6-sorting/
// NumberOfDiscIntersections

int solution(vector<int> &A){
	set<int> iDisksWeAreInto;
	int res = 0;
	int tooMuch = -1;
	int limit = 10000000;
	vector<pair<int,int> >leftPoints, rightPoints;
	for(auto au = 0; au < (int)A.size(); au++) {
		leftPoints.push_back(pair<int,int> (au - A[au],au));
		rightPoints.push_back(pair<int,int> (au + A[au],au));
	}
	sort(leftPoints.begin(), leftPoints.end(),CompareLeft);
	sort(rightPoints.begin(), rightPoints.end(),CompareRight);
	auto aul = leftPoints.begin();
	auto aur = rightPoints.begin();
	while( aur != rightPoints.end()){
		if(aul == leftPoints.end()){
			return res;
		}
		if(aul->first <= aur->first){
			res += iDisksWeAreInto.size();
			if(res > limit)
				return tooMuch;
			iDisksWeAreInto.insert(aul->second);
			aul++;
		}
		else {
			iDisksWeAreInto.erase(aur->second);
			aur++;
		}
	}
	return res;
}


int solution2(vector<int> &A){
	vector<tuple<int, int, bool> > newCircles;
	int res = 0;
	for(int au = 0;au < (int)A.size(); au++){
		tuple<int, int, bool>  ncl = make_tuple(au-A.at(au), au, true);  //left point
		tuple<int, int, bool>  ncr = make_tuple(au+A.at(au), au, false);  //thre right point
		newCircles.push_back(ncl);
		newCircles.push_back(ncr);
	}
	sort(newCircles.begin(), newCircles.end(), Compare2);
	set<int> openCircles;
	for(auto au = newCircles.begin(); au != newCircles.end(); au++){
		bool left = get<2>(*au);
		int num = get<1>(*au);
		if(left){
			if(openCircles.find(num) != openCircles.end()){
				openCircles.erase(num);
			}
			else {
				res += openCircles.size();
				openCircles.insert(num);
			}
		}
		else{
			openCircles.erase(num);
		}
		if(res > 10000000 )
			return -1;
	}
	return res;
}
template<class T> void printVector(const vector<T>& res){
	std::copy(res.begin(), res.end(), ostream_iterator<T>(cout, " "));
	cout<<endl;
}
template<class T> void printSet(const set<T>& res){
	std::copy(res.begin(), res.end(), ostream_iterator<T>(cout, " "));
	cout<<endl;
}


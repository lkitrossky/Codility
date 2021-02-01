#include <vector>
#include <algorithm>
using namespace std;
//bodies
void fillRow(const vector<int> &A, int height, vector<int> &row) { //row: 0, 1 - empty, 1 - full
	const int N = A.size();
	row = vector<int>(N, 0);
	for (int i = 0; i < N; i++)
		if (A.at(i) > height)
			row.at(i) = 1;
}
int findFirstFilled(const vector<int> &row) {  //-1 on none
	const int N = row.size();
	for (int i = 0; i < N; i++)
		if (row.at(i) > 0)
			return i;
	return -1;
}
int findNextFilled(int start, const vector<int> &row) {  //-1 on none, start must be empty
	const int N = row.size();
	if (start < 0 || start >= N)
		return -1;
	if (row.at(start) > 0) //non-empty
		return -1; 
	if ( N-1 == start ) //the last
		return -1;
	for (int i = start + 1; i < N; i++)
		if (row.at(i) > 0)
			return i;
	return -1;
}
int findContinuousLength(int start, const vector<int> &row) {  //-1 on invalid, length of continuous
	const int N = row.size();
	if (start < 0 || start >= N)
		return -1;  //start is invalid
	if (0 == row.at(start))
		return -1; //start must be filled
	if (N - 1 == start)
		return 1; //the last in the row
	for (int i = start + 1; i < N; i++)
		if (0 == row.at(i))
			return i - start; //found end of continuous
	return N - start; //continouous till the end of the row
}
int findMaxContinuousInRow(const vector<int> &row) {
	const int N = row.size();
	int left = findFirstFilled(row);
	if (left < 0)
		return 0; 
	int currentLength = findContinuousLength(left, row);
	int res = currentLength;
	left = findNextFilled(left + currentLength, row);
	while (currentLength > 0 && currentLength < N && left >= 0 && left < N) {
		currentLength = findContinuousLength(left, row); //left start sequence
		res = max(res, currentLength); //possibly keep
		left += currentLength;  //move after sequence
		left = findNextFilled(left, row);  //find start of the next sequence
	}
	return res;
}
//https://app.codility.com/cert/view/certZQMR3J-7AKQ9XPQ7GRX3KVC/details/
//silver N**2, can be used as a checker
int solution1(const vector<int> &A) {
	const int N = A.size();
	int res = 0;
	for (int i = N - 1; i >= 0; i--) {
		if (res > i + 1)
			break; //cannot find bigger
		vector<int> row;
		fillRow(A, i, row);
		int maxLength = findMaxContinuousInRow(row);
		res = max(res, min(i+1, maxLength));
	}
	return res;
}
//another idea
/*
	Pass all data to pairs: height, location
	Prepare a connected list of columns
	Copy data and sort by height
	Start with big to small height, remove one by one and keep max size of the location gap
	If take min(max gap after excluded hight, excluded height). If bigger than result - replace result
	Still Silver!
	https://app.codility.com/cert/view/certZQMR3J-7AKQ9XPQ7GRX3KVC/details/
	Again O(N**2), strange...
	Replaced iterator by index, now it is golden
	https://app.codility.com/cert/view/certJ4FY2V-24GZYQ8RCFR3N3KG/
*/

#include <vector>
#include <algorithm>
using namespace std;
#define mypair pair<int, int> 
#define myvector vector<mypair>
bool mysort(mypair i, mypair j) { 
	if(i.first == j.first)
		return (i.second<j.second);
	return (i.first>j.first); 
} //sort 
void setVectorNeighbors(myvector &v, const vector<int> &A) { //every location knows its neighbors, including -1, N
	v.resize(0);
	for (int i = 0; i < (int)A.size(); i++)
		v.push_back(mypair(i-1, i+1));
}
void setVectorHeights(myvector &v, const vector<int> &A) { //pairs height, location
	v.resize(0);
	for (int i = 0; i < (int)A.size(); i++)
		v.push_back( mypair(A.at(i), i));
}
int solution(vector<int> &A) {
	const int N = A.size();
	int res = 1; //no column has height < 1
	myvector neighbors;
	setVectorNeighbors(neighbors, A); //on this stage we have a connected list, every location knows neighbors
	myvector heights;
	setVectorHeights(heights, A);
	sort(heights.begin(), heights.end(), mysort); //on this stage we have a sorted by height
	//the idea is as follows: we decrease height, every time remove one column
	//update neighbors and find created gap. 
	//min(gap, removed height)  - can create a square insiode removed
	//trying to update the result. 
	//finish: height < accumulated result, which isinitiated as 1
	for (int iter = 0; iter < (int)heights.size(); iter++) {
		int height = heights.at(iter).first;
		int location = heights.at(iter).second;
		if (height <= res)
			break; //we already found bigger, no chance
		int left = neighbors.at(location).first;
		int right = neighbors.at(location).second;
		if(left >= 0 && left < N)
			neighbors.at(left).second = right;
		if(right < N && right >= 0)
			neighbors.at(right).first = left;
		int gap = right - left - 1;
		res = max(res, min(gap, height));
	}
	return res;
}
#include <iostream>
ostream & operator << (ostream & os, const vector<int> &A) {
	for (auto iter = A.begin(); iter != A.end(); iter++)
		os << *iter << " ";
	return os;
}
ostream & operator << (ostream & os, const mypair &p) {
	os << p.first << ", " << p.second << "; ";
	return os;
}
ostream & operator << (ostream & os, const myvector &v) {
	for (auto iter = v.begin(); iter != v.end(); iter++)
		os << *iter;
	return os;
}
void drawSample(const vector<int> &A) {
	const int N = A.size();
	vector<int> row;
	for (int i = N -1; i >= 0; i--) {
		fillRow(A, i, row);
		cout << row << endl;
		/*
		cout << "Find first filled: "<< findFirstFilled(row) << endl;
		cout << "Length from 0: " << findContinuousLength(0, row) << endl;
		cout << "Length from 1: " << findContinuousLength(1, row) << endl;
		cout << "Length from 2: " << findContinuousLength(2, row) << endl;
		cout << "Length from 5: " << findContinuousLength(5, row) << endl;
		*/
	}
	cout << endl;
}
int drawSolve1(const vector<int> &A) {
	drawSample(A);
	int sol = solution1(A);
	cout << "Solution 1 gives: " << sol << endl;
	return sol;
}
int main() {
	cout << "The first solution, slow, N**2" << endl;
	int sample1[] = { 1,2,5,3,1,3 };
	int sample2[] = { 3,3,3,5,4 };
	int sample3[] = { 6,5,5,6,2,2};
	vector<int> vsample1(sample1, sample1 + sizeof(sample1) / sizeof(int));
	vector<int> vsample2(sample2, sample2 + sizeof(sample2) / sizeof(int));
	vector<int> vsample3(sample3, sample3 + sizeof(sample3) / sizeof(int));
	drawSolve1(vsample1);
	drawSolve1(vsample2);
	drawSolve1(vsample3);
	cout << "vsample1: " << solution(vsample1) << endl;
	cout << "vsample2: " << solution(vsample2) << endl;
	cout << "vsample3: " << solution(vsample3) << endl;
	return 0;
}
#include<vector>
#include<algorithm>

using namespace std;
// https://app.codility.com/programmers/challenges/niobium2019/
// Gold
// https://app.codility.com/cert/view/certPCB3KE-FHE93FNN8UZXGCDK/
/*Matrix A, consisting of N rows and M columns, is given, with each cell containing the value 0 or 1. 
Rows are numbered from 0 to N-1 (from top to bottom). 
Columns are numbered from 0 to M-1 (from left to right). The values inside the matrix can be changed: you can select as many columns as you want, and in the selected column(s), every value will be flipped (from 0 to 1, or from 1 to 0).
The goal is to obtain the maximum number of rows whose contents are all the same value (that is, we count rows with all 0s and rows with all 1s).
Write a function:
class Solution { public int solution(int[][] A); }
that, given matrix A, returns the maximum number of rows containing all the same values that can be obtained after flipping the selected columns.
*/
/*

First of all if some string can be brought by flipping some set of positions, 
than its ~ will do the same, just not to 000 but to 111 or vice versa
So, we can just flip strings to make 0 amount > 1 amont, if equal flip to start with 0.
In this conditions it is rather clear that if the same combination leads to all 0 for two strings, they are equal.
If so, enough to find the string that appears most.
Just sort loxicologically and pass through it , counting sequnces of equal. The longets is the answer.
*/

void flip(vector<int> &flipped) {
	for (unsigned int i = 0; i < flipped.size(); i++) {
		if (1 == flipped.at(i))
			flipped.at(i) = 0;
		else
			flipped.at(i) = 1;
	}
}
bool lessZeros(const vector<int> &flipped) {
	unsigned int zeroes = 0, units = 0;
	for (unsigned int i = 0; i < flipped.size(); i++) {
		if (1 == flipped.at(i))
			units++;
		else
			zeroes++;
	}
	if (zeroes < units)
		return true;
	else if (zeroes == units)
		return (1 == flipped.at(0)); //in this csse still flip it to prevent different but essentially the same
	else
		return false;
}
void normalize(vector< vector<int> > &A) {
	for (unsigned int i = 0; i < A.size(); i++) {
		//cout << A.at(i) << endl;
		if (lessZeros(A.at(i))) {	
			flip(A.at(i));	
		}
		//cout << A.at(i) << endl;
	}
}
bool sorting(const vector<int> & A1, const vector<int> & A2) {
	for (unsigned int i = 0; i < A1.size(); i++) {
		if (A1.at(i) < A2.at(i))
			return true;
		if (A1.at(i) > A2.at(i))
			return false;
	}
	return false;
}	

int solution(vector< vector<int> > &A) {
	//cout << "Got:" << endl;
	//cout << A << endl;
	normalize(A);
	//cout << "Normal:" << endl;
	//cout << A << endl;
	sort(A.begin(), A.end(), sorting);
	//cout << "Sorted:" << endl;
	//cout << A << endl;
	int res = 1;
	vector<int> last = A.at(0);
	int localMax = 1;
	for (unsigned int i = 1; i < A.size(); i++) {
		if (A.at(i) != last) {
			res = max(res, localMax);
			localMax = 1;
			last = A.at(i);
		}
		else {
			localMax++;
			res = max(res, localMax);
		}
	}
	return res;
}

//driver
#include <iostream>
ostream & operator<<(ostream & os, const vector<int> & vI);
ostream & operator << (ostream & os, const vector<vector<int> > & mva);

int main() {

	vector< vector<int> > A2;
	int A21[] = { 0, 1, 0, 1 };
	int A22[] = { 1, 0, 1, 0 };
	int A23[] = { 0, 1, 0, 1 };
	int A24[] = { 1, 0, 1, 0 };
	vector<int>VA21(A21, A21 + 4);
	vector<int>VA22(A22, A22 + 4);
	vector<int>VA23(A23, A23 + 4);
	vector<int>VA24(A24, A24 + 4);
	A2.push_back(VA21);
	A2.push_back(VA22);
	A2.push_back(VA23);
	A2.push_back(VA24);
	cout << "A21: " << solution(A2) << endl;

	vector< vector<int> > A1;
	int A11[] = { 0, 0, 0, 0 };
	int A12[] = { 0, 1, 0, 0 };
	int A13[] = { 1, 0, 1, 1 };
	vector<int>VA11(A11, A11 + 4);
	vector<int>VA12(A12, A12 + 4);
	vector<int>VA13(A13, A13 + 4);
	A1.push_back(VA11);
	A1.push_back(VA12);
	A1.push_back(VA13);
	cout << "A11: " << solution(A1) << endl;


	return 0;
}
ostream & operator<<(ostream & os, const vector<int> & vI) {
	for (unsigned int i = 0; i < vI.size(); i++) {
		os << vI.at(i);
		if (i + 1 < vI.size())
			cout << ",";
	}
	return os;
}
ostream & operator << (ostream & os, const vector<vector<int> > & mva) {
	for (unsigned int i = 0; i< mva.size(); i++) {
		os << mva.at(i) << endl;
	}
	return os;
}
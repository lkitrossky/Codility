/*
//https://app.codility.com/programmers/challenges/molybdenum2019/
//Gold
//https://app.codility.com/cert/view/cert9D7EY2-UYVMAXAJXH7JPB33/details/
//https://app.codility.com/cert/view/cert9D7EY2-UYVMAXAJXH7JPB33/
*/

using namespace std;
#include <vector>
#include <map>
#include <set>
#include <algorithm>
vector<int> solution(int K, int M, vector<int> &A) {
	int N = A.size();
	int leader = N / 2 + 1;
	map<int, int> current; 
	// Damian Werpachowski noted that this may result in O(M*log (M)) and not O(M). 
	//If map is replaced by vector it will be O(N+M), but for sparce array and small numbers it maybe even worse
	vector<int> res;
	set<int> resSet;
//initialization
	for (int i = 0; i < K; i++) {
		if (current.find(A.at(i) + 1) == current.end())
			current.insert(pair<int, int>(A.at(i) + 1, 1));
		else
			current.at(A.at(i) + 1)++;
		if (current.at(A.at(i) + 1) >= leader)
			resSet.insert(A.at(i) + 1);
	}
	for (int i = K; i < N; i++) {
		if (current.find(A.at(i) ) == current.end())
			current.insert(pair<int, int>(A.at(i) , 1));
		else
			current.at(A.at(i) )++;
		if (current.at(A.at(i) ) >= leader)
			resSet.insert(A.at(i) );
	}
//movement
	for (int i = K; i < N; i++) {
		current.at(A.at(i))--;
		if (current.find(A.at(i) + 1) == current.end())
			current.insert(pair<int,int>(A.at(i) + 1, 1));
		else
			current.at(A.at(i)+1)++;	
		if (current.find(A.at(i - K)) == current.end())
			current.insert(pair<int, int>(A.at(i - K), 1));
		else
			current.at(A.at(i - K))++;
		current.at(A.at(i-K) + 1)--;
		if (current.at(A.at(i) + 1) >= leader)
			resSet.insert(A.at(i) + 1);
		if (current.at(A.at(i - K)) >= leader)
			resSet.insert(A.at(i - K));
	}
	for (auto i = resSet.begin(); i != resSet.end(); i++)
		res.push_back(*i);
	sort(res.begin(), res.end());
	return res;
}

//driver
#include <iostream>
void printV(vector<int> &A) {
	for (auto i = A.begin(); i != A.end(); i++)
		cout << *i << " ";
	cout << endl;
}
int main() {
	int A[7];
	A[0] = 2; A[1] = 1; A[2] = 3; A[3] = 1; A[4] = 2; A[5] = 2; A[6] = 3;
	vector<int> AV(A, A + 7);
	vector<int> res = solution(3, 5, AV);
	printV(AV);
	printV(res);
	int A2[5];
	A2[0] = 1; A2[1] = 2; A2[2] = 2; A2[3] = 1; A2[4] = 2;
	vector<int> AV2(A2, A2 + 5);
	vector<int> res2 = solution(4, 2, AV2);
	printV(AV2);
	printV(res2);
	return 0;
}
/*
(3,5,[ 2, 1,  3, 1, 2, 2,  3])
(4,2,[ 1, 2, 2, 1, 2])
(1,2,[ 1, 2, 2, 1, 2])
(1,2,[ 1, 2, 2, 1, 2, 3])
(1,2,[ 1, 2, 2, 1, 2, 3, 4, 5])
*/

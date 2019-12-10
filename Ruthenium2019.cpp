//https://app.codility.com/programmers/challenges/ruthenium2019/
//https://app.codility.com/cert/view/certQ7XZTA-ZEMM5DXPM63BVH58/
//Gold!
//2015 started 24 silver awards 109 golden awards, me 83d. Best times 8,20,21 min
//in essence this is caterpiller but done simultaniously on many types
//and without preliminary build of the array, immediate handling
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;
typedef pair<int, int> Island;
class Archip2 {
public:
	Archip2(int _limit, int K);
	void add(const Island & island) { archip.push_back(island); }
	int add(int _start, int _end);
private:
	int limit, left, right, currentK, continuity;
	const int K;
	vector<Island> archip;
	int islandSize(int index)const;
	int leftSpace(int index)const;
	int rightSpace(int index)const;
	int spacesAround(int indexLeft, int indexRight)const;
};
Archip2::Archip2(int _limit, int _K) :limit(_limit), K(_K) {
	archip = vector<Island>();
	continuity = 0;
	left = 0;
	right = -1;
	currentK = _K;
}
int Archip2::islandSize(int index) const {
	if (index >= 0 && index < (int)archip.size())
		return archip.at(index).second - archip.at(index).first;
	return 0;
}
int Archip2::leftSpace(int index)const {
	if (0 == index)
		return archip.at(0).first;
	return archip.at(index).first - archip.at(index - 1).second;
}
int Archip2::rightSpace(int index)const {
	if ((int)archip.size() - 1 == index)
		return limit - archip.at(index).second;
	return archip.at(index + 1).first - archip.at(index).second;
}
int Archip2::spacesAround(int indexLeft, int indexRight)const {
	return leftSpace(indexLeft) + rightSpace(indexRight);
}
int Archip2::add(int _start, int _end) {
	archip.push_back(Island(_start, _end));
	right++;
	continuity += islandSize(right);
	if (archip.size() > 1) {
		continuity += leftSpace(right);
		currentK -= leftSpace(right);
		while (currentK < 0 ) { 
			continuity -= islandSize(left) + rightSpace(left);
			currentK += rightSpace(left);
			left++;
		}
	}
	return continuity + min(currentK, spacesAround(left, right));
}
int solution(vector<int> &A, int K) {
	int res = 0;
	map<int, Archip2> mapArchips;
	if (A.size() < 2)
		return 1;
	int currentLeft = 0;
	int currentRight = 1;
	while (currentRight < (int)A.size()) {
		if (A.at(currentRight) != A.at(currentRight - 1)) {
			if (mapArchips.end() == mapArchips.find(A.at(currentRight - 1))) {
				mapArchips.insert(pair<int, Archip2>(A.at(currentRight - 1), Archip2(A.size(), K)));
			}
			res = max(res, mapArchips.find(A.at(currentRight - 1))->second.add(currentLeft, currentRight));
			currentLeft = currentRight;
		}
		currentRight++;
	}
	if (mapArchips.end() == mapArchips.find(A.at(currentRight - 1))) {
		mapArchips.insert(pair<int, Archip2>(A.at(currentRight - 1), Archip2(A.size(), K)));
	}
	res = max(res, mapArchips.find(A.at(currentRight - 1))->second.add(currentLeft, currentRight));
	return res;
}
//see main below
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*
1. Given A = [1, 1, 3, 4, 3, 3, 4] and K = 2, the function should return 5. Teachers can replace books from age 4 with books from age 3.

2. Given A = [4, 5, 5, 4, 2, 2, 4] and K = 0, the function should return 2. Teachers are not allowed to replace any books.

3. Given A = [1, 3, 3, 2] and K = 2, the function should return 4. Teachers can replace all the books from other ages with books from age 3.
*/
#include <iostream>
ostream & operator << (ostream & os, const vector<int> vb) {
	for (unsigned int i = 0; i< vb.size(); i++) {
		os << vb.at(i) << " ";
	}
	return os;
}
int main() {
	int K = 0;
	int res1 = 0;
	vector<int>v1;
	int arr5[] = { 4,1,4,4,1,4 };
	v1 = vector<int>(arr5, arr5 + sizeof arr5 / sizeof(int));
	K = 2;
	res1 = solution(v1, K);
	cout << v1 << " K: " << K << " res: " << res1 << endl;
	int arr1[] = { 1, 1, 3, 4, 3, 3, 4 };
	v1 = vector<int>(arr1, arr1 + 7);
	K = 2;
	res1 = solution(v1, K);
	cout << v1 << " K: " << K << " res: " << res1 << endl;
	int arr2[] = { 4, 5, 5, 4, 2, 2, 4 };
	v1 = vector<int>(arr2, arr2 + 7);
	K = 0;
	res1 = solution(v1, K);
	cout << v1 << " K: " << K << " res: " << res1 << endl;
	int arr3[] = { 1,3,3,2 };
	v1 = vector<int>(arr3, arr3 + 4);
	K = 2;
	res1 = solution(v1, K);
	cout << v1 << " K: " << K << " res: " << res1 << endl;
	int arr4[] = { 3,5,3,3 };
	v1 = vector<int>(arr4, arr4 + sizeof arr4 / sizeof(int));
	K = 0;
	res1 = solution(v1, K);
	cout << v1 << " K: " << K << " res: " << res1 << endl;
	return 0;
}

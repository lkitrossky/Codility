//https://app.codility.com/cert/view/certXW943K-557CWZAYNDZXMR4U/
//https://app.codility.com/programmers/challenges/bromum2018/
//Golden, easy


#include <vector>
#include <map>
using namespace std;

const long long MAXI = 1000001;
int solution(int N, int Q, vector<int> &B, vector<int> &C) {
	map<long long, int> mymap;
	long long key = 0;
	for ( int i = 0; i < B.size(); i++) {
		key = B.at(i);
		key *= MAXI;
		key += C.at(i);
		auto au = mymap.find(key);
		if (au == mymap.end()) {
			mymap.insert(pair<long long, int>(key, 1));
			au = mymap.find(key);
		}
		else {
			au->second++;
		}
		if (au->second == Q)
			return i;
	}
	return -1;
}

#include <iostream>
void test(int N, int Q, vector<int> &B, vector<int> &C, int expected){
	int res = solution(N, Q, B, C);
	if (res != expected) {
		cout << "Test failed!! Expected: " << expected << " Calc:" << res << endl;
	}
}
int main() {
	int N = 3, Q = 2;
	vector<int>B{ 1, 2, 0, 1, 1, 0, 0, 1 }, C{ 0, 3, 0, 2, 0, 3, 0, 0 };
	test(N, Q, B, C, 4);
	N = 2; Q = 2; B = vector<int>{ 0, 1 }; C = vector<int>{ 5, 5 };
	test(N, Q, B, C, -1);
	N = 2; Q = 2; B = vector<int>{ 0, 1, 0, 1, 0, 1 }; C = vector<int>{ 1, 3, 0, 0, 3, 3 };
	test(N, Q, B, C, 5);

	return 0;
}
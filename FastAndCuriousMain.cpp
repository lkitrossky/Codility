#include <vector>
#include <cmath>
using namespace std;
/*
Fast And Curious Codility Challenge July 2021 
https://app.codility.com/programmers/custom_challenge/fast_and_curious_2021/
https://app.codility.com/cert/view/certWWSXUU-664NGVEQH3FRMGR5/details/   - - failed in some cases, modulus must be at the end, 
first check that there is no overflow after passing to uint64_t, the max number is 10^5*10^9 which is less that max uint64_t:18446744073709551615 > 19 signs
https://app.codility.com/cert/view/cert682F9H-5TCXYAVJ5YHV6TSN/details/ - gold
*/
const uint64_t MODULUS = (uint64_t)pow(10, 9) + 7;
uint64_t remainingWay(vector<uint64_t>& A, size_t i) {
	size_t L = A.size();
	uint64_t remainingWay = A.at(L - 1) - A.at(i);
	return remainingWay;
}
int solution(vector<int>& _A) {
	size_t L = _A.size();
	vector<uint64_t> A(L);
	for (size_t i = 0; i < L; i++)
		A.at(i) = (uint64_t)_A.at(i);
	uint64_t sum = 0;
	for (size_t i = 0; i < L; i++) {
		uint64_t remaining = remainingWay(A, i);
		sum = sum + remaining;
	}
	uint64_t maxEconomy = 0;
	for (size_t i = 0; i < L; i++) {
		uint64_t remaining = remainingWay(A, i);
		uint64_t currentEconomy = ((uint64_t)i + 1) * (uint64_t)remaining;
		if (currentEconomy > maxEconomy) {
			maxEconomy = currentEconomy;
		}
	}
	return (int)((sum - maxEconomy)% MODULUS);
}




#include  <iostream>
int main() {
	cout << "max uint64_t\t:" << numeric_limits<uint64_t>::max() << endl;
	cout << "max int\t\t:" << numeric_limits<int>::max() << endl;
	cout << "max size_t\t:" << numeric_limits<size_t>::max() << endl;
	cout << "MODULUS\t\t:" << MODULUS << endl;
	cout << "4500000000%MODULUS:" << 4500000000 % MODULUS << ", Must be: 499999972, diff = " << 499999972 - (4500000000 % MODULUS) << endl;
	cout << "Max number in the calc:" << pow(10, 9) * pow(10, 5) << endl;
	int A5[] = { 0, 1000, 1000000000 };
	vector<int>VA5(A5, A5 + sizeof(A5) / sizeof(int));
	cout << solution(VA5) << endl;
	vector<int> VA4(20);
	for (int i = 0; i < 20; i++)
		VA4.at(i) = i * 5 *(int) pow(10 , 7);
	cout << solution(VA4) << endl;
	int A3[] = {2, 6, 7, 8, 12};
	vector<int>VA3(A3, A3 + sizeof(A3) / sizeof(int));
	cout << solution(VA3) << endl;
	int A2[] = { 5, 15 };
	vector<int>VA2(A2, A2 + sizeof(A2) / sizeof(int));
	cout << solution(VA2) << endl;
	int A1[] = { 1, 5, 9, 12 };
	vector<int>VA1(A1, A1 + sizeof(A1) / sizeof(int));
	cout << solution(VA1) << endl;
	return 0;
}
// Selenium2018.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////////
//			BEGINNG OF SOLUTION
// Gold medal   https://app.codility.com/cert/view/certGZEKZJ-BDVHM9F6ETVBVEUP/
////////////////////////////////////////////////////////////////////////////////

#include <vector>
//#include <algorithm>
#include <queue>
using namespace std;
#define MODULO 1000000007


unsigned long long solution(vector<int> &A) {
	unsigned long long res = 0;
	const unsigned int len = (unsigned int)A.size();
	//we need queue but the first version is with vector
	//vector< pair<unsigned int, unsigned int> > depot;  //sprinklers to move, first - index of location, second - amount
	//vector< unsigned int > depotOfEmpties;  //Vacant places
	queue< pair<unsigned int, unsigned int> > depot;  //sprinklers to move, first - index of location, second - amount
	queue< unsigned int > depotOfEmpties;  //Vacant places
	vector<unsigned int>places(len, 0);  //how many sprinkles have coordinate i
	for (unsigned int i = 0; i < len; i++) {
		places.at(A.at(i) - 1)++;
	}
	for (unsigned int i = 0; i < len; i++) {
		if (0 == places.at(i)) {
			if (depot.empty()) {
				depotOfEmpties.push(i);
			}
			else {
				unsigned int distance = (i - depot.front().first) ;
				res += distance;
				res = res % MODULO;
				depot.front().second--;
				while (!depot.empty() && depot.front().second < 1) {
					depot.pop();
				}
			}
		} else {
			if (1 == places.at(i)) {
				continue;  //this sprinkler remains in its place, only one
			}
			else {  
				unsigned int pieces = places.at(i) - 1; //amount to move
				unsigned int lenEmptySequence  = depotOfEmpties.size();
				if (pieces <= lenEmptySequence) {  //all to move can be placed in empty met already
					for (unsigned int j = 0; j < pieces; j++) {
						res = (res + i - depotOfEmpties.front()) % MODULO;
						depotOfEmpties.pop();
					}
					
				}  
				else {  //empty are not enough, including case of 0
					for (unsigned int j = 0; j < lenEmptySequence; j++) {
						res = (res + i - depotOfEmpties.front()) % MODULO;
						depotOfEmpties.pop();
					}
					depot.push(pair<unsigned int, unsigned int>(i, pieces - lenEmptySequence));
				}
			}
		}

	}
	return res;
}
int solution(vector<int> &A, vector<int> &B) {
	unsigned long long res = solution(B);
	res += solution(A);
	res = res % MODULO;
	return (int)res;
}



////////////////////////////////////////////////////////////////////
//   Driver and tests
////////////////////////////////////////////////////////////////////

#include <iostream>
void test(vector<int> &A, vector<int> &B, int expected, bool printAll = false, bool doNotPrint = true) {
	int res = solution(A, B);
	if ((expected != res && !doNotPrint) || printAll) {
		for (size_t i = 0; i < A.size(); i++) {
			cout << A.at(i) << " ";
		}
		cout << endl;
		for (size_t i = 0; i < B.size(); i++) {
			cout << B.at(i) << " ";
		}
		cout << endl;
		if (expected != res)
			cout << "Error! Expected: " << expected << "  ";
		else
			cout << "Expected: " << expected << "  ";
	}
	cout << " Result: " << res << endl;
}
unsigned long long squareFunction(int k) {
	unsigned long long half;
	unsigned long long res = (k*k) % MODULO;
	if (1 == k % 2) {
		half = (k - 1) / 2;
		res = (res * k) % MODULO;
		res = (res * half) % MODULO;
	}
	else {
		half = (k) / 2;
		res = (res * (k-1)) % MODULO;
		res = (res * half) % MODULO;
	}
	return res;
}
bool testSquareCase(int k, vector <int> &A) {
	long long res1 = squareFunction(k);
	long long res2 = solution(A);
	bool res = (res1 != res2);
	if (res1 != res2)
		cout << "Error! Fromula gives: "<<res1 << " algo gives: "<< res2 << " for k = "<< k <<" Shift: "<<A[0]<< endl;
	else
		cout << "K = " << k << " Res: " << res1 << " Shift: " << A[0] << " 2D answer: "<< (2*res1)%MODULO << endl;
	return res;
}
void testOneD1(vector<int> & A, int expected, bool print = true) {
	unsigned long long res = solution(A);
	if (res != expected ) {
		cout << "Error! Expected: " << expected << " " << " got: "<<res << endl;
		for (auto a : A) {
			cout << a << " " ;
		}
		cout << endl;
	}
}
void testOneD() {
	const int limit = 10000;
	const int minLimit = 2;
	for(int j = 0; j < 10; j++)
	{
		int l = (minLimit + rand()) % limit + 2;
		vector<int> A(l, 0);
		for (auto & a : A) {
			a = rand() % l + 1;
		}
		unsigned long long res = solution(A);
		cout << "#" << j << "\t"<< l<<"\t" << res << endl;
		if (164648 == res) {
			for (auto it = A.begin(); it != A.end(); it++) {
				cout << *it << " ";
			}
			cout << "Found " << res << endl;
			break;
		}
	}
}
void easyTests() {
	vector<int>VA;
	vector<int>VB;
	VA.push_back(2);
	VA.push_back(2);
	VA.push_back(4);
	VA.push_back(4);
	VB.push_back(1);
	VB.push_back(2);
	VB.push_back(2);
	VB.push_back(3);
	test(VA, VB, 4, true, false);
	int A[] = {2,2,2,2};
	int B[] = { 1,2,3,4 };
	VA = vector<int>(A, A + 3);
	VB = vector<int>(B, B + 3);
	test(VA, VB, 2, true, false);
	int A1[] = { 2,3,3,3 };
	int B1[] = { 1,1,2,3 };
	VA = vector<int>(A1, A1 + 4);
	VB = vector<int>(B1, B1 + 4);
	test(VA, VB, 6, true, false);
	int A2[] = { 2,3,3,3, 4 };
	int B2[] = { 1,1,2,3, 1 };
	VA = vector<int>(A2, A2 + 5);
	VB = vector<int>(B2, B2 + 5);
	test(VA, VB, 11, true, false);
	int A3[] = { 3,3,3,3,4 };
	int B3[] = { 1,2,3,4,1 };
	VA = vector<int>(A3, A3 + 5);
	VB = vector<int>(B3, B3 + 5);
	test(VA, VB, 9, true, false);
	int A4[] = { 1,3,3,3,5 };
	int B4[] = { 1,1,2,3,1 };
	VA = vector<int>(A4, A4 + 5);
	VB = vector<int>(B4, B4 + 5);
	test(VA, VB, 9, true, false);
	int A5[] = { 1,3,3,4,4 };
	int B5[] = { 1,2,3,2,3 };
	VA = vector<int>(A5, A5 + 5);
	VB = vector<int>(B5, B5 + 5);
	test(VA, VB, 6, true, false);
	int A6[] = { 2,2,3,3 };
	int B6[] = { 1,2,1,2 };
	VA = vector<int>(A6, A6 + 4);
	VB = vector<int>(B6, B6 + 4);
	test(VA, VB, 6, true, false);
	int A7[] = { 2, 2, 3, 4, 4 };
	int B7[] = { 2, 4, 3, 2, 4 };
	VA = vector<int>(A7, A7 + 5);
	VB = vector<int>(B7, B7 + 5);
	test(VA, VB, 4, true, false);

	int A8[] = { 7,7,7,7,8,8,8,8 };
	int B8[] = { 1,2,3,4,1,2,3,4 };
	VA = vector<int>(A8, A8 + 8);
	VB = vector<int>(B8, B8 + 8);
	test(VA, VB, 40, true, false);
}
void easyTest1D() {
	int A1[] = {2,3,3,3,4};
	testOneD1(vector<int>(A1, A1+5), 4, true);
	int A2[] = {2, 4, 6,6,6, 6};
	testOneD1(vector<int>(A2, A2 + 6), 9, true);
	int A3[] = {1};
	int A4[] = {1};
	int A5[] = {1};

}

int main()
{
	easyTest1D();
	//return 0;

	easyTests();
	//return 0;
	vector<int>VA;
	vector<int>VB;

	//testOneD();
	//return 0;

	int A4[] = { 1, 2, 2, 3, 4 };
	int B4[] = { 1, 1, 4, 5, 4 };
	VA = vector<int>(A4, A4 + 5);
	VB = vector<int>(B4, B4 + 5);
	test(VA, VB, 5);
	reverse(VA.begin(), VA.end());
	reverse(VB.begin(), VB.end());
	test(VA, VB, 5);

	int A0[] = { 1, 1, 1, 1 };
	int B0[] = { 1,2,3,4 };
	VA = vector<int>(A0, A0 + 4);
	VB = vector<int>(B0, B0 + 4);
	test(VA, VB, 6);
	reverse(VA.begin(), VA.end());
	reverse(VB.begin(), VB.end());
	test(VA, VB, 6);

	int A2[] = {1, 1, 2 };
	int B2[] = { 1, 2, 1 };
	VA = vector<int>(A2, A2 + 3);
	VB = vector<int>(B2, B2 + 3);
	test(VA, VB, 4);
	reverse(VA.begin(), VA.end());
	reverse(VB.begin(), VB.end());
	test(VA, VB, 4);

	//square case
	int A3[] = { 1, 2, 3, 1, 2, 3, 1, 2, 3};
	int B3[] = { 1, 1, 1, 2 ,2,2, 3,3,3 };
	VA = vector<int>(A3, A3 + 9);
	VB = vector<int>(B3, B3 + 9);
	test(VA, VB, 54);
	//also
	int A5[] = { 7, 8, 9, 7, 8, 9, 7, 8, 9 };
	int B5[] = { 7, 7, 7, 8 ,8,8, 9,9,9 };
	VA = vector<int>(A5, A5 + 9);
	VB = vector<int>(B5, B5 + 9);
	test(VA, VB, 54);

	int A6[] = { 1,1,2,3 };
	int B6[] = { 1,2,3,4 };
	VA = vector<int>(A6, A6 + 4);
	VB = vector<int>(B6, B6 + 4);
	test(VA, VB, 3);
	test(VB, VA, 3);

	int A7[] = { 1,1,3,5, 5 };
	int B7[] = { 1,5,3,1, 5};
	VA = vector<int>(A7, A7 + 5);
	VB = vector<int>(B7, B7 + 5);
	test(VA, VB, 4);
	test(VB, VA, 4);

	//smaller square case
	int A8[] = { 1,2,1,2 };
	int B8[] = { 1,1,2,2 };
	VA = vector<int>(A8, A8 + 4);
	VB = vector<int>(B8, B8 + 4);
	test(VA, VB, 8);

	int A9[] = { 3,4,3,4 };
	int B9[] = { 3,3,4,4 };
	VA = vector<int>(A9, A9 + 4);
	VB = vector<int>(B9, B9 + 4);
	test(VA, VB, 8);

	int A10[] = {1,2,3,4,1,2,3,4, 1,2,3,4,1,2,3,4};
	int B10[] = {1,1,1,1,2,2,2,2, 3,3,3,3,4,4,4,4};
	VA = vector<int>(A10, A10 + 16);
	VB = vector<int>(B10, B10 + 16);
	test(VA, VB, 192);

	int A11[] = { 13,14,15,16, 13,14,15,16,  13,14,15,16, 13,14,15,16 };
	int B11[] = { 13,13,13,13, 14,14,14,14,  15,15,15,15, 16,16,16,16 };
	VA = vector<int>(A11, A11 + 16);
	VB = vector<int>(B11, B11 + 16);
	test(VA, VB, 192);


	return 0;
	unsigned int square = 1000;
	unsigned int ssquare = square*square;
	VA = vector<int>(ssquare,0);
	VB = vector<int>(ssquare,0);
	for (unsigned int i = 0; i < ssquare; i++) {
		VA.at(i) = i % square + 1;
		VB.at(i) = i / square + 1;
	}
	
	unsigned int shift = 0;
	unsigned int k = 781;
	for (unsigned int j = 2; j <= 316; j += 2) {
		vector <int> AA(j*j, 0);
		for (unsigned int i = 0; i < j*j; i++) {
			AA.at(i) = (i + shift) % j + 1;
		}
		if (testSquareCase(j, AA))
			break;
		vector <int> AA2((j+1)*(j+1), 0);
		for (unsigned int i = 0; i < (j + 1)*(j + 1); i++) {
			AA2.at(i) = (i + shift) % (j+1) + 1;
		}
		if (testSquareCase(j+1, AA2))
			break;
	}
	/*
	AA = vector <int>(k*k, 0);
	for (unsigned int i = 0; i < k*k; i++) {
		AA.at(i) = k*k - k + i % k + 1;
	}
	testSquareCase(k, AA);
	*/
	return 0;
}


/*
My squares

Result: 5
Result: 5
Result: 6
Result: 6
Result: 4
Result: 4
Result: 54
Result: 54
Result: 3
Result: 3
Result: 4
Result: 4
Result: 8
Result: 8
Result: 192
Result: 192
K = 2 Res: 4 Shift: 1 2D answer: 8
K = 3 Res: 27 Shift: 1 2D answer: 54
K = 4 Res: 96 Shift: 1 2D answer: 192
K = 5 Res: 250 Shift: 1 2D answer: 500
K = 6 Res: 540 Shift: 1 2D answer: 1080
K = 7 Res: 1029 Shift: 1 2D answer: 2058
K = 8 Res: 1792 Shift: 1 2D answer: 3584
K = 9 Res: 2916 Shift: 1 2D answer: 5832
K = 10 Res: 4500 Shift: 1 2D answer: 9000
K = 11 Res: 6655 Shift: 1 2D answer: 13310
K = 12 Res: 9504 Shift: 1 2D answer: 19008
K = 13 Res: 13182 Shift: 1 2D answer: 26364
K = 14 Res: 17836 Shift: 1 2D answer: 35672
K = 15 Res: 23625 Shift: 1 2D answer: 47250
K = 16 Res: 30720 Shift: 1 2D answer: 61440
K = 17 Res: 39304 Shift: 1 2D answer: 78608
K = 18 Res: 49572 Shift: 1 2D answer: 99144
K = 19 Res: 61731 Shift: 1 2D answer: 123462
K = 20 Res: 76000 Shift: 1 2D answer: 152000
K = 21 Res: 92610 Shift: 1 2D answer: 185220
K = 22 Res: 111804 Shift: 1 2D answer: 223608
K = 23 Res: 133837 Shift: 1 2D answer: 267674
K = 24 Res: 158976 Shift: 1 2D answer: 317952
K = 25 Res: 187500 Shift: 1 2D answer: 375000
K = 26 Res: 219700 Shift: 1 2D answer: 439400
K = 27 Res: 255879 Shift: 1 2D answer: 511758
K = 28 Res: 296352 Shift: 1 2D answer: 592704
K = 29 Res: 341446 Shift: 1 2D answer: 682892
K = 30 Res: 391500 Shift: 1 2D answer: 783000
K = 31 Res: 446865 Shift: 1 2D answer: 893730
K = 32 Res: 507904 Shift: 1 2D answer: 1015808
K = 33 Res: 574992 Shift: 1 2D answer: 1149984
K = 34 Res: 648516 Shift: 1 2D answer: 1297032
K = 35 Res: 728875 Shift: 1 2D answer: 1457750
K = 36 Res: 816480 Shift: 1 2D answer: 1632960
K = 37 Res: 911754 Shift: 1 2D answer: 1823508
K = 38 Res: 1015132 Shift: 1 2D answer: 2030264
K = 39 Res: 1127061 Shift: 1 2D answer: 2254122
K = 40 Res: 1248000 Shift: 1 2D answer: 2496000
K = 41 Res: 1378420 Shift: 1 2D answer: 2756840
K = 42 Res: 1518804 Shift: 1 2D answer: 3037608
K = 43 Res: 1669647 Shift: 1 2D answer: 3339294
K = 44 Res: 1831456 Shift: 1 2D answer: 3662912
K = 45 Res: 2004750 Shift: 1 2D answer: 4009500
K = 46 Res: 2190060 Shift: 1 2D answer: 4380120
K = 47 Res: 2387929 Shift: 1 2D answer: 4775858
K = 48 Res: 2598912 Shift: 1 2D answer: 5197824
K = 49 Res: 2823576 Shift: 1 2D answer: 5647152
K = 50 Res: 3062500 Shift: 1 2D answer: 6125000
K = 51 Res: 3316275 Shift: 1 2D answer: 6632550
K = 52 Res: 3585504 Shift: 1 2D answer: 7171008
K = 53 Res: 3870802 Shift: 1 2D answer: 7741604
K = 54 Res: 4172796 Shift: 1 2D answer: 8345592
K = 55 Res: 4492125 Shift: 1 2D answer: 8984250
K = 56 Res: 4829440 Shift: 1 2D answer: 9658880
K = 57 Res: 5185404 Shift: 1 2D answer: 10370808
K = 58 Res: 5560692 Shift: 1 2D answer: 11121384
K = 59 Res: 5955991 Shift: 1 2D answer: 11911982
K = 60 Res: 6372000 Shift: 1 2D answer: 12744000
K = 61 Res: 6809430 Shift: 1 2D answer: 13618860
K = 62 Res: 7269004 Shift: 1 2D answer: 14538008
K = 63 Res: 7751457 Shift: 1 2D answer: 15502914
K = 64 Res: 8257536 Shift: 1 2D answer: 16515072
K = 65 Res: 8788000 Shift: 1 2D answer: 17576000
K = 66 Res: 9343620 Shift: 1 2D answer: 18687240
K = 67 Res: 9925179 Shift: 1 2D answer: 19850358
K = 68 Res: 10533472 Shift: 1 2D answer: 21066944
K = 69 Res: 11169306 Shift: 1 2D answer: 22338612
K = 70 Res: 11833500 Shift: 1 2D answer: 23667000
K = 71 Res: 12526885 Shift: 1 2D answer: 25053770
K = 72 Res: 13250304 Shift: 1 2D answer: 26500608
K = 73 Res: 14004612 Shift: 1 2D answer: 28009224
K = 74 Res: 14790676 Shift: 1 2D answer: 29581352
K = 75 Res: 15609375 Shift: 1 2D answer: 31218750
K = 76 Res: 16461600 Shift: 1 2D answer: 32923200
K = 77 Res: 17348254 Shift: 1 2D answer: 34696508
K = 78 Res: 18270252 Shift: 1 2D answer: 36540504
K = 79 Res: 19228521 Shift: 1 2D answer: 38457042
K = 80 Res: 20224000 Shift: 1 2D answer: 40448000
K = 81 Res: 21257640 Shift: 1 2D answer: 42515280
K = 82 Res: 22330404 Shift: 1 2D answer: 44660808
K = 83 Res: 23443267 Shift: 1 2D answer: 46886534
K = 84 Res: 24597216 Shift: 1 2D answer: 49194432
K = 85 Res: 25793250 Shift: 1 2D answer: 51586500
K = 86 Res: 27032380 Shift: 1 2D answer: 54064760
K = 87 Res: 28315629 Shift: 1 2D answer: 56631258
K = 88 Res: 29644032 Shift: 1 2D answer: 59288064
K = 89 Res: 31018636 Shift: 1 2D answer: 62037272
K = 90 Res: 32440500 Shift: 1 2D answer: 64881000
K = 91 Res: 33910695 Shift: 1 2D answer: 67821390
K = 92 Res: 35430304 Shift: 1 2D answer: 70860608
K = 93 Res: 37000422 Shift: 1 2D answer: 74000844
K = 94 Res: 38622156 Shift: 1 2D answer: 77244312
K = 95 Res: 40296625 Shift: 1 2D answer: 80593250
K = 96 Res: 42024960 Shift: 1 2D answer: 84049920
K = 97 Res: 43808304 Shift: 1 2D answer: 87616608
K = 98 Res: 45647812 Shift: 1 2D answer: 91295624
K = 99 Res: 47544651 Shift: 1 2D answer: 95089302
K = 100 Res: 49500000 Shift: 1 2D answer: 99000000
K = 101 Res: 51515050 Shift: 1 2D answer: 103030100
K = 102 Res: 53591004 Shift: 1 2D answer: 107182008
K = 103 Res: 55729077 Shift: 1 2D answer: 111458154
K = 104 Res: 57930496 Shift: 1 2D answer: 115860992
K = 105 Res: 60196500 Shift: 1 2D answer: 120393000
K = 106 Res: 62528340 Shift: 1 2D answer: 125056680
K = 107 Res: 64927279 Shift: 1 2D answer: 129854558
K = 108 Res: 67394592 Shift: 1 2D answer: 134789184
K = 109 Res: 69931566 Shift: 1 2D answer: 139863132
K = 110 Res: 72539500 Shift: 1 2D answer: 145079000
K = 111 Res: 75219705 Shift: 1 2D answer: 150439410
K = 112 Res: 77973504 Shift: 1 2D answer: 155947008
K = 113 Res: 80802232 Shift: 1 2D answer: 161604464
K = 114 Res: 83707236 Shift: 1 2D answer: 167414472
K = 115 Res: 86689875 Shift: 1 2D answer: 173379750
K = 116 Res: 89751520 Shift: 1 2D answer: 179503040
K = 117 Res: 92893554 Shift: 1 2D answer: 185787108
K = 118 Res: 96117372 Shift: 1 2D answer: 192234744
K = 119 Res: 99424381 Shift: 1 2D answer: 198848762
K = 120 Res: 102816000 Shift: 1 2D answer: 205632000
K = 121 Res: 106293660 Shift: 1 2D answer: 212587320
K = 122 Res: 109858804 Shift: 1 2D answer: 219717608
K = 123 Res: 113512887 Shift: 1 2D answer: 227025774
K = 124 Res: 117257376 Shift: 1 2D answer: 234514752
K = 125 Res: 121093750 Shift: 1 2D answer: 242187500
K = 126 Res: 125023500 Shift: 1 2D answer: 250047000
K = 127 Res: 129048129 Shift: 1 2D answer: 258096258
K = 128 Res: 133169152 Shift: 1 2D answer: 266338304
K = 129 Res: 137388096 Shift: 1 2D answer: 274776192
K = 130 Res: 141706500 Shift: 1 2D answer: 283413000
K = 131 Res: 146125915 Shift: 1 2D answer: 292251830
K = 132 Res: 150647904 Shift: 1 2D answer: 301295808
K = 133 Res: 155274042 Shift: 1 2D answer: 310548084
K = 134 Res: 160005916 Shift: 1 2D answer: 320011832
K = 135 Res: 164845125 Shift: 1 2D answer: 329690250
K = 136 Res: 169793280 Shift: 1 2D answer: 339586560
K = 137 Res: 174852004 Shift: 1 2D answer: 349704008
K = 138 Res: 180022932 Shift: 1 2D answer: 360045864
K = 139 Res: 185307711 Shift: 1 2D answer: 370615422
K = 140 Res: 190708000 Shift: 1 2D answer: 381416000
K = 141 Res: 196225470 Shift: 1 2D answer: 392450940
K = 142 Res: 201861804 Shift: 1 2D answer: 403723608
K = 143 Res: 207618697 Shift: 1 2D answer: 415237394
K = 144 Res: 213497856 Shift: 1 2D answer: 426995712
K = 145 Res: 219501000 Shift: 1 2D answer: 439002000
K = 146 Res: 225629860 Shift: 1 2D answer: 451259720
K = 147 Res: 231886179 Shift: 1 2D answer: 463772358
K = 148 Res: 238271712 Shift: 1 2D answer: 476543424
K = 149 Res: 244788226 Shift: 1 2D answer: 489576452
K = 150 Res: 251437500 Shift: 1 2D answer: 502875000
K = 151 Res: 258221325 Shift: 1 2D answer: 516442650
K = 152 Res: 265141504 Shift: 1 2D answer: 530283008
K = 153 Res: 272199852 Shift: 1 2D answer: 544399704
K = 154 Res: 279398196 Shift: 1 2D answer: 558796392
K = 155 Res: 286738375 Shift: 1 2D answer: 573476750
K = 156 Res: 294222240 Shift: 1 2D answer: 588444480
K = 157 Res: 301851654 Shift: 1 2D answer: 603703308
K = 158 Res: 309628492 Shift: 1 2D answer: 619256984
K = 159 Res: 317554641 Shift: 1 2D answer: 635109282
K = 160 Res: 325632000 Shift: 1 2D answer: 651264000
K = 161 Res: 333862480 Shift: 1 2D answer: 667724960
K = 162 Res: 342248004 Shift: 1 2D answer: 684496008
K = 163 Res: 350790507 Shift: 1 2D answer: 701581014
K = 164 Res: 359491936 Shift: 1 2D answer: 718983872
K = 165 Res: 368354250 Shift: 1 2D answer: 736708500
K = 166 Res: 377379420 Shift: 1 2D answer: 754758840
K = 167 Res: 386569429 Shift: 1 2D answer: 773138858
K = 168 Res: 395926272 Shift: 1 2D answer: 791852544
K = 169 Res: 405451956 Shift: 1 2D answer: 810903912
K = 170 Res: 415148500 Shift: 1 2D answer: 830297000
K = 171 Res: 425017935 Shift: 1 2D answer: 850035870
K = 172 Res: 435062304 Shift: 1 2D answer: 870124608
K = 173 Res: 445283662 Shift: 1 2D answer: 890567324
K = 174 Res: 455684076 Shift: 1 2D answer: 911368152
K = 175 Res: 466265625 Shift: 1 2D answer: 932531250
K = 176 Res: 477030400 Shift: 1 2D answer: 954060800
K = 177 Res: 487980504 Shift: 1 2D answer: 975961008
K = 178 Res: 499118052 Shift: 1 2D answer: 998236104
K = 179 Res: 510445171 Shift: 1 2D answer: 20890335
K = 180 Res: 521964000 Shift: 1 2D answer: 43927993
K = 181 Res: 533676690 Shift: 1 2D answer: 67353373
K = 182 Res: 545585404 Shift: 1 2D answer: 91170801
K = 183 Res: 557692317 Shift: 1 2D answer: 115384627
K = 184 Res: 569999616 Shift: 1 2D answer: 139999225
K = 185 Res: 582509500 Shift: 1 2D answer: 165018993
K = 186 Res: 595224180 Shift: 1 2D answer: 190448353
K = 187 Res: 608145879 Shift: 1 2D answer: 216291751
K = 188 Res: 621276832 Shift: 1 2D answer: 242553657
K = 189 Res: 634619286 Shift: 1 2D answer: 269238565
K = 190 Res: 648175500 Shift: 1 2D answer: 296350993
K = 191 Res: 661947745 Shift: 1 2D answer: 323895483
K = 192 Res: 675938304 Shift: 1 2D answer: 351876601
K = 193 Res: 690149472 Shift: 1 2D answer: 380298937
K = 194 Res: 704583556 Shift: 1 2D answer: 409167105
K = 195 Res: 719242875 Shift: 1 2D answer: 438485743
K = 196 Res: 734129760 Shift: 1 2D answer: 468259513
K = 197 Res: 749246554 Shift: 1 2D answer: 498493101
K = 198 Res: 764595612 Shift: 1 2D answer: 529191217
K = 199 Res: 780179301 Shift: 1 2D answer: 560358595
K = 200 Res: 796000000 Shift: 1 2D answer: 591999993
K = 201 Res: 812060100 Shift: 1 2D answer: 624120193
K = 202 Res: 828362004 Shift: 1 2D answer: 656724001
K = 203 Res: 844908127 Shift: 1 2D answer: 689816247
K = 204 Res: 861700896 Shift: 1 2D answer: 723401785
K = 205 Res: 878742750 Shift: 1 2D answer: 757485493
K = 206 Res: 896036140 Shift: 1 2D answer: 792072273
K = 207 Res: 913583529 Shift: 1 2D answer: 827167051
K = 208 Res: 931387392 Shift: 1 2D answer: 862774777
K = 209 Res: 949450216 Shift: 1 2D answer: 898900425
K = 210 Res: 967774500 Shift: 1 2D answer: 935548993
K = 211 Res: 986362755 Shift: 1 2D answer: 972725503
K = 212 Res: 5217497 Shift: 1 2D answer: 10434994
K = 213 Res: 24341275 Shift: 1 2D answer: 48682550
K = 214 Res: 43736629 Shift: 1 2D answer: 87473258
K = 215 Res: 63406118 Shift: 1 2D answer: 126812236
K = 216 Res: 83352313 Shift: 1 2D answer: 166704626
K = 217 Res: 103577797 Shift: 1 2D answer: 207155594
K = 218 Res: 124085165 Shift: 1 2D answer: 248170330
K = 219 Res: 144877024 Shift: 1 2D answer: 289754048
K = 220 Res: 165955993 Shift: 1 2D answer: 331911986
K = 221 Res: 187324703 Shift: 1 2D answer: 374649406
K = 222 Res: 208985797 Shift: 1 2D answer: 417971594
K = 223 Res: 230941930 Shift: 1 2D answer: 461883860
K = 224 Res: 253195769 Shift: 1 2D answer: 506391538
K = 225 Res: 275749993 Shift: 1 2D answer: 551499986
K = 226 Res: 298607293 Shift: 1 2D answer: 597214586
K = 227 Res: 321770372 Shift: 1 2D answer: 643540744
K = 228 Res: 345241945 Shift: 1 2D answer: 690483890
K = 229 Res: 369024739 Shift: 1 2D answer: 738049478
K = 230 Res: 393121493 Shift: 1 2D answer: 786242986
K = 231 Res: 417534958 Shift: 1 2D answer: 835069916
K = 232 Res: 442267897 Shift: 1 2D answer: 884535794
K = 233 Res: 467323085 Shift: 1 2D answer: 934646170
K = 234 Res: 492703309 Shift: 1 2D answer: 985406618
K = 235 Res: 518411368 Shift: 1 2D answer: 36822729
K = 236 Res: 544450073 Shift: 1 2D answer: 88900139
K = 237 Res: 570822247 Shift: 1 2D answer: 141644487
K = 238 Res: 597530725 Shift: 1 2D answer: 195061443
K = 239 Res: 624578354 Shift: 1 2D answer: 249156701
K = 240 Res: 651967993 Shift: 1 2D answer: 303935979
K = 241 Res: 679702513 Shift: 1 2D answer: 359405019
K = 242 Res: 707784797 Shift: 1 2D answer: 415569587
K = 243 Res: 736217740 Shift: 1 2D answer: 472435473
K = 244 Res: 765004249 Shift: 1 2D answer: 530008491
K = 245 Res: 794147243 Shift: 1 2D answer: 588294479
K = 246 Res: 823649653 Shift: 1 2D answer: 647299299
K = 247 Res: 853514422 Shift: 1 2D answer: 707028837
K = 248 Res: 883744505 Shift: 1 2D answer: 767489003
K = 249 Res: 914342869 Shift: 1 2D answer: 828685731
K = 250 Res: 945312493 Shift: 1 2D answer: 890624979
K = 251 Res: 976656368 Shift: 1 2D answer: 953312729
K = 252 Res: 8377490 Shift: 1 2D answer: 16754980
K = 253 Res: 40478888 Shift: 1 2D answer: 80957776
K = 254 Res: 72963582 Shift: 1 2D answer: 145927164
K = 255 Res: 105834611 Shift: 1 2D answer: 211669222
K = 256 Res: 139095026 Shift: 1 2D answer: 278190052
K = 257 Res: 172747890 Shift: 1 2D answer: 345495780
K = 258 Res: 206796278 Shift: 1 2D answer: 413592556
K = 259 Res: 241243277 Shift: 1 2D answer: 482486554
K = 260 Res: 276091986 Shift: 1 2D answer: 552183972
K = 261 Res: 311345516 Shift: 1 2D answer: 622691032
K = 262 Res: 347006990 Shift: 1 2D answer: 694013980
K = 263 Res: 383079543 Shift: 1 2D answer: 766159086
K = 264 Res: 419566322 Shift: 1 2D answer: 839132644
K = 265 Res: 456470486 Shift: 1 2D answer: 912940972
K = 266 Res: 493795206 Shift: 1 2D answer: 987590412
K = 267 Res: 531543665 Shift: 1 2D answer: 63087323
K = 268 Res: 569719058 Shift: 1 2D answer: 139438109
K = 269 Res: 608324592 Shift: 1 2D answer: 216649177
K = 270 Res: 647363486 Shift: 1 2D answer: 294726965
K = 271 Res: 686838971 Shift: 1 2D answer: 373677935
K = 272 Res: 726754290 Shift: 1 2D answer: 453508573
K = 273 Res: 767112698 Shift: 1 2D answer: 534225389
K = 274 Res: 807917462 Shift: 1 2D answer: 615834917
K = 275 Res: 849171861 Shift: 1 2D answer: 698343715
K = 276 Res: 890879186 Shift: 1 2D answer: 781758365
K = 277 Res: 933042740 Shift: 1 2D answer: 866085473
K = 278 Res: 975665838 Shift: 1 2D answer: 951331669
K = 279 Res: 18751800 Shift: 1 2D answer: 37503600
K = 280 Res: 62303979 Shift: 1 2D answer: 124607958
K = 281 Res: 106325719 Shift: 1 2D answer: 212651438
K = 282 Res: 150820383 Shift: 1 2D answer: 301640766
K = 283 Res: 195791346 Shift: 1 2D answer: 391582692
K = 284 Res: 241241995 Shift: 1 2D answer: 482483990
K = 285 Res: 287175729 Shift: 1 2D answer: 574351458
K = 286 Res: 333595959 Shift: 1 2D answer: 667191918
K = 287 Res: 380506108 Shift: 1 2D answer: 761012216
K = 288 Res: 427909611 Shift: 1 2D answer: 855819222
K = 289 Res: 475809915 Shift: 1 2D answer: 951619830
K = 290 Res: 524210479 Shift: 1 2D answer: 48420951
K = 291 Res: 573114774 Shift: 1 2D answer: 146229541
K = 292 Res: 622526283 Shift: 1 2D answer: 245052559
K = 293 Res: 672448501 Shift: 1 2D answer: 344896995
K = 294 Res: 722884935 Shift: 1 2D answer: 445769863
K = 295 Res: 773839104 Shift: 1 2D answer: 547678201
K = 296 Res: 825314539 Shift: 1 2D answer: 650629071
K = 297 Res: 877314783 Shift: 1 2D answer: 754629559
K = 298 Res: 929843391 Shift: 1 2D answer: 859686775
K = 299 Res: 982903930 Shift: 1 2D answer: 965807853
K = 300 Res: 36499972 Shift: 1 2D answer: 72999944
K = 301 Res: 90635122 Shift: 1 2D answer: 181270244
K = 302 Res: 145312976 Shift: 1 2D answer: 290625952
K = 303 Res: 200537149 Shift: 1 2D answer: 401074298
K = 304 Res: 256311268 Shift: 1 2D answer: 512622536
K = 305 Res: 312638972 Shift: 1 2D answer: 625277944
K = 306 Res: 369523912 Shift: 1 2D answer: 739047824
K = 307 Res: 426969751 Shift: 1 2D answer: 853939502
K = 308 Res: 484980164 Shift: 1 2D answer: 969960328
K = 309 Res: 543558838 Shift: 1 2D answer: 87117669
K = 310 Res: 602709472 Shift: 1 2D answer: 205418937
K = 311 Res: 662435777 Shift: 1 2D answer: 324871547
K = 312 Res: 722741476 Shift: 1 2D answer: 445482945
K = 313 Res: 783630304 Shift: 1 2D answer: 567260601
K = 314 Res: 845106008 Shift: 1 2D answer: 690212009
K = 315 Res: 907172347 Shift: 1 2D answer: 814344687
K = 316 Res: 969833092 Shift: 1 2D answer: 939666177
K = 317 Res: 33092019 Shift: 1 2D answer: 66184038
Press any key to continue . . .

*/
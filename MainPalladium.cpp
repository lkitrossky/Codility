// Gold from the first attempt ( almost, some compiler issues, but not the solution per se)
//this one was easy, but I was busy with something else
//https://app.codility.com/cert/view/cert4R3JKZ-2GBANK62V8G8Q9RT/
//https://app.codility.com/cert/view/cert4R3JKZ-2GBANK62V8G8Q9RT/details/
/*
	Codility confirms that a person who identified to us on-line as:
	Levi Kitrossky
	was awarded
	Codility Golden Award for the Palladium 2020 Challenge
	This is the 96th Codility Golden Award granted this month.
*/
#include <vector>
#include <algorithm>
using namespace std;
unsigned int solutionInternal(vector<int> &H) {
	const unsigned int N = H.size();
	unsigned int highest = 0;
	for (unsigned int i = 0; i < N; i++) {
		if (H.at(i) > (int)highest) {
			highest = H.at(i);
		}
	}
	unsigned int worstAnswer = highest*N; //cannot be worst
	unsigned int answer = worstAnswer;
	unsigned int secondHigh = *H.begin();
	for (unsigned int i = 0; i < N; i++) { //from left to right u
		if (secondHigh >= highest) //until meeting the highest no sens eto continue if we suppose that the economy is on the left
			break;
		if (H.at(i) >(int)secondHigh) {
			secondHigh = H.at(i);
		}
		unsigned int possibleAnswer = worstAnswer - (highest - secondHigh) * (i + 1); //economy is the squaet between the highest and second highest from the left tot the current place
		answer = min(answer,possibleAnswer); //better answer  found
	}
	return answer;
}
int solution(vector<int> &H) {
	int answer1 = (int)solutionInternal(H);
	reverse(H.begin(), H.end());
	int answer2 = (int)solutionInternal(H);
	return min(answer1, answer2);
}

#include <iostream>
ostream & operator<<(ostream & os, const vector<int> & vI) {
	for (unsigned int i = 0; i < vI.size(); i++) {
		os << vI.at(i);
		if (i + 1 < vI.size())
			os << ",";
	}
	return os;
}
int main() {

	int H1[] = { 3, 1, 4 };
	vector<int> VH = vector<int>(H1, H1 + 3);
	cout << VH << " solution:" << solution(VH) << endl;
	int H2[] = { 5,3,2,4 };
	VH = vector<int>(H2, H2 + 4);
	cout << VH << " solution:" << solution(VH) << endl;
	int H3[] = { 5,3,5,2,1 };
	VH = vector<int>(H3, H3 + 5);
	cout << VH << " solution:" << solution(VH) << endl;
	int H4[] = { 7,7,3,7,7 };
	VH = vector<int>(H4, H4 + 5);
	cout << VH << " solution:" << solution(VH) << endl;

	int H5[] = { 1, 1, 7, 6, 6, 6 };
	VH = vector<int>(H5, H5 + 6);
	cout << VH << " solution:" << solution(VH) << endl;

	return 0;
}
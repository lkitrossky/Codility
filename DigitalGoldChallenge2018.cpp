//////////////////////////////////////////////////////////////////////
#include <vector>
using namespace std;
int solution(int N, int M, vector<int> &X, vector<int> &Y);

int solution(int N, int M, vector<int> &X, vector<int> &Y) {
	unsigned int res = 0;
	vector<unsigned long long> xWeights(N, 0);
	vector<unsigned long long> yWeights(M, 0);
	vector<unsigned long long> xPartialSums(N, 0);
	vector<unsigned long long> yPartialSums(M, 0);
	unsigned int K = X.size();
	if (K % 2 > 0)
		return 0;
	unsigned int half = K / 2;
	for (unsigned int i = 0; i < K; i++) {
		xWeights.at(X.at(i))++;
		yWeights.at(Y.at(i))++;
	}
	for (unsigned int i = 1; i <(unsigned) N; i++) {
		xPartialSums.at(i) = xPartialSums.at(i - 1) + xWeights.at(i-1);
		if (xPartialSums.at(i) == half)
			res++;
		if (xPartialSums.at(i) > half)
			break;
	}
	for (unsigned int i = 1; i <(unsigned)M; i++) {
		yPartialSums.at(i) = yPartialSums.at(i - 1) + yWeights.at(i-1);
		if (yPartialSums.at(i) == half)
			res++;
		if (yPartialSums.at(i) > half)
			break;
	}
	return res;
}
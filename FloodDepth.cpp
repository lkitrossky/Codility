// FloodDepth.cpp : Defines the entry point for the console application.
//https://codility.com/programmers/lessons/90-tasks_from_indeed_prime_2015_challenge/flood_depth/




#include "stdafx.h"
#include <iterator>
#include <iostream>
#include <vector>
using namespace std;


//The idea is to passfrom left to right, the state will be: SeekingPool, FoundPool, SeekingPoolEnd

//I left the idea in the middle and looked into the seciond solution here, which uses the similar idea:
//https://codesays.com/2016/solution-to-flood-depth-by-codility/
//it is 100%. The conclusion: do not be afraid the direction was more ot less correct
//nothing too complicated

#include <vector>
#include <algorithm>
using namespace std;
int solution(vector<int> &A) {
    int left = 0, right = A.size() - 1, maxHeightFromLeft = 0, maxHeightFromRight = 0, ans = 0;
    while(left < right) {
        if(A[left] <= A[right]) {
            maxHeightFromLeft = std::max(maxHeightFromLeft, A[left]);
            ans = std::max(ans, maxHeightFromLeft - A[left++]);
        }
        else {
            maxHeightFromRight = std::max(maxHeightFromRight, A[right]);
            ans = std::max(ans, maxHeightFromRight - A[right--]);
        }
    }
    return ans;
}

template<class T> void printVector(const vector<T>& res){
	if(res.empty()){
		cout<<"Empty vector"<<endl;
		return;
	}
	std::copy(res.begin(), res.end(), ostream_iterator<T>(cout, " "));
	cout<<endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int AA[] = {1, 3, 2, 1, 2, 1, 5, 3, 3, 4, 2};
	vector<int> A(AA, AA + sizeof(AA)/sizeof (int));
	printVector(A);
	cout<<"Res: " << solution(A)<<endl;
	return 0;
}


//#include "stdafx.h"
//Rhodium 2019
//https://app.codility.com/programmers/challenges/rhodium2019/
//https://app.codility.com/programmers/custom_challenge/rhodium2019/
// https://app.codility.com/cert/view/certMB4B5A-WQ5DBVT4DCJJ4ASZ/
//GOLDEN!!
//Detected time complexity: O(N**2)


/*			Ideas

1. The problem ( bellow) seems from graph theory but it is unlikely since the same edge used several times in paths
2. An attempt to pass the graph by abrute force leads to N**3 performance or worse
3. The main catch is that the graph is connected and each two vortices have one and only one path. It means no loops.
4. The main idea is to grasp that in the a-b-c there is always 2 edges and so for every k
5. It means by Dirichlet principle that if we want a connected set of only consequential numbers i,i+1, i+2...i+k
it must have betwen them strictly k edges.
6. So, we can take any i, j: i< j and find all intervals k1,k2: i <= k1, k2 <=j, if they are j-k - it is one of sets. Also all edge is the set by itself.

The easiets algo was writtent in several minutes after long thinking about graphs and segment trees.
Initiate result as N
Create two dictionaries with left < right: 
left end of an interval-> set of right ends
right end of an interval-> set of left ends
Initiate numberOfIntervalsInsideij = 0
Move i++, j++ : j>i, j,j<N
After every i++ - remove all intervals with i-1 in both dictionaries
After every j++ add amount of intervals with j as right end, since too leftists were erased they are inside i,j interval
If numberOfIntervalsInsideij is equal j-n, increment the result.

The performance is N*N but it seems it cannot be less, since we must count them and there cannot be a formula for them (I think)


*/

#include <vector>
#include <set>
#include <algorithm>
using namespace std;

const int MAX = 1000;

int solution(vector< int> &T) {
	size_t  L = T.size();
	int res = L;

	//from left end find all right ends and vice versa
	vector<set<int> > leftToRight(MAX, set<int>()), rightToLeft(MAX, set<int>());

	for (size_t i = 0; i < L; i++) {
		int mi = min(T.at(i), (int)i);
		int ma = max(T.at(i), (int)i);
		if (mi != ma) {
			leftToRight.at(mi).insert(ma);
			rightToLeft.at(ma).insert(mi);
		}
	}

	for (size_t left = 0; left < L; left++) {
		size_t howManyInside = 0;
		if (left > 0 ) {
			//remove all intervals (left-1,*)
			for (auto & au : leftToRight.at(left - 1)) {
				rightToLeft.at(au).erase(left-1); //no rigth wil refer to left-1
			}
			leftToRight.at(left - 1) = set<int>(); //and left-1 has no rigth connection
		}
		for (size_t right = left + 1; right < L; right++) {
			howManyInside += rightToLeft.at(right).size(); //since all lefts < left are erased, we have only new rights with some left >= left and <= right
			if (howManyInside == right - left)
				res++;
		}
	}
	return res;
}

/////////////////////////////////////////////////////////////////////
// driver
/////////////////////////////////////////////////////////////////////
#include <iostream>
ostream & operator<<(ostream & os, const vector<vector< int> > & vs);
ostream & operator<<(ostream & os, const vector<int> & vI);
ostream & operator<<(ostream & os, const vector<pair<int, int> > &v);
ostream & operator<<(ostream & os, const set<unsigned int> & s);
ostream & operator<<(ostream & os, const vector<set<unsigned int> > & vs);
ostream & operator<<(ostream & os, const vector<unsigned int> & vI);
int solutionSilver2(vector< int> &_T);
void propagate(int start, int inf, int sup, set<unsigned int> & availbale, set<unsigned int> & unAvailbale, const vector<set<unsigned int> > & neighbors);
int solutionSilver(vector< int> &_T);
set<unsigned int> filterSet(const set<unsigned int> & s, int inf, int sup);
vector<set<unsigned int> > createNeighbours(const vector<unsigned int> &T);
bool comparePair(pair<int, int> p1, pair<int, int> p2);
struct comparePairStruct {
	bool operator() (pair<int, int> p1, pair<int, int> p2) const {
		return comparePair(p1, p2);
	}
};
ostream & operator<<(ostream & os, const set<pair<int, int>, comparePairStruct> & s);
void test(int * arr, int l) {
	vector< int> vl2(arr, arr + l);
	cout << vl2 << endl;
	cout<<"Latest: " << solution(vl2) << endl;
	//cout<<"Old: " << solutionSilver(vl2) << "  " << solutionSilver2(vl2) << endl;

}
int main() {
	int T[] = { 2,0,2,2,1,0 }; //see their drawing
	vector<int> vl2(T, T + 6);
	test(T, 6);


	int i2[] = { 5,2,1,4,0,2 };//1-2-5-0-4-3 //here is the catch : I am missing 0 (5,2) 1 2 (5 0 4 ) 3 4 (0) 5  //must be 9: 6 stops, all, 1-2, 3-4
	test(i2, 6);
	int i[] = { 2,5,3,4,5,4 };//0-2-3-4-5-1 // 7 usualls + 1-5 + 2-3 + 2-4 + 2-5 + 3-4 + 3-5 + 4-5= 14
	test(i, 6);
	int L3[] = { 2,2,2,0,3,4 }; //1-2-0-3-4-5 //7 usuals + 0-2 + 0-3 + 0-4 + 1-2 + 3-4 + 3-5 + 4-5 = 14
	test(L3, 6);
;
	int L2[] = { 1,1,0,2,3,4 }; //1-0-2-3-4-5
	test(L2, 6);
	int L1[] = { 2,0,3,4,5,5 }; //1-0-2-3-4-5
	test(L1, 6);
	int L[] = { 1,2,3,4,5,5 }; //0-1-2-3-4-5
	test(L, 6);

	test(T, 6);
	return 0;
	set<unsigned int> s;
	for (int i = 0; i < 10; i++)
		s.insert(i);
	cout << s << endl;
	int inf1 = 3, sup1 = 7;
	cout << "Print >= " << inf1 << " and  < " << sup1 << " : ";
	cout << filterSet(s, inf1, sup1) << endl;
	inf1 = -3; sup1 = 10;
	cout << "Print >= " << inf1 << " and  < " << sup1 << " : ";
	cout << filterSet(s, inf1, sup1) << endl;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//				END of driver  /////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//				silver and bad solutions  /////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//this solution is almost never correct!
/*
https://app.codility.com/cert/view/certMP2B7W-TMZFZ979V8PBXBFZ/details/

*/
// silver
//  https://app.codility.com/cert/view/certTY48XD-TT9QSHDPDRB529UX/
// timeout error on: line big (network is a line, N <= 1,000), star big (network has vertices with many neighbors, ), limited big (each village has at most 3 neighbors)
//a bit cleaner silver with unsigned:
// https://app.codility.com/cert/view/cert9GKADP-NH2VTGNVSXSTKS6H/
//https://app.codility.com/cert/view/cert9GKADP-NH2VTGNVSXSTKS6H/details/
//Detected time complexity: O(N**3) or O(N**3 * log(N))  How is is it a cube??
/* of possible importance
N is an integer within the range [2..1,000];
each element of array T is an integer within the range [0..N-1];
there is exactly one (possibly indirect) connection between any two distinct villages.
*/
/*
Ideas
1. Find how is it cube. Decrease to square may solve the issue
2. Try to find a kind of formula (fat chance)
3. Thinks about building a matrix, but I cannot see what to put into places
4. Change all sets to vectors or at least parts of them. Say neighbours can be vectors<bool> or just ordered vectors
4a. Availables can be just a number and vector<bool>[N]
4b. Unavailables can be a vector<bool> [N] without number
*/
set<unsigned int> filterSet(const set<unsigned int> & s, int inf, int sup) {
	set<unsigned int> res;
	res.insert(s.lower_bound(inf), s.lower_bound(sup));
	return res;
}
vector<set<unsigned int> > createNeighbours(const vector<unsigned int> &T) {
	const unsigned int L = T.size();
	vector<set<unsigned int> > neighbors((int)L, set<unsigned int>());
	for (unsigned int i = 0; i < L; i++) {
		if (T.at(i) == i)
			continue; //no sense to connect town to itself, supposed in any case
		unsigned int mi = min((int)T.at(i), (int)i);
		unsigned int ma = max((int)T.at(i), (int)i);
		neighbors.at(mi).insert(ma);
		neighbors.at(ma).insert(mi);
	}
	return neighbors;
}
void propagate(int start, int inf, int sup, set<unsigned int> & availbale, set<unsigned int> & unAvailbale, const vector<set<unsigned int> > & neighbors) {
	set<unsigned int> layer1 = filterSet(neighbors.at(start), inf, sup);
	set<unsigned int> layer2;
	for (auto &au : layer1) {
		if (availbale.end() == availbale.find(au))
			layer2.insert(au);
	}
	set<unsigned int> layer3 = filterSet(neighbors.at(start), sup, neighbors.size()); //too big neighbours
	if (!layer3.empty())
		unAvailbale.insert(layer3.begin(), layer3.end()); //put inyo unavailable can be of use for bigger j
	if (layer2.empty())
		return;
	availbale.insert(layer2.begin(), layer2.end());
	for (auto &au : layer2) {
		unAvailbale.erase(au);
	}
	for (auto &au : layer2) {
		propagate(au, inf, sup, availbale, unAvailbale, neighbors);

	}
}
int solutionSilver(vector< int> &_T) {
	const unsigned int L = _T.size();
	vector<unsigned  int> T(L, 0);
	for (unsigned int i = 0; i < L; i++)
		T.at(i) = _T.at(i);
	unsigned int res = 0;
	vector<set<unsigned int> > neighbors = createNeighbours(T); //translate into a standard graph form except that connections are ordered
	for (unsigned int i = 0; i < L; i++) {//loop start for all towns
		res++; //every town in itsels is a route
		set<unsigned int> availbale; //it is foir towns available on route [i,j]
		availbale.insert(i); //i is in every routes [i,j] for every j
		set<unsigned int> unAvailable = filterSet(neighbors.at(i), i + 1, L); //currently cannot move to any town > i because with start and end in i
																			  //unAvailable - neighbours of available but numbers are too high
		for (unsigned int j = i + 1; j < L; j++) { //loop through possible ends
			if (unAvailable.end() == unAvailable.find(j))
				continue;  //j is not found in towns connacted to availables
			availbale.insert(j); // we can go to j now
			unAvailable.erase(j); //not mopre unavailable
			propagate(j, i, j + 1, availbale, unAvailable, neighbors); //find all neighboring towns wuth new end, will possibly include some others k: i<k<j
			if (availbale.size() == j - i + 1) //if the available towns amounts exactly to j-i+1 than we can cover [i,j], otherwise it is always too small
				res++;
		}
	}
	return (int)res;
}

///////////////////////////////////////////////////////////////////
//creates asuqre matrix , true means i,j are neighbours
//rebuilt on vectors but still silver
//https://app.codility.com/cert/view/cert733DTE-MUP36Z9X4R5GF8GP/
//https://app.codility.com/cert/view/cert733DTE-MUP36Z9X4R5GF8GP/details/
//Detected time complexity:  O(N**3) or O(N**3 * log(N)), 

vector<vector<bool> > createNeighboursMatrix(const vector<unsigned int> &T) { //n**2
	const unsigned int L = T.size();
	vector<bool> row(L, false);
	vector<vector<bool> > neighborsMatrix(L, row);
	vector<set<unsigned int> > neighbors((int)L, set<unsigned int>());
	for (unsigned int i = 0; i < L; i++) {
		if (T.at(i) == i)
			continue; //no sense to connect town to itself, supposed in any case
		unsigned int mi = min((int)T.at(i), (int)i);
		unsigned int ma = max((int)T.at(i), (int)i);
		neighborsMatrix.at(mi).at(ma) = true;
		neighborsMatrix.at(ma).at(mi) = true;
	}
	return neighborsMatrix;
}
void propagate(unsigned int start, unsigned int inf, unsigned int sup, unsigned int & availableNumber, vector<bool> & availbale, vector<bool> & unAvailbale, const vector<vector<bool> >  & neighborsMatrix) {
	const unsigned int L = neighborsMatrix.at(0).size();
	for (unsigned int i = sup; i < L; i++) {
		if (neighborsMatrix.at(start).at(i)) {  //find too big new neighbours and add into unavailables, can be used on the route end
			unAvailbale.at(i) = true;
		}
	}
	vector<unsigned int> newPlaces;
	for (unsigned int i = inf; i < sup; i++) {
		if (neighborsMatrix.at(start).at(i) && !availbale.at(i)) {  //new good numbers to extend the area
			availbale.at(i) = true;
			availableNumber++;
			newPlaces.push_back(i);
		}
	}
	for (auto &au : newPlaces) {
		propagate(au, inf, sup, availableNumber, availbale, unAvailbale, neighborsMatrix);
	}
}
int solutionSilver2(vector< int> &_T) {
	// just copy into unsigned
	const unsigned int L = _T.size();
	vector<unsigned  int> T(L, 0);
	for (unsigned int i = 0; i < L; i++)
		T.at(i) = _T.at(i);

	unsigned int res = 0;
	vector<vector<bool> > neighborsMatrix = createNeighboursMatrix(T); //translate a matrix form of graph
	for (unsigned int i = 0; i < L; i++) {//loop start for all towns
		res++; //every town in itsels is a route
		vector<bool> availbale(L, false); //it is for towns available on route [i,j]
		availbale.at(i) = true;//i is in every routes [i,j] for every j
		vector<bool> unAvailable = neighborsMatrix.at(i); //currently cannot move to any town > i because with start and end in i
		unsigned int availableNumber = 1;																	  //unAvailable - neighbours of available but numbers are too high
		for (unsigned int j = i + 1; j < L; j++) { //loop through possible ends
			if (!unAvailable.at(j))
				continue;  //j is not found in towns connacted to availables
			availbale.at(j) = true; // we can go to j now
			unAvailable.at(j) = false; //not more unavailable
			availableNumber++;
			propagate(j, i, j + 1, availableNumber, availbale, unAvailable, neighborsMatrix); //find all neighboring towns wuth new end, will possibly include some others k: i<k<j
			if (availableNumber == j - i + 1) //if the available towns amounts exactly to j-i+1 than we can cover [i,j], otherwise it is always too small
				res++;
		}
	}
	return (int)res;
}
void buildTreeArray(int idx, int ss, int se, vector<vector<int>> a,vector<int> sTree[]);
int queryArray(int start, int end, int k, vector<vector<int>> a, int n,vector<int> sTree[]);
int solutionAttempt1BasedOnTreeButNotAllL(vector< int> &T) {
	size_t  L = T.size();
	int res = L;
	set<pair<int, int>, comparePairStruct> fragmentsSet;  //left, right  (1,2) (1,3) (2,4)... al unique
	for (size_t i = 0; i < L; i++) {
		int mi = min(T.at(i), (int)i);
		int ma = max(T.at(i), (int)i);
		if (mi != ma) {
			fragmentsSet.insert(pair<int, int>(mi, ma));//fragmentsSet.insert(pair<int, int>(mi, L - 1 - ma));
		}
	}
	cout << "Fragment set:\n" << fragmentsSet << endl;

	vector<int> sTree[MAX]; // Construct segment tree

	int realArrayLength = 0;
	set<int> firsts;
	vector<int> firstv;
	vector<vector<int>> a;
	for (auto &au : fragmentsSet) {
		if (firsts.find(au.first) == firsts.end()) {
			realArrayLength++;
			firsts.insert(au.first);
			firstv.push_back(au.first);
			a.push_back(vector<int>());
		}
		a[realArrayLength - 1].push_back(au.second);
	}
	cout << "a to build tree:\n";
	cout << a << endl;

	buildTreeArray(0, 0, realArrayLength - 1, a, sTree);

	for (int i = 0; i < (int)L; i++) {
		vector<int>::iterator it = lower_bound(firstv.begin(), firstv.end(), i);
		int d1 = distance(firstv.begin(), it);
		for (int j = L - 1; j > i; j--) {
			vector<int>::iterator it2 = lower_bound(firstv.begin(), firstv.end(), j);
			int d2 = distance(firstv.begin(), it2);
			int q = queryArray(d1, d2, j, a, L - 1, sTree);
			//cout << "*it: " << *it << " *it2: " << *it2 << " d1:" << d1 << " d2:" << d2 << " q:" << q << endl;
			cout << "i: " << i << " j: " << j << " d1:" << d1 << " d2:" << d2 << " q:" << q << endl;
			if (q == j - i)
				res++;
		}
	}

	return res;
}















#include <iterator>

//silver
//https://app.codility.com/cert/view/certQ2XUDK-V2Z8UX9QHNP82SV6/
//https://app.codility.com/cert/view/certQ2XUDK-V2Z8UX9QHNP82SV6/details/
//Detected time complexity: O(N**3)
//idea: economize one for in the final


/*
Ideas
1. Find how is it cube. Decrease to square may solve the issue
2. Try to find a kind of formula (fat chance)
3. Can stress that afragmnt is good only if its count = max - min + 1. Noit good only too short. Cannoty be too long.
4. Can decrease from all.
5. Say, we take some continuous sequence length 2,...,7 and count only connections inside it. To be connected it must have exactly N-1 edges
6. The method: have again edges as sets. And then counting internaledges for all continuous sequences
7. Matrix: i,j = 1, when they are connected, 0 if not.
8. Matrix: i, j:  how many connections are in square i-j, i-j

*/




// Constructs a segment tree and stores sTree[] 
void buildTreeArray(int idx, int ss, int se, vector<vector<int>> a,
	vector<int> sTree[])
{
	/*leaf node*/
	if (ss == se)
	{
		sTree[idx] = a[ss];
		return;
	}

	int mid = (ss + se) / 2;

	/* building left subtree */
	buildTreeArray(2 * idx + 1, ss, mid, a, sTree);

	/* building right subtree */
	buildTreeArray(2 * idx + 2, mid + 1, se, a, sTree);

	/* merging left and right child in sorted order */
	merge(sTree[2 * idx + 1].begin(), sTree[2 * idx + 1].end(),
		sTree[2 * idx + 2].begin(), sTree[2 * idx + 2].end(),
		back_inserter(sTree[idx]));
}

// Recursive function to count smaller elements from row 
// a[ss] to a[se] and value smaller than or equal to k. 
int queryRecArray(int node, int start, int end, int ss, int se,
	int k, vector<vector<int>> a, vector<int> sTree[])
{
	/* If out of range return 0 */
	if (ss > end || start > se)
		return 0;

	/* if inside the range return count */
	if (ss <= start && se >= end)
	{
		/* binary search over the sorted vector
		to return count >= X */
		return upper_bound(sTree[node].begin(),
			sTree[node].end(), k) -
			sTree[node].begin();
	}

	int mid = (start + end) / 2;

	/*searching in left subtree*/
	int p1 = queryRecArray(2 * node + 1, start, mid, ss, se, k, a, sTree);

	/*searching in right subtree*/
	int p2 = queryRecArray(2 * node + 2, mid + 1, end, ss, se, k, a, sTree);

	/*adding both the result*/
	return p1 + p2;
}

// A wrapper over queryArray(). 
int queryArray(int start, int end, int k, vector<vector<int>> a, int n,
	vector<int> sTree[])
{
	return queryRecArray(0, 0, n - 1, start, end, k, a, sTree);
}

ostream & operator<<(ostream & os, const set<pair<int, int>, comparePairStruct> & s);
/*
This method is based on assumption that if between number i and number j there are exactly j-i bridges
Check this one:
https://www.geeksforgeeks.org/graph-data-structure-and-algorithms/

*/
//https://app.codility.com/cert/view/certBHUXFG-Z4X487H6RY8MDTZR/details/
//timeout , evaluation N**4, even not silver, although numbers are correct
//but not calc errors
int solutionN4(vector< int> &T) {
	size_t  L = T.size();
	int res = L;
	set<pair<int, int>, comparePairStruct> fragmentsSet;  //left, right  (1,2) (1,3) (2,4)... al unique
	for (size_t i = 0; i < L; i++) {
		int mi = min(T.at(i), (int)i);
		int ma = max(T.at(i), (int)i);
		if (mi != ma) {
			fragmentsSet.insert(pair<int, int>(mi, ma));//fragmentsSet.insert(pair<int, int>(mi, L - 1 - ma));
		}
	}
	//cout << "Fragment set:\n" << fragmentsSet << endl;

	vector<int> sTree[MAX]; // Construct segment tree

	int realArrayLength = 0;
	set<int> firsts;
	vector<int> firstv;
	vector<vector<int>> a(L, vector<int>());
	for (auto &au : fragmentsSet) {
		if (firsts.find(au.first) == firsts.end()) {
			realArrayLength++;
			firsts.insert(au.first);
			firstv.push_back(au.first);
		}
		a.at(au.first).push_back(au.second);
	}

	buildTreeArray(0, 0, L - 1, a, sTree);

	for (int i = 0; i < (int)L; i++) {
		for (int j = L - 1; j > i; j--) {
			int q = queryArray(i, j, j, a, L, sTree);
			//cout << "*it: " << *it << " *it2: " << *it2 << " d1:" << d1 << " d2:" << d2 << " q:" << q << endl;
			//cout << "i: " << i << " j: " << j  << " q:" << q << endl;
			if (q == j - i)
				res++;
		}
	}

	return res;
}

int solutionSilver3(vector< int> &T) {
	size_t  L = T.size();
	int res = L;
	vector<int> row(L, 0);
	vector<vector<int>> MatrixEdges(L, row);
	vector<int> miv(L - 1, 0);
	vector<int> mav(L - 1, 0);
	int counter = 0;
	for (size_t i = 0; i < L; i++) {
		int mi = min(T.at(i), (int)i);
		int ma = max(T.at(i), (int)i);
		if (mi != ma && 0 == MatrixEdges.at(mi).at(ma)) {
			MatrixEdges.at(mi).at(ma) = 1;
			miv.at(counter) = mi;
			mav.at(counter) = ma;
			counter++;
		}
	}
	cout << MatrixEdges << endl;
	vector<vector<int>> MatrixEdgesAggregated(L, row);
	for (size_t c = 0; c < L - 1; c++) {
		int mi = miv.at(c);
		int ma = mav.at(c);
		for (size_t k1 = 0; k1 <= (size_t)mi; k1++) {
			for (size_t k2 = ma; k2 < L; k2++) {
				MatrixEdgesAggregated[k1][k2] += MatrixEdges[mi][ma];
				if ((size_t)MatrixEdgesAggregated[k1][k2] == k2 - k1)
					res++;
			}
		}
	}
	cout << MatrixEdgesAggregated << endl;
	return res;
}
ostream & operator<<(ostream & os, const vector<int> & vI) {
	for (unsigned int i = 0; i < vI.size(); i++) {
		os << vI.at(i);
		if (i + 1 < vI.size())
			os << ",";
	}
	return os;
}
ostream & operator<<(ostream & os, const vector<unsigned int> & vI) {
	for (unsigned int i = 0; i < vI.size(); i++) {
		os << vI.at(i);
		if (i + 1 < vI.size())
			os << ",";
	}
	return os;
}
ostream & operator<<(ostream & os, const set<unsigned int> & s) {
	for (auto au = s.begin(); au != s.end(); au++) {
		os << *au << " ";
	}
	return os;
}
ostream & operator<<(ostream & os, const vector<set<unsigned int> > & vs) {
	for (auto au = vs.begin(); au != vs.end(); au++) {
		os << *au << endl;
	}
	return os;
}
ostream & operator<<(ostream & os, const vector<vector< int> > & vs) {
	for (auto au = vs.begin(); au != vs.end(); au++) {
		if (au->empty())
			os << "empty" << endl;
		else
			os << *au << endl;
	}
	return os;
}
ostream & operator<<(ostream & os, const vector<pair<int, int> > &v) {
	for (auto & au : v) {
		os << au.first << "," << au.second << "  ";
	}

	return os;
}
ostream & operator<<(ostream & os, const set<pair<int, int>, comparePairStruct> & s) {
	for (auto & au : s) {
		os << au.first << "," << au.second << "  ";
	}
	return os;
}
bool comparePair(pair<int, int> p1, pair<int, int> p2)
{ // sort by left, then sort by right end
	if (p1.first < p2.first)
		return true;
	else if (p1.first > p2.first)
		return false;
	return (p1.second < p2.second);
}
/*
Task description
You want to spend your next vacation in a foreign country. In the summer you are free for N consecutive days. By a strange coincidence, in the foreign country there are N villages numbered from 0 to N-1.
On each day there will be a festival in one of the villages; on day K there will be a festival in village number K.

You plan to begin your vacation on some day A and end it on some day B (0 <= A <= B < N). During day K you will attend the festival organized in village K, and in the evening (if K < B) you will take a bus to village K+1.

The villages are connected by N-1 bidirectional roads in such a way that there is exactly one path (not necessarily direct) between any two villages.
If the path between villages K and K+1 is not direct, the bus will travel through some other village(s). Whenever you travel through a village, you are extremely sad if you did not and will not attend the festival in this village, which would totally ruin your vacations. In other words, if during travel between villages K and K+1 the bus passes through some village J, following inequalities must be satisfied: A <= J <= B.

You are given a map of the road network in the form of array T of length N. This means that if T[K] = J and K != J, then there is a direct road between villages K and J.

Write a program that calculates how many possible vacations you can consider, i.e.,
how many pairs of days (A, B) satisfy the constraints that you will visit all the villages you see during your bus trips (visited villages numbers create continuous range of integers).

Write a function:

int solution(vector<int> &T);

that, given a non-empty array T consisting of N integers describing a road network between N villages, returns your number of possible vacations.

For example, the following array

T[0] = 2
T[1] = 0
T[2] = 2
T[3] = 2
T[4] = 1
T[5] = 0
encodes a country of N = 6 villages depicted in the figure below:

Drawing of the example layout

For this array the function should return 12, since there are twelve possible vacations: (0, 0), (0, 1), (0, 2), (0, 3), (0, 4), (0, 5), (1, 1), (2, 2), (2, 3), (3, 3), (4, 4), (5, 5).

Every pair in which A = B is a correct vacation. Pair (2, 3) is also correct, since, during the bus trip from village 2 to village 3, you do not pass through any other village.

Pair (0, 3) is correct. Paths the bus goes through are following: (0 -> 1), (1 -> 0 -> 2), (2 -> 3). Every bus path covers villages only from the continuous range (0, 3).

Pair (2, 4) is not correct, since, during the bus trip from village 3 to village 4, you pass through villages 0 and 1, but you do not attend festivals in these villages.

Pair (1, 4) is not correct. You would start on day A = 1 in village 1 and finish on day B = 4 in village 4. In the first evening you would take the bus from village 1 to 2.
The path of the bus would lead through village 0 (the path is 1 -> 0 -> 2),
in which the festival happened on day J = 0, i.e. before day A, hence condition A <= J <= B is violated.
As you will not ever visit festival in a village you pass through, you would be extremely sad. You would pass through village 0 again after the day K = 3 during travel to village B = 4.

Write an efficient algorithm for the following assumptions:

N is an integer within the range [2..1,000];
each element of array T is an integer within the range [0..N-1];
there is exactly one (possibly indirect) connection between any two distinct villages.

*/
/*
https://app.codility.com/programmers/challenges/great_code_off2021/ 
The solution was published here: https://www.youtube.com/watch?v=XzUuEkLygbs&t=4s by Codility itself
Segment tree - https://www.youtube.com/watch?v=XzUuEkLygbs&t=474s
with lazy propagation - https://youtu.be/XzUuEkLygbs?t=738
performance https://youtu.be/XzUuEkLygbs?t=884
Implementation in Python https://youtu.be/XzUuEkLygbs?t=1005
N*LogN  but at the end they say that even
https://youtu.be/XzUuEkLygbs?t=1526 - sweep line method in linear time!


*/
//these two lines only for debugging
//#define MYDEBUG    //to printout stages
#include <iostream>
// Silver 8
// https://app.codility.com/cert/view/certKFM4RC-GCKGZ9Q9753DNPY3/details/
//Silver 8 B 
/*
O(M*sqrt(N))
https://app.codility.com/cert/view/certNRTGEX-XG6C6UX5CVGX7ZFY/details/

After leaves splicing
https://app.codility.com/cert/view/certDJB25R-JRY7Z8PAXC3C92XV/details/
O(M*sqrt(N))
Only one test fails:
many_colors							OK
medium_perf_package					OK
big_perf_package					TIMEOUT ERROR
Killed. Hard limit reached: 6.000 sec.
1.0.008 sOK
2.6.000 sTIMEOUT ERROR, Killed. Hard limit reached: 6.000 sec.
3.0.088 sOK
4.0.176 sOK
5.6.000 sTIMEOUT ERROR, Killed. Hard limit reached: 6.000 sec.
6.6.000 sTIMEOUT ERROR, Killed. Hard limit reached: 6.000 sec.
*/

/* ////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Description

We have 1,...,N numbers.
Let us call interval - set of four numbers: left, right, minimal flavor, maximum flavor
Like 2,4,5,8 means a fragment with numbers 2-4 which covers points with flavors >= 5 and 8<=
We keep a binary tree with nodes TNode.
Every node has and interval and two pointers TNode *left, *right - to two siblings.
The lowest nodes are leaves they have both pointers NULL, and minimal flavor equalt maximum flavor
In short it is a sequence of the same cakes with the same flavor, the first is in left position , the last is right position.
Important and obvious - leaves never overlap but can be adjacent.
Every node but not a leaf covers some leaves, 
	"left" of node is the extreme left point of all covered leaves
	"right" of node is the extreme rigth point of all covered leaves
In some nodes minimal flavor = maximum flavor, we will call the uniform nodes.

	Algorithm
The starting position is interval 1,N,0,0 that is all cakes have 0 flavor.It is a uniform node also leaf
Now we have M instructions, every instruction also has left, right and flavor (one)

Instruction vs. poiner to node pnode. The first instruction meets uniform node 1,N,0,0
0. If  pnode is NULL, return
1. If  instruction.left > node.right or instruction.right , node.right, return. The instruction misses
2. If left of instruction < left of the node - set instruction.left = node.left. 
3. If right of instruction > right of the node - set instruction.left = node.right.
These two steps just restrict instruction to the node
4. If instruction.left >= node.left and instruction.left <= node.right, the node is spilt:
	Now the node's left pointer to the same node but restricted to out of the instruction
	Now the node's right pointer to the same node leftovers, the same operation invoked again
5. The same with the right end of the instrucion is inside the inrevat of the node.

This moment we arrive to treatment when interval of the instruction is the same as the interval of the node.
The operation is called 
TNode* changeTree
(TNode* node, const int k, int theLeftest, int theRightest, int flavorFromAbove)
	TNode* node -  the node we apply it to
	const int k -  the flavor of the isntruction
	int theLeftest - left of the instruction interval, it is now equal to node.left 
	int theRightest - right of the instruction interval, it is now equal to node.right  
	int flavorFromAbove - 
		if the above node is uniform, it is its flavor
		if the above node is not uniform, it is the invalid flavor -1
The idea is as follows: whenever we meet a uniform node, the instruction applies to it all
						if the node is upgraded, incrementing flavor, and we stop, th children
						will be not in sync with its flavor. So, we need to pass the enforcing flavor down
						All kds must get this flavor, then the parent is updated, possibly losing uniformity
When instruction's boundarie are the same as the uniform node
	if k-1 = flavor, increment the flavor and retun pnode
	if k-1 != flavor, retun NULL, the node is dimissed
When instruction's boundarie are the same as the non-uniform node and
	k-1 < minimal flavor or k-1 > maximum flavor, return NULL, the node is dismissed
If there are two valid pointers, apply instruction to both
If there is one valid pointer, apply to it
If there are not valid pointrs, it must be leaf and already treated, if not - exception
After treatment of kids, of both are NULL, return NULL, the node is dismissed
If not, correct bounderies and min/max flavors with one or two.
Return pnode.

After all isntructions are done, the tree can have leaves not in sync with nodes, so pass from all uniform
nodes the flavor down. Start with root, pass -1 on non-uniform, valid flavor on uniform

Now the counting - pass over all tree, count only leaves with flavor K. return the result.
/////////////////////////////////////////////////////////////////////////////////////////*/
//////////////////////////////////////////////////////////////////////////////////////////
/// solution per se  /////////////////////////////////////////////////////////////////////
#include <vector>
#include <algorithm>
using namespace std;

	/* A Binary Tree node */
	class TNode
	{
	public:
		int theMostLeft, theMostRight;
		int maxFlavor;  //only >=0, if maxFlavor == minFlavor, it is a leaf and has a vector of positions
		int minFlavor;  //any other num means the actual topping of all the locations
		TNode *left;
		TNode *right;
		bool isLeaf() { return (NULL == left && NULL == right); }
		static TNode *newNode(int l, int r, int minFlavor, int maxFlavor) {
			TNode* node = new TNode();
			node->theMostLeft = l;
			node->theMostRight = r;
			node->minFlavor = minFlavor;
			node->maxFlavor = maxFlavor;
			node->left = NULL;
			node->right = NULL;
			return node;
		}
		static void freeNode(TNode* node) {
			if (NULL != node)
				free(node);
		}
	};
	void preOrder(TNode* node);
	static TNode* sortedArrayToBST(int N) {  //the initial state is one big node all 0 with no descendants
		TNode *leaf = TNode::newNode(1, N, 0, 0);
		return leaf;
	}
	//A utility to push down colors from nodes at the end, if node's flavor >=0
	static void pushColorsToLeaves(TNode* node, int k) {
		if (node == NULL)
			return;
		if (k >= 0)
			node->maxFlavor = node->minFlavor = k;
		int pushFlavor = (node->maxFlavor == node->minFlavor ? node->minFlavor : -1);
		pushColorsToLeaves(node->left, pushFlavor);
		pushColorsToLeaves(node->right, pushFlavor);
	}
	// A utility function counts all leaves with flavor k 
	static int countFlavorK(TNode* node, int k)  //counts only leaves, but sometimes must count nodes
	{
		if (node == NULL)
			return 0;
		if (k == node->maxFlavor && k == node->minFlavor && node->isLeaf()) //no descendants, so handle it
			return node->theMostRight + 1 - node->theMostLeft;
		//it is not leaf and need collect to the bottom
		int res = 0;
		res += countFlavorK(node->left, k);
		res += countFlavorK(node->right, k);
		return res;
	}
	/*
	Main function, see above
	Logic:
		guard cases: NULL, outside
		Boundaries are the same case, check if can throw away or upgrade only the node
		Was not able to deal on package, and not a leaf case:
			handle kids and update info, if no kids - throw away
		Leaf case with instruction too small - split cases
	*/
	static TNode* changeTree
	(TNode* node, const int k, int theLeftest, int theRightest, int flavorFromAbove) {
		//this is in essence flag. If  -1 under me flavors are versatile, if not - only this
		if (NULL == node || theLeftest > theRightest)
			return node; //guard conditions
		if (flavorFromAbove >= 0) { //this node must be as above flavor
			node->maxFlavor = node->minFlavor = flavorFromAbove;
		}
		if (theLeftest > node->theMostRight || theRightest < node->theMostLeft)
			return node; //the instruction is outside 
		//limit change to the overlap
		theLeftest = max(node->theMostLeft, theLeftest);
		theRightest = min(node->theMostRight, theRightest);
		//first treat the case, when boundaries of the instructon and the node are the same
		//impossible upgrade for uniform or non-uniform node
		if (theLeftest == node->theMostLeft && theRightest == node->theMostRight)
		{
			if (k - 1 < node->minFlavor || k - 1 > node->maxFlavor) {
				TNode::freeNode(node);
				return NULL;
			}
			//upgrade can be only on a uniform node, try it
			if (k - 1 == node->minFlavor && k - 1 == node->maxFlavor) {
				node->minFlavor = node->maxFlavor = k;
				return node; //kids, if any, may be not in sync
			}
		}
		int myOnlyFlavor = (node->maxFlavor == node->minFlavor ? node->maxFlavor : -1);
		if (!node->isLeaf()) { //has kids, handle them with my parameters
			node->left = changeTree(node->left, k, theLeftest, theRightest, myOnlyFlavor);
			node->right = changeTree(node->right, k, theLeftest, theRightest, myOnlyFlavor);
			if (NULL == node->left && NULL == node->right) {
				return NULL; //lost both kids, not existing any more
			}
			if (NULL != node->left && NULL != node->right) { //still has both, take info from them
				node->minFlavor = min(node->left->minFlavor, node->right->minFlavor);
				node->maxFlavor = max(node->left->maxFlavor, node->right->maxFlavor);
				node->theMostLeft = min(node->left->theMostLeft, node->right->theMostLeft);
				node->theMostRight = max(node->left->theMostRight, node->right->theMostRight);
				//can splice leaves here
				if (node->left->isLeaf() && node->right->isLeaf()
					&& node->left->minFlavor == node->right->minFlavor
					&& node->left->theMostRight + 1 == node->right->theMostLeft){ 
					TNode *pnode = TNode::newNode
						(node->left->theMostLeft, node->right->theMostRight, node->minFlavor, node->minFlavor);
					return pnode; //spliced kids instead of father
				}
				return node; //father
			}
			//one kid remains, copy contents 
			TNode *psingleKid = NULL;
			if (NULL != node->left && NULL == node->right) {
				psingleKid = node->left;
			}
			if (NULL == node->left && NULL != node->right) {
				psingleKid = node->right;
			}
			return psingleKid;
		}
		//no kids and the instruction cannot cover all, split cases
		if (node->theMostLeft < theLeftest) //split into left not affected and right affected
		{ //node has left outside of the interval
			node->left = TNode::newNode(node->theMostLeft, theLeftest - 1, node->minFlavor, node->maxFlavor);
			node->right = TNode::newNode(theLeftest, node->theMostRight, node->minFlavor, node->maxFlavor);
			node = changeTree(node, k, theLeftest, theRightest, myOnlyFlavor);
		}
		else if (node->theMostRight > theRightest) { //left not affected but right affected
			node->left = TNode::newNode(node->theMostLeft, theRightest, node->minFlavor, node->maxFlavor);
			node->right = TNode::newNode(theRightest + 1, node->theMostRight, node->minFlavor, node->maxFlavor);
			node = changeTree(node, k, theLeftest, theRightest, myOnlyFlavor);
		}
		return node;
	}
	static int solution(int N, int K, vector<int> &A, vector<int> &B, vector<int> &C) {
		vector<int> loc(N);
		for (int i = 1; i <= N; i++)
			loc.at(i - 1) = i;
		vector<int> flav(N, 0);
		TNode *root = sortedArrayToBST(N);
#ifdef MYDEBUG
		preOrder(root);
#endif //#ifdef MYDEBUG
		for (unsigned int i = 0; i < A.size(); i++) {
			root = changeTree(root, C.at(i), A.at(i), B.at(i), -1);
#ifdef MYDEBUG
			cout << "After change " << i + 1 << endl;
			preOrder(root);
#endif //#ifdef MYDEBUG
		}
		pushColorsToLeaves(root, -1);		//make leaves in sync
		int res = countFlavorK(root, K);	//final result
		return res;
	}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//		DRIVER /////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

	// Driver Code
	/* A utility function to print
	preorder traversal of BST */
	static void preOrder(TNode* node)
	{
		if (node == NULL) {
			//cout << "Empty tree\n";
			return;
		}

		cout << "(l:" << node->theMostLeft << ", r: " << node->theMostRight
			<< " minfl:" << node->minFlavor << " maxfl:" << node->maxFlavor << ") " << (node->isLeaf() ? " Leaf " : "") << endl;
		preOrder(node->left);
		preOrder(node->right);
	}
	static ostream& operator<<(ostream& os, const vector< int>& v) {
		for (auto iter = v.begin(); iter != v.end(); iter++)
			if ((*iter) >= 0)
				os << (*iter) << " ";
			else
				os << "-" << " ";
		return os;
	}
	static int solutionSilver(int N, int K, vector<int> &A, vector<int> &B, vector<int> &C) {
		vector<int> stand(N + 1, 0);
		int M = (int)A.size();
		for (int i = 0; i < M; i++) {
			for (int j = A.at(i); j <= B.at(i); j++) {
				if (stand.at(j) == C.at(i) - 1)
					stand.at(j) = C.at(i);
				else
					stand.at(j) = -1;
			}
#ifdef MYDEBUG
			cout << "After layer " << i + 1 << " : " << vector<int>(stand.begin() + 1, stand.end()) << endl;

#endif //#ifdef MYDEBUG
		}
		int res = 0;
		for (int i = 1; i <= N; i++) {
			if (K == stand.at(i))
				res++;
		}
		return res;
	}
	static bool test(int N, int K, vector<int> &A, vector<int> &B, vector<int> &C, int expected) {
		int res = solution(N, K, A, B, C);
		int resSilver = solutionSilver(N, K, A, B, C);
		if (res != resSilver) {
			cout << "Test failed" << endl;
			cout << N << "  " << K << "  " << A << " " << B << " " << C << endl;
			cout << "Got:  " << res << " Expected: " << resSilver << endl;
			return false;
		}
		else
			cout << "Test Passed. Value: " << res << endl;
		return true;
	}
	static bool test1(int N, int K, vector<int> &A, vector<int> &B, vector<int> &C, int expected) {
		int res = solution(N, K, A, B, C);
		if (res != expected) {
			cout << "Test failed. Expected: " << expected << " Got: " << res << endl;
			cout << N << "  " << K << "  " << A << " " << B << " " << C << endl;
			return false;
		}
		else
			cout << "Test Passed. Value: " << expected << endl;
		return true;
	}
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
	static  TNode* sortedArrayToBST(int loc[], int flav[], int start, int end);

int main()
{
	/*

	Test failed
	10  1  10 4 7 7 1 6 6 10 1 9  10 4 7 7 9 8 9 10 1 10  1 1 1 1 1 1 1 1 1 1
	Got:  2 Expected: 3

	*/
	bool res12 = test
	(10, 1, vector<int>({ 10, 4, 7, 7, 1, 6, 6, 10, 1, 9 }), vector<int>({ 10, 4, 7, 7, 9, 8, 9, 10, 1, 10 }),
		vector<int>({ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }), 3);
	if (!res12)
		return 0;
	/*
	Test failed
	4  1  3 1  4 3  1 1
	Got:  4 Expected: 3
	*/
	bool res11 = test
	(4, 1, vector<int>({ 3, 1 }), vector<int>({ 4, 3 }), vector<int>({ 1, 1 }), 3);
	if (!res11)
		return 0;
	/*
	10  1  10 1 5 9 5 7 9 10 10 2  10 3 5 9 7 9 10 10 10 10  1 1 1 1 1 1 1 1 1 1
	*/
	bool res10 = test
	(11, 3, vector<int>({ 10, 1, 5, 9, 5, 7, 9, 10, 10, 2 }), vector<int>({ 10, 3, 5, 9, 7, 9, 10, 10, 10, 10 }), vector<int>({ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }), 4);
	if (!res10)
		return 0;
	/*
	Test failed
	11  3  5 1 2 6 11 1  7 4 7 10 11 11  1 1 2 2 1 3
	Got:  0 Expected: 4
	*/
	bool res7 = test
	(11, 3, vector<int>({ 5, 1, 2, 6, 11, 1 }), vector<int>({ 7, 4, 7, 10, 11, 11 }), vector<int>({ 1, 1, 2, 2, 1, 3 }), 4);
	if (!res7)
		return 0;
	/*
	Test failed
	9  2  8 3 6 2 5 9 4 5 3  8 5 6 5 7 9 9 6 3  1 2 2 1 1 1 2 1 2
	Got:  1 Expected: 3
	*/
	bool res9 = test
	(9, 2, vector<int>({ 8, 3, 6, 2, 5, 9, 4, 5, 3, }), vector<int>({ 8, 5, 6, 5, 7, 9, 9, 6, 3, }),
		vector<int>({ 1, 2, 2, 1, 1, 1, 2, 1, 2, }), 2);
	if (!res9)
		return 0;
	/*
	Test failed
	6  3  6 5 1 1 1 3 6  6 6 2 5 4 3 6  1 3 1 2 3 3 2
	Got:  0 Expected: 2
	*/
	bool res6 = test
	(6, 3, vector<int>({ 6, 5, 1, 1, 1, 3, 6 }), vector<int>({ 6, 6, 2, 5, 4, 3, 6 }), vector<int>({ 1, 3, 1, 2, 3, 3, 2 }), 2);
	if (!res6)
		return 0;
	/*
	Test failed
	9  2  6 9 2 2 1 4 2 5 1 8  9 9 4 6 3 8 4 8 1 8  1 1 1 1 2 2 2 1 2 2
	Got:  7 Expected: 0
	*/
	bool res8 = test
	(9, 2, vector<int>({ 6,9,2,2,1,4,2,5,1,8 }), vector<int>({ 9,9,4,6,3,8,4,8,1,8 }),
		vector<int>({ 1,1,1,1,2,2,2,1,2,2 }), 2);
	if (!res8)
		return 0;


	// 2  1  1 2 1  2 2 2  1 1 1  // without balance

	bool res1 = test
	(2, 1, vector<int>({ 1, 2, 1 }), vector<int>({ 2,2,2 }), vector<int>({ 1, 1, 1 }), 1);
	if (!res1)
		return 0;

	/*
	Test failed
	3  1  2 1 3 2  2 1 3 3  1 1 1 1
	Got:  0 Expected: 1
	*/
	bool res5 = test
	(3, 1, vector<int>({ 2, 1, 3, 2 }), vector<int>({ 2, 1, 3, 3 }), vector<int>({ 1, 1, 1, 1 }), 1);
	if (!res5)
		return 0;
	/*Test failed
	7  1  6 5 5  6 5 5  1 1 1
	Got:  0 Expected : 1
	*/
	bool res4 = test
	(7, 1, vector<int>({ 6,  5, 5 }), vector<int>({ 6, 5, 5 }), vector<int>({ 1, 1, 1 }), 1);
	if (!res4)
		return 0;
	//Test failed?
	//3  1  2 1 1  2 1 1  1 1 1
	//Got:  0 Expected : 1
	bool res3 = test
	(3, 1, vector<int>({ 2,  1, 1 }), vector<int>({ 2, 1, 2 }), vector<int>({ 1, 1, 1 }), 1);
	if (!res3)
		return 0;

	// 3  1  1 1    1 2    1 1  // with    balance
	bool res2 = test
	(3, 1, vector<int>({ 1, 1 }), vector<int>({ 1, 2 }), vector<int>({ 1, 1 }), 1);
	if (!res2)
		return 0;


	srand((unsigned int)time(NULL));

	/* generate secret number between 1 and 10: */
	for (int j = 0; j < 10000000; j++)
	{
		int k = 1 + rand() % 15;
		int m = k + rand() % 15;
		int n = 1 + rand() % 15;
		vector<int> A1(m), B1(m), C1(m);
		for (int i = 0; i < m; i++) {
			A1.at(i) = 1 + rand() % n;
			if (n == A1.at(i))
				B1.at(i) = A1.at(i);
			else
				B1.at(i) = A1.at(i) + rand() % (n - A1.at(i) + 1);
			C1.at(i) = 1 + rand() % k;
		}
		if (!test(n, k, A1, B1, C1, 0))
			getchar();
	}
	bool res;

	vector<int> A(20), B(20), C(20, 1);
	for (int i = 0; i < (int)A.size(); i++) {
		A.at(i) = B.at(i) = i + 1;
		C.at(i) = 1;
	}
	res = test
	(20, 1, A, B, C, 20);
	if (!res)
		return 0;
	res = test
	(20, 1, vector<int>({ 1, 11 }), vector<int>({ 10, 20 }), vector<int>({ 1, 1 }), 20);
	if (!res)
		return 0;
	res = test
	(2, 2, vector<int>({ 1, 1 }), vector<int>({ 2, 2 }), vector<int>({ 1, 2 }), 2);
	if (!res)return 0;
	res = test
	(2, 2, vector<int>({ 1, 1 }), vector<int>({ 1, 1 }), vector<int>({ 1, 2 }), 1);
	if (!res)return 0;
	res = test
	(1, 1, vector<int>({ 1 }), vector<int>({ 1 }), vector<int>({ 1 }), 1);

	res = test
	(5, 3, vector<int>({ 1, 1, 4, 1, 4 }), vector<int>({ 5, 2, 5, 5, 4 }), vector<int>({ 1, 2, 2, 3, 3 }), 3);
	res = test
	(5, 5, vector<int>({ 1, 1, 1, 1, 1 }), vector<int>({ 5, 5, 5, 5, 5 }), vector<int>({ 1, 2, 3, 4, 5 }), 5);
	res = test
	(1, 5, vector<int>({ 1, 1, 1, 1, 1 }), vector<int>({ 1, 1, 1, 1, 1 }), vector<int>({ 1, 8, 3, 4, 5 }), 0);
	res = test
	(1, 5, vector<int>({ 1, 1, 1, 1, 1 }), vector<int>({ 1, 1, 1, 1, 1 }), vector<int>({ 1, 2, 3, 4, 5 }), 1);
	res = test
	(5, 4, vector<int>({ 1, 1, 1, 1, 1 }), vector<int>({ 5, 5, 5, 5, 5 }), vector<int>({ 1, 2, 3, 4, 5 }), 0);
	res = test
	(5, 2, vector<int>({ 1, 1, 2 }), vector<int>({ 5, 5, 3 }), vector<int>({ 1, 2, 1 }), 3);
	res = test
	(1, 1, vector<int>({ 1, 1 }), vector<int>({ 1, 1 }), vector<int>({ 1, 1 }), 0);
	res = test
	(6, 4, vector<int>({ 1, 2, 1, 1 }), vector<int>({ 3, 3, 6, 6 }), vector<int>({ 1, 2, 3, 4 }), 2);
	res = test
	(3, 2, vector<int>({ 1, 3, 3, 1, 1 }), vector<int>({ 2, 3, 3, 1, 2 }), vector<int>({ 1, 2, 1, 2, 2 }), 1);

	return 0;
}
/*
There is a cake factory producing K-flavored cakes.
Flavors are numbered from 1 to K.
A cake should consist of exactly K layers, each of a different flavor.
It is very important that every flavor appears in exactly one cake layer and
that the flavor layers are ordered from 1 to K from bottom to top.
Otherwise the cake doesn't taste good enough to be sold. For example, for K = 3, cake [1, 2, 3] is well-prepared and can be sold, whereas cakes [1, 3, 2] and [1, 2, 3, 3] are not well-prepared.

The factory has N cake forms arranged in a row, numbered from 1 to N. Initially, all forms are empty. At the beginning of the day a machine for producing cakes executes a sequence of M instructions
(numbered from 0 to M-1) one by one. The J-th instruction adds a layer of flavor C[J] to all forms from A[J] to B[J], inclusive.

What is the number of well-prepared cakes after executing the sequence of M instructions?

Write a function:

int solution(int N, int K, vector<int> &A, vector<int> &B, vector<int> &C);

that, given two integers N and K and three arrays of integers A, B, C describing the sequence, returns the number of well-prepared cakes after executing the sequence of instructions.


Write an efficient algorithm for the following assumptions:

N is an integer within the range [1...100,000];
M is an integer within the range [1...200,000];
each element of arrays A, B is an integer within the range [1...N];
each element of array C is an integer within the range [1...K];
for every integer J, A[J] <= B[J];
arrays A, B and C have the same length, equal to M.








Examples:

1. Given N = 5, K = 3, A = [1, 1, 4, 1, 4], B = [5, 2, 5, 5, 4] and C = [1, 2, 2, 3, 3].

There is a sequence of five instructions:

The 0th instruction puts a layer of flavor 1 in all forms from 1 to 5.
The 1st instruction puts a layer of flavor 2 in all forms from 1 to 2.
The 2nd instruction puts a layer of flavor 2 in all forms from 4 to 5.
The 3rd instruction puts a layer of flavor 3 in all forms from 1 to 5.
The 4th instruction puts a layer of flavor 3 in the 4th form.
The picture describes the first example test.

The function should return 3. The cake in form 3 is missing flavor 2, and the cake in form 5 has additional flavor 3. The well-prepared cakes are forms 1, 2 and 5.

2. Given N = 6, K = 4, A = [1, 2, 1, 1], B = [3, 3, 6, 6] and C = [1, 2, 3, 4],

The picture describes the second example test.

the function should return 2. The 2nd and 3rd cakes are well-prepared.

3. Given N = 3, K = 2, A = [1, 3, 3, 1, 1], B = [2, 3, 3, 1, 2] and C = [1, 2, 1, 2, 2],

The picture describes the third example test.

the function should return 1. Only the 2nd cake is well-prepared.

4. Given N = 5, K = 2, A = [1, 1, 2], B = [5, 5, 3] and C = [1, 2, 1],

The picture describes the fourth example test.

the function should return 3. The 1st, 4th and 5th cakes are well-prepared.

*/

/*
Old Silver
https://app.codility.com/cert/view/certJ4QDG7-RYYJQPGVUXVYQ9ZU/
https://app.codility.com/cert/view/certJ4QDG7-RYYJQPGVUXVYQ9ZU/details/
Silver 2
https://app.codility.com/cert/view/certDTPSRE-Y2897F5FQTUWE5V3/
https://app.codility.com/cert/view/certDTPSRE-Y2897F5FQTUWE5V3/details/`     it has one performance good
Silver 3:
https://app.codility.com/cert/view/certW69ZHY-V6VWDUMJQYN7B9SA/details/
Silver 3 with balancing on every instruction: worse!
Silver 4
https://app.codility.com/cert/view/cert7SJ4K4-MMWMKJVUZS3K39R2/details/
many_colors									OK
medium_perf_package							TIMEOUT ERROR
big_perf_package							TIMEOUT ERROR
Detected time complexity:
O(M*N) or O(M*sqrt(N))
silver 5
https://app.codility.com/cert/view/certMBYU2R-58S3J8DS5JE4S2AT/details/
Again performance issues
*/

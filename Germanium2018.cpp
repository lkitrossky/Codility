// Germanium2018.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
//#define PRINT_DEDUG

/* 

Golden
https://app.codility.com/cert/view/cert2NNXAB-WGEW6CXU55SZD9T8/
Assessment:
https://app.codility.com/cert/view/cert2NNXAB-WGEW6CXU55SZD9T8/details/

 
Codility confirms that a person who identified to us on-line as:
Levi Kitrossky
was awarded
Codility Golden Award for the Germanium 2018 Challenge
https://app.codility.com/programmers/challenges/germanium2018/
This is the 27th Codility Golden Award granted this month.
Mine is 34th
The first three: 46 min, 4 hours, 6 hours
Relatively complex

AWARD TYPE:Codility Golden Award (Germanium 2018)
EXPIRATION DATE:2020-07-21 17:00:00 GMT
CHALLENGE POSTED:2018-07-21 17:00:00 GMT
CHALLENGE SOLVED:2018-08-01 14:23:47 GMT
TIME SINCE POSTED:10 days
ASSESSMENT:correct functionality and scalability


		Main ideas
1. Numbers are big, but... we demand a contunous sequence from 1 to the last number,
but there are <= 100 000 numbers. So, the biggest possible is really 100 000, take it as the first hypothesis
1A. Due to the same considerations, the biggest number cannot be bigger than N (amount of numbers)
2. We can also pass over all numbers and take the biggest which is <= N, its is the biggest possible. 
We call it NN
3. THe next big idea is that the set of cards can be seen as a graph.
The graph has nodes 0,..., NN-1, which are translated to the original numbers as +1 and back as -1
4. Card like 3,4 is translated into nodes 2,3 with edge 2-3
5. I made a mistake to think that the graph is directed, but we can pass 3 to 4 and back, so
the graph is undirected
6. Card doubles like 3,3 means that 3 can be chosen and cannot be a problem
7. Card that has one realistic numebr and too big number like 3, 200 000 means that 3 is OK without any problem
8. It is true also for cards like 3,x if x has surely no problems, like set 3,4; 4,4. Or 3,4; 4, 200 000.
So, the easiest way to translate to graph is 
	8A. to discard all x, y: x,y> NN
	8B. all cards like x, 200 000 to translate into x,x. 
	8C. all cards like x,y: x,y < NN and x,y >0 translate into edge x-1, y-1 of the Graph with nodes 1...NN
Every node has a list/vector of adjacents, so all edges are vector<vector<int> >
9. Card duplicates like 3,4 and 3,4 again mean that both numbers can set without any problems
10. Cases like 2,2 or (2,3 and 3,2) or ( 2, 200000) are all translated into loops in Graph.
11. Now we see it as partoculra case of cycle of any length. Say, cards 2,3; 3,4; 4,10; 10, 2;  
produce all their numbers without any trouble, just like it is shown.
12. Any card that is connected to a cycle buy and edge can have no prpblem, since its pair is promised according to 8
13. As a consequence if there is a connected node subset of graph which contains a cycle, all cycle has no problem.
So, we can apply one of the knows alorithms to find loops like https://www.geeksforgeeks.org/detect-cycle-undirected-graph/
with an addition to propagation
14. We can also designate all nodes in the grapha as connected to a cycle, cycled[]. And we can propagate the propery
Just start with some in a cycle, set it as a cycled, then pass over its adjacents skipping the cycled and calling
the same function on the adjacent.
15. Using a combination of the knwon algo to detect cycles in a cycle with skipping on cycled and propagation, we
find all nodes connected to cycles
All such nodes are safe, they cannot be a problem
16. But remain nodes not connected to any cycle, their probkem can be simplified but cutting into separated
graphs which have no common edges or nodes.
17. But what to do with them? It can be a linear branch or branches that cross one another. All numbers are different.
18. With some difficulty I understood that any such set has only one problematic number - maximum of the set.
It can be proven, paying attention that cross only makes thing better, but the maximum stays still. 
If we show the maximum, some other number will be covered.
19. So we cut all non-cycled into connected separated entities and find maximum in every one.
20. Then we find the minimum of them, it is the answer.

*/

/////////////////////////////////////////////////////////////////////////////////
//			BEGINNG OF SOLUTION
////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <algorithm>
using namespace std;

class Graph
{
private:
	int V;    // No. of vertices
	vector< vector<int> >adj;    // edges grouped by nodes
	bool isCyclicUtil(int v, vector<bool> & visited, int parent);
	vector<bool> cyclic;
	vector<int> maxInNonCyclicFragments;
	int findMax(int v);
	vector<bool>visitedNonCyclic;
	void setPropagateCycle(int v);
public:
	Graph(int V);   // Constructor
	void addEdge(int v, int w);   // to add an edge to graph
	bool isCyclic();   // returns true if there is a cycle and also designates all connected to a cycle
	int getSize() const { return V; }
	int GetAnswer();
};
Graph::Graph(int V)
{
	this->V = V;
	adj = vector< vector<int> >(V, vector<int>());
	visitedNonCyclic = cyclic = vector<bool>(V, false);
}
void Graph::addEdge(int v, int w)
{
	adj[v].push_back(w); 
	adj[w].push_back(v); 
}
void Graph::setPropagateCycle(int v) {
	if (cyclic[v])
		return;
	cyclic[v] = true;
	for (auto i = adj[v].begin(); i != adj[v].end(); ++i)
	{
		setPropagateCycle(*i);
	}
}
bool Graph::isCyclicUtil(int v, vector<bool> & visited, int parent)
{
	if(cyclic[v])
		return true;
	// Mark the current node as visited
	visited[v] = true;

	// Recur for all the vertices adjacent to this vertex
	vector<int>::iterator i;
	for (i = adj[v].begin(); i != adj[v].end(); ++i)
	{
		// If an adjacent is not visited, then recur for that adjacent
		if (!visited[*i])
		{
			if (isCyclicUtil(*i, visited, v)) {
				setPropagateCycle(v);
				return true;
			}
		}

		// If an adjacent is visited and not parent of current vertex,
		// then there is a cycle.
		else if (*i != parent){
			setPropagateCycle(v);
			return true;
		}
		if (cyclic[*i]) {
			setPropagateCycle(v);
			return true;
		}
	}
	return false;
}
bool Graph::isCyclic()
{
	// Mark all the vertices as not visited and not part of recursion
	// stack
	vector<bool> visited(V, false);

	// Call the recursive helper function to detect cycle in different
	// DFS trees
	bool res = false;
	for (int u = 0; u < V; u++)
		if (!visited[u] && !cyclic[u]){ // Don't recur for u if it is already visited{
			if (isCyclicUtil(u, visited, -1)) {
				res = true;
				visited = vector<bool>(V, false);  //there was retur true originally
			}
	}
	return res;
}
int Graph::findMax(int v) {
	if (cyclic[v])
		return -1;
	if (visitedNonCyclic.at(v))
		return -1;
	int res = v;
	visitedNonCyclic.at(v) = true;
	for (auto & u2 : adj.at(v)) {
		res = max(res, findMax(u2));
	}
	return res;
}
int Graph::GetAnswer() {

	int res = V;  //cannot be less than, after extract must add 1
	for (int u = 0; u < V; u++) {
		maxInNonCyclicFragments.push_back( findMax(u));
	}
#ifdef PRINT_DEDUG
	cout << "Temp maxInNonCyclicFragments" << endl;
#endif // PRINT_DEDUG
	for (auto & u : maxInNonCyclicFragments) {
#ifdef PRINT_DEDUG
		cout << u << " ";
#endif // PRINT_DEDUG
		if(u >= 0)
			res = min(res, u);
	}
#ifdef PRINT_DEDUG
	cout << endl;
#endif // PRINT_DEDUG
	return res;
}
int solution(vector<int> &A, vector<int> &B) {

	const int N = (int)A.size();
		
	const int MAX_AMOUNT = 100001;
	vector<bool> present(MAX_AMOUNT, false);
	
	for (auto &au : A) {
		if (au <= N) {
			present.at(au) = true;
		}
	}
	for (auto &au : B) {
		if (au <= N) {
			present.at(au) = true;
		}
	}
	int MAX_POSSIBLE = N;
	for (int i = 1; i <= N; i++) {
		if (false == present.at(i)) {
			MAX_POSSIBLE = i - 1;
			break;
		}
	}
	
	Graph graph(MAX_POSSIBLE);

	for (int i = 0; i < N; i++) {
		if (A.at(i) > MAX_POSSIBLE && B.at(i) > MAX_POSSIBLE) {
			continue;  //this card is useless
		}
		int mi = min(A.at(i), B.at(i));
		int ma = max(A.at(i), B.at(i));
		if (A.at(i) > MAX_POSSIBLE || B.at(i) > MAX_POSSIBLE) {		
			graph.addEdge(mi - 1, mi - 1); //A or B is free, no obstacles, because its partner is unrealistically big. The (a,a) will result in cicle detected
		} else{   // -1 sonce Graph supposes 0...
			graph.addEdge(mi - 1, ma - 1); //we keep direction, so use min and max, 
			//also sometimes some of some duplicate pairs (a,b)(a.b), will result in a cycle detection cycle
		}
	}
	graph.isCyclic();
	int res = 1 + graph.GetAnswer();
	return res;
}

#include <iostream>
void test(vector<int> &A, vector<int> &B, int expected, bool printAll = false) {
	int res = solution(A, B);
	if (expected != res || printAll) {
		for (size_t i = 0; i < A.size(); i++) {
			cout << A.at(i) << " ";
		}
		cout << endl;
		for (size_t i = 0; i < B.size(); i++) {
			cout << B.at(i) << " ";
		}
		cout << endl;
		if(expected != res )
			cout << "Error! Expected: " << expected << "  ";
		else
			cout << "Expected: " << expected << "  ";
	}
	cout << " Result: " << res << endl;
}
/////////////////////////////////////////////////////////////////////////////////
//			END OF SOLUTION
////////////////////////////////////////////////////////////////////////////////
int main()
{
	
	
	Graph g1(8);
	g1.addEdge(0, 1); 
	g1.addEdge(1, 2);
	g1.addEdge(2, 3);
	//g1.addEdge(3, 3);
	//g1.addEdge(4, 4);
	g1.addEdge(4, 5);
	g1.addEdge(5, 6);
	g1.addEdge(6, 7);

	/*
	g1.addEdge(0, 2);
	g1.addEdge(2, 0);
	g1.addEdge(0, 3);
	g1.addEdge(3, 4);
	*/
	g1.isCyclic() ? cout << "Graph contains cycle\n" :
		cout << "Graph doesn't contain cycle\n";
	cout<< "Answer: " << g1.GetAnswer() << endl;
	
//	return 0;
	vector<int>VA;
	vector<int>VB;

	int A4[] = { 1, 1, 1, 1, 1 };
	int B4[] = { 2, 3, 4, 5, 6 };
	VA = vector<int>(A4, A4 + 1);
	VB = vector<int>(B4, B4 + 1);
	test(VA, VB, 2, true);

	int A0[] = { 1, 1 };
	int B0[] = { 2, 2 };
	VA = vector<int>(A0, A0 + 2);
	VB = vector<int>(B0, B0 + 2);
	test(VA, VB, 3);

	int A[] = { 1, 2, 4, 3 };
	int B[] = { 1, 3, 2, 3 };
	VA = vector<int>(A, A + 4);
	VB = vector<int>(B, B + 4);
	test(VA, VB, 5);

	int A2[] = { 4, 2, 1, 6, 5 };
	int B2[] = { 3, 2, 1, 7, 7 };
	VA = vector<int>(A2, A2 + 5);
	VB = vector<int>(B2, B2 + 5);
	test(VA, VB, 4);

	int A3[] = { 2, 3 };
	int B3[] = { 2, 3 };
	VA = vector<int>(A3, A3 + 2);
	VB = vector<int>(B3, B3 + 2);
	test(VA, VB, 1);


    return 0;
}

#include <vector>
#include <list>
/*
Gold medal
https://app.codility.com/programmers/custom_challenge/doge2021/
https://app.codility.com/cert/view/cert2NMT5F-2HJFENDSV7P4KRF7/
https://app.codility.com/cert/view/cert2NMT5F-2HJFENDSV7P4KRF7/details/

*/
using namespace std;
// GraphWithWeight class represents a undirected graph with weighted nodes
class GraphWithWeight {
	int V; // No. of vertices
	// Pointer to an array containing adjacency lists
	list<int>* adj;
	//weights of nodes, in out case it is -1,0,1. O is "balanced"
	vector<int> weight;
	int componentWeight; //while we split graph into components we also calc weight of components
	// A function used by DFS to split and weigh
	void DFSUtil(int v, bool visited[]);  //standard depth first search

public:
	GraphWithWeight(int V, const vector<int>&  _weight); // Constructor
	~GraphWithWeight();
	void addEdge(int v, int w);
	bool connectedComponentsCheck();
};

// Method to print connected components in an
// undirected graph
bool GraphWithWeight::connectedComponentsCheck()
{
	// Mark all the vertices as not visited
	bool* visited = new bool[V];
	for (int v = 0; v < V; v++)
		visited[v] = false;
	componentWeight = 0;
	for (int v = 0; v < V; v++) {
		if (visited[v] == false) {
			// found not visited, so a component started
			componentWeight = 0;
			DFSUtil(v, visited);
			//component finished
			if (0 != componentWeight)
				return false;
		}
	}
	delete[] visited;
	return true; //there were no components with sum != 0
}

void GraphWithWeight::DFSUtil(int v, bool visited[])
{
	// Mark the current node as visited and print it
	visited[v] = true;
	componentWeight += weight[v]; //and add its weight to the compoment's weight
	list<int>::iterator i;
	for (i = adj[v].begin(); i != adj[v].end(); ++i)
		if (!visited[*i])
			DFSUtil(*i, visited);
}

GraphWithWeight::GraphWithWeight(int V, const vector<int>&  _weight)
{
	this->V = V;
	weight = _weight;
	adj = new list<int>[V];
	componentWeight = 0;
}

GraphWithWeight::~GraphWithWeight() { delete[] adj; }

// method to add an undirected edge
void GraphWithWeight::addEdge(int v, int w)
{
	adj[v].push_back(w);
	adj[w].push_back(v);
}
bool solution(vector<int> &P, vector<int> &T, vector<int> &A, vector<int> &B) {
	const int N = (int)P.size();
	const int M = (int)A.size();
	vector<int>PT(N); //instead of 1,2 it will be -1,0,1. 
	//If a subset know each other and sum=0, it is good
	for (int i = 0; i < N; i++)
		PT.at(i) = P.at(i) - T.at(i);
	GraphWithWeight graph(N, PT);
	for (int i = 0; i < M; i++)
		graph.addEdge(A.at(i), B.at(i));
	//is is a graph and we need to cut it into components
	//then sum PT o every component, if != 0 - false
	return graph.connectedComponentsCheck();
}
#include <iostream>
ostream & operator << (ostream & os, const vector<int> & v) {
	for (auto iter = v.begin(); iter != v.end(); iter++)
		os << *iter << " ";
	return os;
}
int main() {
	cout << "Hello from Doge!" << endl;
	int P3[] = { 2, 2, 2, 2, 1, 1, 1, 1 }, T3[] = { 1, 1, 1, 1, 2, 2, 2, 2 }, A3[] = { 0, 1, 2, 3, 4, 5, 6 }, B3[] = { 1, 2, 3, 4, 5, 6, 7 };
	vector<int> vP3(P3, P3 + 8), vT3(T3, T3 + 8), vA3(A3, A3 + 7), vB3(B3, B3 + 7);
	cout << vP3 << endl << vT3 << endl << vA3 << endl << vB3 << endl;
	cout << "Solution: " << (solution(vP3, vT3, vA3, vB3) ? "true" : "false") << endl;
	int P2[] = { 1, 1, 2, 2, 1, 1, 2, 2 }, T2[] = { 1, 1, 1, 1, 2, 2, 2, 2 }, A2[] = { 0, 2, 4, 6 }, B2[] = { 1, 3, 5, 7 };
	vector<int> vP2(P2, P2 + 8), vT2(T2, T2 + 8), vA2(A2, A2 + 4), vB2(B2, B2 + 4);
	cout << vP2 << endl << vT2 << endl << vA2 << endl << vB2 << endl;
	cout << "Solution: " << (solution(vP2, vT2, vA2, vB2) ? "true" : "false") << endl;
	int P1[] = { 2, 2, 1, 1, 1 }, T1[] = { 1, 1, 1, 2, 2 }, A1[] = { 0, 1, 2, 3 }, B1[] = { 1, 2, 0, 4 };
	vector<int> vP1(P1, P1 + 5), vT1(T1, T1 + 5), vA1(A1, A1 + 4), vB1(B1, B1 + 4);
	cout << vP1 << endl << vT1 << endl << vA1 << endl << vB1 << endl;
	cout << "Solution: " << (solution(vP1, vT1, vA1, vB1) ? "true" : "false") << endl;
	int P[] = { 1, 1, 2 }, T[] = { 2, 1, 1 }, A[] = { 0, 2 }, B[] = { 1, 1 };
	vector<int> vP(P, P+3), vT(T, T+3), vA(A, A+2), vB(B, B+2);
	cout << vP << endl << vT << endl << vA << endl << vB << endl;
	cout << "Solution: " << (solution(vP, vT, vA, vB)? "true":"false")<<endl;
	return 0;
}
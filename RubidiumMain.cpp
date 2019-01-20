#include <vector>
#include <algorithm>
using namespace std;
int MAXF = 100000;

int solutionSilver(vector<int> &X, vector<int> &Y);


// Golden
// https://app.codility.com/cert/view/certG8TW44-N6W6NX2M5EY9ASWN/

class Point {
public:
	Point(int _x, int _y) :x(_x), y(_y) {}
	static unsigned int dx(const Point &f, const Point &s) { return abs(f.x - s.x); }
	static unsigned int dy(const Point &f, const Point &s) { return abs(f.y - s.y); }
	static unsigned int distance(const Point &f, const Point &s) { return max(dx(f, s), dy(f, s)); }
	const int getX() { return x; }
	const int getY() { return y; }
private:
	int x;
	int y;
};

typedef vector<Point>  Vertical;
typedef vector<Vertical>  Verticals;
typedef vector<Point>  Horizontal;
typedef vector<Horizontal>  Horizontals;

bool sortFunction1(  Point i,   Point j) { 
	if (i.getX() != j.getX())
		return (i.getX() < j.getX());
	return (i.getY() < j.getY());
}


unsigned int absMin = MAXF;



unsigned int dx(const Vertical & v1, const Vertical & v2) {
	return Point::dx(v1.at(0), v2.at(0));
}
unsigned int distance( Vertical & v1,  Vertical & v2) {
	
	const unsigned int localAbdsMin = dx(v1, v2);
	if (localAbdsMin > absMin)
		return absMin;
	unsigned int localMin = MAXF;
	unsigned int i = 0; 
	unsigned int j = 0;
	while (i < v1.size() && j < v2.size()) {
		unsigned int d = Point::distance(v1.at(i), v2.at(j));
		localMin = min(localMin, d);
		if (localAbdsMin == localMin)
			return localMin;
		if (i + 1 < v1.size() && v1.at(i).getY() < v2.at(j).getY())
			i++;
		if (i + 1 >= v1.size() && v1.at(i).getY() < v2.at(j).getY())
			return localMin;
		if (j + 1 < v2.size() && v1.at(i).getY() > v2.at(j).getY())
			j++;
		if (j + 1 >= v2.size() && v1.at(i).getY() > v2.at(j).getY())
			return localMin;
	}
	return localMin;
}


int solution(vector<int> &X, vector<int> &Y) {
	Verticals verticals;
	vector<Point> P;
	for (unsigned int i = 0; i < X.size(); i++) {
		P.push_back(Point(X.at(i), Y.at(i)));
	}
	sort(P.begin(), P.end(), sortFunction1);

	for (unsigned int i = 1; i < X.size(); i++) {
		absMin = min(absMin, Point::distance(P.at(i), P.at(i-1)));
	}
	Vertical currentVertical;
	currentVertical.push_back(P.at(0));

	for (unsigned int i = 1; i < X.size(); i++) {
		if (0 != Point::dx(P.at(i), P.at(i - 1))){
			verticals.push_back(currentVertical);
			currentVertical = Vertical();
		}
		currentVertical.push_back(P.at(i));
		if (X.size() == i + 1) {
			verticals.push_back(currentVertical);
		}
	}

	if (verticals.size() < 2)
		return absMin/2;

	unsigned int indV1 = 0, indV2 = 1;

	while (indV1 < verticals.size() && indV2 < verticals.size()) {
		unsigned int dd = dx(verticals.at(indV1), verticals.at(indV2));
		if (dd >= absMin) {
			indV1++;
			indV2 = indV1 + 1;
		}
		else {
			unsigned int di = distance(verticals.at(indV1), verticals.at(indV2));
			absMin = min(di, absMin);
			indV2++;
			if (indV2 >= verticals.size()) {
				indV1++;
				indV2 = indV1 + 1;
			}
		}
	}
	return absMin/2;
}


int main() {

	int X1[] = { 0, 0, 10, 10 }, Y1[] = { 0, 10, 0, 10 };
	int X2[] = { 1,1,8}, Y2[] = { 1,6,0 };
	int X3[] = { 0,2 }, Y3[] = { 0, 0 };
	int res = solution(vector<int>(X1, X1 + 4), vector<int>(Y1, Y1 + 4));
	int resSi = solutionSilver(vector<int>(X1, X1 + 4), vector<int>(Y1, Y1 + 4));
	res = solution(vector<int>(X2, X2 + 3), vector<int>(Y2, Y2 + 3));
	resSi = solutionSilver(vector<int>(X2, X2 + 3), vector<int>(Y2, Y2 + 3));
	res = solution(vector<int>(X3, X3 + 2), vector<int>(Y3, Y3 + 2));
	resSi = solutionSilver(vector<int>(X3, X3 + 2), vector<int>(Y3, Y3 + 2));
	return 0;
}

/*
Compilation successful.

Example test : ([0, 2], [0, 0])
WRONG ANSWER(got 2 expected 1)

Example test : ([0, 0, 10, 10], [0, 10, 0, 10])
WRONG ANSWER(got 10 expected 5)

Example test : ([1, 1, 8], [1, 6, 0])
WRONG ANSWER(got 5 expected 2)
*/

// https://app.codility.com/cert/view/certHN5YVE-XNUSATH32C3JXU2A/
//silver
int solutionSilver(vector<int> &X, vector<int> &Y) {
	unsigned int minmax = MAXF;
	unsigned int N = X.size();
	for (unsigned int i = 0; i < N; i++) {
		for (unsigned int j = i + 1; j < N; j++) {
			unsigned int m = max(abs(X.at(i) - X.at(j)), abs(Y.at(i) - Y.at(j)));
			minmax = min(minmax, m);
		}
	}
	return minmax / 2;
}
#pragma once
#include "End.h"
#include <tuple>
using namespace std;
class CSegment
{
public:
	CSegment(const CEnd& _startEnd, const CEnd& _startFinish):
	  start(_startEnd), finish(_startFinish){}
	virtual ~CSegment(void);
	std::tuple<double, double, double>coefficients()const;  //a,b,c: ax+by+c=0 fr this line
	CEnd Start()const{return start;}
	CEnd Finish()const{return finish;}
	bool parallel(const CSegment& other)const;
	CEnd intersection(const CSegment& other)const;
	bool intersect(const CSegment& other)const;
	bool PointIsOnTheCloseSegment(const CEnd cend)const;
	double Length()const;
	friend ostream& operator<<(ostream& os, const CSegment cs){
		os<<"Start: "<<cs.Start() <<" Finish: "<<cs.Finish();
		return os;
	}
	bool operator == (CSegment& other)const{
		return 
			(Start()==other.Start() && Finish()==other.Finish())
			||
			(Start()==other.Finish() && Finish()==other.Start());
	}
private:
	CEnd start;
	CEnd finish;
};


#include "StdAfx.h"
#include "Segment.h"


CSegment::~CSegment(void)
{
}
tuple<double, double, double>CSegment::coefficients()const { //a,b,c: ax+by+c=0 fr this line
	double a = finish.Y()-start.Y();
	double b = start.X() - finish.X();
	double c = ( start.Y()*(finish.X()-start.X())  + start.X()*(start.Y()-finish.Y())   );
	return make_tuple(a,b,c);
}
CEnd CSegment::intersection(const CSegment& other)const{
	tuple<double, double, double>line1 = this->coefficients();
	tuple<double, double, double>line2 = other.coefficients();
	double a1 = get<0>(line1);
	double b1 = get<1>(line1);
	double c1 = get<2>(line1);
	double a2 = get<0>(line2);
	double b2 = get<1>(line2);
	double c2 = get<2>(line2);
	double D = a1*b2-a2*b1;
	if(0 == D)
		throw string("Cannot get intersection of parallel or overlapping");
	double Dx = - c1*b2 + c2*b1;
	double Dy = - a1*c2 + a2*c1;
	double x =  Dx/D;
	double y =  Dy/D;
	return CEnd(x,y);
}
bool CSegment::intersect(const CSegment& other)const{
	if(parallel(other))
		return false;
	CEnd intersectionPoint = intersection(other);
	bool condition1 = CSegment(intersectionPoint, start).Length() < Length();
	bool condition2 = CSegment(intersectionPoint, finish).Length() < Length();
	bool condition3 = CSegment(intersectionPoint, other.Start()).Length() < other.Length();
	bool condition4 = CSegment(intersectionPoint, other.Finish()).Length() < other.Length();
	return condition1 && condition2 && condition3 && condition4;
}
bool CSegment::parallel(const CSegment& other)const{
	tuple<double, double, double>line1 = this->coefficients();
	tuple<double, double, double>line2 = other.coefficients();
	double a1 = get<0>(line1);
	double b1 = get<1>(line1);
	double a2 = get<0>(line2);
	double b2 = get<1>(line2);
	return 0 == a1*b2-a2*b1;
}
bool CSegment::PointIsOnTheCloseSegment(const CEnd point)const{
	tuple<double, double, double>line = this->coefficients();
	double value = get<0>(line)*point.X() + get<1>(line)*point.Y() + get<2>(line);
	if(value != 0)
		return false; //the point is not on the line, equation fails
	bool condition1 = (point.X()-start.X())*(point.X()-finish.X()) <= 0;
	bool condition2 = (point.Y()-start.Y())*(point.Y()-finish.Y()) <= 0; 
	return condition1 && condition2; //inside or on one of the ends, so <=
}
double CSegment::Length()const{
	return sqrt( pow(start.X() - finish.X(),2) 
		         + 
		         pow(start.Y() - finish.Y(),2)
			);
}
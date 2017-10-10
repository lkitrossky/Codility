#include "StdAfx.h"
#include "Iceberg.h"


bool CIceberg::order_function(const CSegment& s1, const CSegment& s2){
	std::tuple<double, double, double> t1 = s1.coefficients();
	std::tuple<double, double, double> t2 = s2.coefficients();
	double det = get<0>(t1)*get<1>(t2) - get<1>(t1)*get<0>(t2);
	return det > 0;
}
CIceberg::~CIceberg(void)
{
}
vector<CEnd> CIceberg::NormalizeVortices(vector<CEnd> _vortices){
	if(_vortices.size()<4)  //order in triangle of no importance
		return _vortices;	
	vector<CSegment> diagonals;
	CEnd first = _vortices.back();
	_vortices.pop_back();
	for(auto citer = _vortices.begin();citer != _vortices.end();citer++){
		diagonals.push_back(CSegment(first,*citer));
	}
	sort(diagonals.begin(),diagonals.end(),order_function);
	vector<CEnd> res;
	res.push_back(first);
	for(auto citer = diagonals.cbegin();citer != diagonals.cend();citer++){
		res.push_back(citer->Finish());
	}
	return res;
}
vector<CSegment> CIceberg::BuildEdges(){
	vector<CSegment> res;
	int l = vortices.size();
	for(int i = 0;i < l;i++){
		res.push_back(CSegment(vortices[i], vortices[(i+1)%l]));
	}
	return res;
}
bool CIceberg::SegmentForbidden(const CSegment &cs)const{
	for(auto citer = edges.cbegin();citer != edges.cend();citer++){
		if(citer->intersect(cs))
			return true;
	}
	if(SegmentDiagonal(cs))
		return true;
	return false;
}
ostream& operator<<(ostream &os, const CIceberg &ci){
	os<<"Iceberg vortices "<<endl;
	const vector <CEnd> vortices = ci.vortices;
	for(auto citer = vortices.cbegin(); citer != vortices.cend();citer++){
		os<<*citer<<" ";
	}
	os<<endl;
	os<<"Iceberg edges "   <<endl;
	const vector<CSegment> edges = ci.edges;
	for(auto citer = edges.cbegin(); citer != edges.cend();citer++){
		os<<*citer<<endl;
	}
	return os;
}
bool CIceberg::SegmentDiagonal(const CSegment &cs)const{
	int l = HowManyVortices();
	if(l<4)return false; //no diagonal in triangle or less
	vector<int> vorticesOnSegmentOrderNumber;
	for(auto it = vortices.cbegin(); it != vortices.cend(); it++){
		if(cs.PointIsOnTheCloseSegment(*it)){
			int order = distance(vortices.begin(),it);
			vorticesOnSegmentOrderNumber.push_back(order);
		}
	}
	if(vorticesOnSegmentOrderNumber.size() != 2) 
		return false;  //diagonal only passes two vortices
	int jump = abs(vorticesOnSegmentOrderNumber.at(0) - vorticesOnSegmentOrderNumber.at(1));
	if(jump != 1 && jump != (l-1))
		return true;
	return false;
}
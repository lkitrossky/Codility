#include "StdAfx.h"
#include "Sea.h"


CSea::~CSea(void)
{
}

ostream& operator<<(ostream& os, const CSea &csea){
	os<<"Sea data. Start: "<<csea.Start() <<" Finish: "<<csea.Finish()<<endl;
	vector<CIceberg>icebergs2 = csea.GetIcebergs() ;
	for(auto it = icebergs2.cbegin();it != icebergs2.cend(); it++){
		os<< *it <<endl;
	}
	os<<endl;
	return os;
}
bool CSea::SegmentForbidden(const CSegment &cs)const{
	for(auto it = icebergs.cbegin();it != icebergs.cend(); it++){
		if(it->SegmentForbidden(cs) ){
			return true;
		}
	}
	return false;
}
vector<CEnd> CSea::GetEdges()const{
	vector<CEnd> res;
	for( auto it = icebergs.cbegin(); it != icebergs.cend();it++){
		vector<CEnd> vortices = it->Vortices();
		res.insert(res.end(),vortices.cbegin(),vortices.cend());
	}
	return res;
}
double CSea::GetRouteLength(const vector<CEnd> route){
	double res = 0;
	int l = route.size();
	if(l < 2) return 0;

	for(int i=0;i<l-1;i++){
		res += Distance(route[i], route[i+1]);
	}
	return res;
}
double CSea::Distance(const CEnd from,const CEnd to){
	return CSegment(from, to).Length();
}
vector<CEnd> CSea::GetShortestRoute()const{
	return GetShortestRoute(start,GetEdges());
}
vector<CEnd> CSea::GetShortestRoute(const CEnd from,const vector<CEnd> connectors)const{
	vector<CEnd> res;
	if(SegmentPermitted(CSegment(from, finish))) {  //possible to jump to finish straight
		res.push_back(from);
		res.push_back(finish);
		return res;
	}
	if(connectors.size() < 1){   //and no possibility to jump (checked above), could-de-sack
		return res;
	}
	for(auto citer = connectors.cbegin();citer != connectors.cend(); citer++){
		if(SegmentForbidden(CSegment(from, *citer)))
			continue;
		vector<CEnd> connectors_smaller = connectors;
		vector<CEnd>::iterator position = find(connectors_smaller.begin(), connectors_smaller.end(), *citer);
		if (position != connectors_smaller.end()) 
			connectors_smaller.erase(position);
		//preparing recursion without current edge
		vector<CEnd> res_smaller = GetShortestRoute(*citer,connectors_smaller);
		if(res_smaller.size() > 0){
			res_smaller.insert(res_smaller.begin(),from); 
			if(GetRouteLength(res_smaller) < GetRouteLength(res) || GetRouteLength(res) == 0){
				res = res_smaller;
			}
		}
	}
	return res;
}
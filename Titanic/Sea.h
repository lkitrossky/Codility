#pragma once
class CSea
{
public:
	CSea(CEnd _start, CEnd _finish, vector<CIceberg> _icebergs):
	  start(_start), finish( _finish), icebergs(_icebergs){}
	virtual ~CSea(void);
	friend ostream& operator<<(ostream& os, const CSea &csea);
	CEnd Start()const{return start;}
	CEnd Finish()const{return finish;}
	vector<CIceberg> GetIcebergs()const{return icebergs;}
	vector<CEnd> GetEdges()const;
	bool SegmentForbidden(const CSegment &cs)const;
	bool SegmentPermitted(const CSegment &cs)const{return !SegmentForbidden(cs);}
	vector<CEnd> GetShortestRoute()const;
	vector<CEnd> GetShortestRoute(const CEnd from,const vector<CEnd> connectors)const;
	static double GetRouteLength(const vector<CEnd> route);
	static double Distance(const CEnd from, const CEnd to);
private:
	const CEnd start;
	const CEnd finish;
	const vector<CIceberg> icebergs;
};


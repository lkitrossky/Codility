#pragma once
class CIceberg
{
public:
	CIceberg(const vector<CEnd> &_vortices):
	  vortices(NormalizeVortices(_vortices)), edges(BuildEdges()){}
	virtual ~CIceberg(void);
	int HowManyVortices()const{return vortices.size();}
	vector<CEnd> Vortices()const{return vortices;}
	static vector<CEnd> NormalizeVortices(vector<CEnd> _vortices);
	static bool order_function(const CSegment& s1, const CSegment& s2);
	vector<CSegment> BuildEdges();
	friend ostream& operator<<(ostream &os, const CIceberg &ci);
	bool SegmentDiagonal(const CSegment &cs)const;
	bool SegmentForbidden(const CSegment &cs)const;
	bool SegmentPermitted(const CSegment &cs)const{return !SegmentForbidden(cs);}
private:
	const vector<CEnd> vortices;
	const vector<CSegment> edges;
};


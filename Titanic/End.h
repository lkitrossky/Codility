#pragma once
class CEnd
{
public:
	CEnd(const double& _x, const double& _y):x(_x),y(_y){};
	virtual ~CEnd(void);
	double X() const {return x;}
	double Y() const {return y;}
	friend ostream& operator<<(ostream& os, const CEnd end){
		os<<"x = "<<end.X() <<" y = "<<end.Y();
		return os;
	}
	bool operator == (const CEnd& other)const{
		return X()==other.X() && Y()==other.Y();}
private:
	double x;
	double y;
};


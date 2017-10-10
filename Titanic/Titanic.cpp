// Titanic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

ostream& operator<<(ostream& os, const tuple<double, double, double> t) {
	os<<get<0>(t)<<" "<<get<1>(t)<<" "<<get<2>(t);
	return os;
}
ostream& operator<<(ostream& os, const vector<CEnd> route) {
	os<<"Number of points in the route is: "<< route.size()<< " Length of route is: "<<CSea::GetRouteLength(route)<<endl;
	for ( auto it = route.cbegin();it != route.cend();it++)
		os<<*it<<" ";
	os<<endl;
	return os;
}


int _tmain(int argc, _TCHAR* argv[])
{
	cout<<"Start moving Titanic"<<endl;
	CEnd p1(3,4);
	cout<<"First point p: "<<p1<<endl;
	CEnd p2(3,5);
	cout<<boolalpha<<(p1==p2)<<endl;
	CSegment cs1(p1,p2);
	CSegment cs2(p2,p1);
	tuple<double, double, double> line = cs1.coefficients();
	cout<<line<<endl;
	line = CSegment(CEnd(1,1),CEnd(1,2)).coefficients();
	cout<<line<<endl;
	cout<<"Segments equal? "<<boolalpha<<(cs2==cs1)<<endl;
	line = CSegment(CEnd(1,1),CEnd(2,2)).coefficients();
	cout<<line<<endl;
	line = CSegment(CEnd(1,2),CEnd(2,3)).coefficients();
	cout<<line<<endl;
	line = CSegment(CEnd(1,2),CEnd(2,4)).coefficients();
	cout<<line<<endl;
	line = CSegment(CEnd(1,-3),CEnd(2,-5)).coefficients();
	cout<<line<<endl;
	CSegment lineup = CSegment(CEnd(0,0), CEnd(1,1));
	CSegment linedown = CSegment(CEnd(0,1), CEnd(1,0));
	CEnd inters = lineup.intersection(linedown);
	cout<<"inersection, must be 0.5, 0.5: "<<inters<<" Parallel? "<<boolalpha<<lineup.parallel(linedown)<<
		" Intersect? " <<lineup.intersect(linedown)  <<endl;
	
	CSegment line1 = CSegment(CEnd(0,0), CEnd(1,0));
	CSegment line2 = CSegment(CEnd(0,0), CEnd(0,1));
	CEnd inters1 = line1.intersection(line2);
	cout<<"inersection, must be 0, 0: "<<inters1
		<<" Parallel? "<<boolalpha<<line1.parallel(line2)<<
		" Intersect? " <<line1.intersect(line2)  <<endl;

	CSegment line3 = CSegment(CEnd(0,0), CEnd(1,0));
	CSegment line4 = CSegment(CEnd(0,1), CEnd(1,1));
	cout<<"Segments equal? "<<boolalpha<<(line3==line4)<<endl;
	try {
		CEnd inters2 = line3.intersection(line4);
	}
	catch(string& s){
		cout<<"Exception: "<<s<<endl;
	}
	cout<<"Parallel? "<<boolalpha<<line3.parallel(line4)<<
		" Intersect? " <<line3.intersect(line4)  <<endl;
	
	cout<<"Iceberg operations"<<endl;
	vector<CEnd>svortices, svortices2;
	
	svortices2.push_back(CEnd(0,5));
	svortices2.push_back(CEnd(1,6));
	svortices2.push_back(CEnd(0,7));
	svortices2.push_back(CEnd(1,8));
	
	svortices.push_back(CEnd(2,4));
	svortices.push_back(CEnd(1,0));
	svortices.push_back(CEnd(3,2));
	svortices.push_back(CEnd(3,0));
	svortices.push_back(CEnd(0,1));
	vector<CEnd>vortices = CIceberg::NormalizeVortices(svortices);
	for(auto citer = svortices.cbegin(); citer!=svortices.cend(); citer++){
		cout<<*citer<<" ";
	}
	cout<<endl;
	for(auto citer = vortices.cbegin(); citer!=vortices.cend(); citer++){
		cout<<*citer<<" ";
	}
	cout<<endl;
	CIceberg cIceberg(svortices);
	cout<<cIceberg<<endl;
	CIceberg cIceberg2(svortices2);
	cout<<cIceberg<<endl;
	vector<CIceberg> vicebergs;
	vicebergs.push_back(cIceberg);
	vicebergs.push_back(cIceberg2);
	CSea sea1(CEnd(0,0), CEnd(3,9), vicebergs);
	cout<<sea1<<endl;
	cout<<"If there is a straight way? "<<boolalpha<<sea1.SegmentPermitted(CSegment(CEnd(0,0), CEnd(3,9)))<<endl;
	cout<<"Can pass vertically? "<<boolalpha<<sea1.SegmentPermitted(CSegment(CEnd(0,0), CEnd(0,10)))<<endl;
	cout<<"Shorted route: "<<sea1.GetShortestRoute()<<endl;
	CSea sea2(CEnd(0,0), CEnd(3,3), vicebergs);
	cout<<"Sea 2"<<endl<<sea2<<endl;
	cout<<"Shorted route: "<<sea2.GetShortestRoute()<<endl;
	
	vector <CEnd> ice1vort;
	ice1vort.push_back(CEnd(1,2));
	ice1vort.push_back(CEnd(5,2));
	ice1vort.push_back(CEnd(1,3));
	ice1vort.push_back(CEnd(5,3));
	CIceberg shice1(ice1vort);

	vector <CEnd> ice2vort;
	ice2vort.push_back(CEnd(0,4));
	ice2vort.push_back(CEnd(2,4));
	ice2vort.push_back(CEnd(0,6));
	ice2vort.push_back(CEnd(2,6));
	CIceberg shice2(ice2vort);

	vector <CEnd> ice3vort;
	ice3vort.push_back(CEnd(4,4));
	ice3vort.push_back(CEnd(6,4));
	ice3vort.push_back(CEnd(4,6));
	ice3vort.push_back(CEnd(6,6));
	CIceberg shice3(ice3vort);

	vector<CIceberg>licebergs;
	licebergs.push_back(ice1vort);
	licebergs.push_back(ice2vort);
	licebergs.push_back(ice3vort);

	CSea lsea(CEnd(-0.5,3), CEnd(3,7), licebergs);
	cout<<"lsea"<<endl<<lsea<<endl;
	cout<<"Shorted route: "<<lsea.GetShortestRoute()<<endl;

	vector <CEnd> shice1vort;
	shice1vort.push_back(CEnd(1,2));
	shice1vort.push_back(CEnd(6,2));
	shice1vort.push_back(CEnd(6,4));
	shice1vort.push_back(CEnd(2,4));
	CIceberg shice12(shice1vort);

	vector <CEnd> ice2vortsh;
	ice2vortsh.push_back(CEnd(5,6));
	ice2vortsh.push_back(CEnd(11,7));
	ice2vortsh.push_back(CEnd(9,10));
	CIceberg shice22(ice2vortsh);

	vector <CEnd> shice3vort;
	shice3vort.push_back(CEnd(0,7));
	shice3vort.push_back(CEnd(4,8));
	shice3vort.push_back(CEnd(0,12));
	CIceberg shice32(shice3vort);

	vector<CIceberg>shlicebergs;
	shlicebergs.push_back(shice12);
	shlicebergs.push_back(shice22);
	shlicebergs.push_back(shice32);

	CSea shsea(CEnd(2,1), CEnd(2,20), shlicebergs);
	cout<<"shsea"<<endl<<shsea<<endl;
	cout<<"Shorted route: "<<shsea.GetShortestRoute()<<endl;

	return 0;
}


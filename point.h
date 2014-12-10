#ifndef _POINT_H
#define _POINT_H
#include "matrix.h"

class point{
public:
	double x;
	double y;
	double z;
	double w;
	
	point();
	point(double xx,double yy,double zz);
	point(double xx,double yy,double zz,double ww);
	
	double magnitude();
	
	point s_times(double scalar);	
	point times(matrix matt);
	point cross(point other);

	point operator=(const point& r){
		x = r.x;
		y = r.y;
		z = r.z;
		w = r.w;
	}
	
	point operator-(const point& r){
		double nx = x-r.x;
		double ny = y-r.y;
		double nz = z-r.z;
		//double nw = w-r.w;

		return point(nx,ny,nz,1);
	}

};
#endif

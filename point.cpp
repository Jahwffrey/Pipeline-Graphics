#include "point.h"
#include "matrix.h"
#include "commonvars.h"
#include "math.h"
#include <iostream>

point::point(){
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}

point::point(double xx,double yy, double zz){
	x = xx;
	y = yy;
	z = zz;
	w = 1;

	(*this) = (*this).times(mainMatrix);
}

point::point(double xx,double yy, double zz,double ww){
	x = xx;
	y = yy;
	z = zz;
	w = ww;
}

double point::magnitude(){
	return pow(x*x + y*y + z*z,.5);
}

point point::s_times(double scalar){
	return point(x*scalar,y*scalar,z*scalar);
}

point point::times(matrix matt){
	double* mat = matt.values;
	double xx = mat[0]*x + mat[1]*y + mat[2]*z + mat[3]*w;
	double yy = mat[4]*x + mat[5]*y + mat[6]*z + mat[7]*w;
	double zz = mat[8]*x + mat[9]*y + mat[10]*z + mat[11]*w;
	double ww = mat[12]*x + mat[13]*y + mat[14]*z + mat[15]*w;	

	return point(xx,yy,zz,ww);
}

point point::cross(point other){
	double newX = y*other.z - z*other.y;
	double newY = z*other.x - x*other.z;
	double newZ = x*other.y - y*other.x;
	
	return point(newX,newY,newZ,1);
}


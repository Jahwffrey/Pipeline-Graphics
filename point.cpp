#include "point.h"
#include "matrix.h"

point::point(){
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}

point::point(float xx,float yy, float zz){
	x = xx;
	y = yy;
	z = zz;
	w = 1;
}

point::point(float xx,float yy, float zz,float ww){
	x = xx;
	y = yy;
	z = zz;
	w = ww;
}

point point::times(matrix matt){
	float* mat = matt.values;
	float xx = mat[0]*x + mat[1]*x + mat[2]*x + mat[3]*x;
	float yy = mat[4]*y + mat[5]*y + mat[6]*y + mat[7]*y;
	float zz = mat[8]*z + mat[9]*z + mat[10]*z + mat[11]*z;
	float ww = mat[12]*w + mat[13]*w + mat[14]*w + mat[15]*w;	

	return point(xx,yy,zz,ww);
}

#ifndef _POINT_H
#define _POINT_H
#include "matrix.h"

class point{
public:
	float x;
	float y;
	float z;
	float w;
	
	point();
	point(float xx,float yy,float zz);
	point(float xx,float yy,float zz,float ww);
	
	point times(matrix matt);
};
#endif
#ifndef _TRIANGLE_H
#define _TRIANGLE_H
#include "commonvars.h"
#include "point.h"

class triangle{
public:	
	point p1;
	point p2;
	point p3;
	bool drawLine12;
	bool drawLine23;
	bool drawLine31;
	
	triangle(point p11,point p22,point p33);
	triangle(point p11,point p22,point p33,bool dLine12,bool dLine23,bool dLine31);
	triangle(float* p11,float* p22,float* p33);

	void drawT();
	void drawT(bool drawLine12,bool drawLine23,bool drawLine31);
};

#endif

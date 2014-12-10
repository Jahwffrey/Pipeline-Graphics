#include "matrix.h"
#include "triangle.h"

/*
class triangle{
	point p1;
	point p2;
	point p3;
	
	triangle(point p11,point p22,point p33);
	triangle(float* p11,float* p22,float* p33);
	triangle(point p11,point p22,point p33,bool dLine12,bool dLine23,bool dLine31);

	void drawT();
	void drawT(bool drawLine12,bool drawLine23,bool drawLine32);
};
*/

triangle::triangle(point p11,point p22,point p33){
	p1 = p11;
	p2 = p22;
	p3 = p33;

	drawLine12 = true;
	drawLine23 = true;
	drawLine31 = true;
}

triangle::triangle(float* p11,float* p22,float* p33){
	p1 = point (p11[0],p11[1],p11[2]);
	p2 = point (p22[0],p22[1],p22[2]);
	p3 = point (p33[0],p33[1],p33[2]);
	drawLine12 = true;
	drawLine23 = true;
	drawLine31 = true;

}

triangle::triangle(point p11,point p22,point p33,bool dLine12,bool dLine23,bool dLine31){
	p1 = p11;
	p2 = p22;
	p3 = p33;

	drawLine12 = dLine12;
	drawLine23 = dLine23;
	drawLine31 = dLine31;

}

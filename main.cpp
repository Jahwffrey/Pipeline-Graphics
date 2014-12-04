//John Thomason Assignment 4 Main File
#include <fstream>
#include "matrix.h"
#include "commonvars.h"
#include "point.h"
#include "triangle.h"
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

//Global things:
const int width = 150;
const int height = 150;
int imageBuffer[width][height];
float zBuffer[width][height];
float IDENTITY[16] = {1,0,0,0,
		      0,1,0,0,
		      0,0,1,0,
		      0,0,0,1};

matrix mainMatrix(IDENTITY);
matrix matrixStack[32];
int mStackPos = 0;

void pushMatrix(){
	if(mStackPos >= 32){
		std::cout << "Matrix stack too deep\n";
		//exit(1);
	}
	matrixStack[mStackPos] = mainMatrix;
	mStackPos++;
}

void popMatrix(){
	if(mStackPos == 0){
		std::cout << "Pop called on empty matrix stack\n";
		//exit(1);
	}
	mainMatrix = matrixStack[mStackPos-1];
	mStackPos--;
}

void scale(float cx,float cy,float cz){
	float svals[16]={cx,0,0,0,
			 0,cy,0,0,
			 0,0,cz,0,
			 0,0,0,1};
	mainMatrix.times(svals);
}

void translate(float cx,float cy,float cz){
	float tvals[16] = {1,0,0,cx,
			   0,1,0,cy,
			   0,0,1,cz,
			   0,0,0,1};
	mainMatrix.times(tvals);
}

void rotate(float angle,bool xr,bool yr,bool zr){
	if(xr){
		float rvalsx[16] = {1,         0,          0,0,
				0,cos(angle),-sin(angle),0,
				0,sin(angle),cos(angle) ,0,
				0,         0,          0,1};
		mainMatrix.times(rvalsx);
	}
	if(yr){
		float rvalsy[16] = {cos(angle),0,sin(angle),0,
				    0,1,0,0,
				    -sin(angle),0,cos(angle),0,
				    0,0,0,1};
		mainMatrix.times(rvalsy);
	}
	if(zr){
		float rvalsz[16] = {cos(angle),-sin(angle),0,0,
				    sin(angle),cos(angle),0,0,
				    0,0,1,0,
				    0,0,0,1};
		mainMatrix.times(rvalsz);
	}
}

void lookAt(float ex,float ey,float ez,float cx,float cy,float cz,float ux,float uy,float uz,float near,float far){
	float left = -5;
	float right = 5;
	float top = -5;
	float bottom = 5;

	point eye(ex,ey,ez);
	point up(ux,uy,uz);
	point center(cx,cy,cz);
	
	point emc = eye - center;

	point w = emc.s_times(1/emc.magnitude());
	point upxw = up.cross(w);	

	point u = upxw.s_times(1/upxw.magnitude()); 
	point v = w.cross(u);

	//First build the p matrix
	matrix P(IDENTITY);
	P.values[0] = (1/far) * (near/((right-left)/2));
	P.values[5] = (1/far) * (near/((top-bottom)/2));
	P.values[10]= 1/(far);
	
	//next build the rotation matrix
	float rvals[16] = {u.x,u.y,u.z,0,
			   v.x,v.y,v.z,0,
			   w.x,w.y,w.z,0,
			   0,  0,  0,  1};
	matrix R(rvals);

	//Now build the translation matrix
	float tvals[16] = {1,0,0,-ex,
			   0,1,0,-ey,
			   0,0,1,-ez,
			   0,0,0,1};

	matrix T(tvals);

	//Bring it all together:
	R.times(T);
	P.times(R);
	mainMatrix.times(P);
}

//Functions:
void drawLine(point p1, point p2){
	int x1 = (int)(((p1.x/p1.z)*width)+(width/2));
	int y1 = (int)(((p1.y/p1.z)*height)+(height/2));
	float z1 = p1.z;

	int x2 = (int)(((p2.x/p2.z)*width)+(width/2));
	int y2 = (int)(((p2.y/p2.z)*height)+(height/2));
 	float z2 = p2.z;

	bool extreme = false;
	
	//If the y change is more than the x change
	if(std::abs(y2 - y1) > std::abs(x2 - x1)){
		extreme = true;
		//If so, change the coordinate system to account for this
		int temp = x1;
		x1 = y1;
		y1 = temp;
		temp = x2;
		x2 = y2;
		y2 = temp;
	}

	//if x2 is greater than x1, switch the order of drawing
	if(x1 > x2){
		int temp = x1;
		x1 = x2;
		x2 = temp;
		temp = y1;
		y1 = y2;
		y2 = temp;
	}


	int dx = x2 - x1;
	int dy = abs(y2 - y1);
	float dz = z2 - z1;	

	int err = (dy << 1) - dx;
	int y = y1;

	//Account for positive slopes
	int ymod = ((y2 < y1) ? (-1) : (1));
	
	//I may need to center this around the origin by adding width/2 or height/2 to indicies of imageBuffer
	for(int x = x1+1; x < x2; x++){
		if(err > 0){
			y += ymod;
			if(y >= 0 && y < height && x >= 0 && x < width){
				(extreme ? imageBuffer[y][x] = 1 : imageBuffer[x][y] = 1);
			}
			//Left shifts are multiplication by two
			err += ((dy << 1) - (dx << 1));
		} else {
			if(y >= 0 && y < height && x >= 0 && x < width){
				(extreme ? imageBuffer[y][x] = 1 : imageBuffer[x][y] = 1);
			}
			err += (dy << 1);
		}
	}	
}

int main(){
	std::ofstream stream;
	stream.open("testimg.ppm");

	//Greater x = Farther Right
	//Greater y = Farther Down
	//Greater z = Farther Back
	
	//for the image buffer, a greater number of the first is farther right. Greater of second is father down.
	for(int i = 0; i < width; i++){
		for(int ii = 0; ii < height; ii++){
			imageBuffer[i][ii] = 0;
		}
	}

	lookAt(3,-3,10,0,0,0,0,1,0,9,100);	
	
	point p1(-1,1,1);
	point p2(1,1,1);
	
	point p3(-1,-1,1);
	point p4(1,-1,1);	

	point p5(-1,1,-1);
	point p6(1,1,-1);

	point p7(-1,-1,-1);
	point p8(1,-1,-1);

//	triangle testtri(p1,p2,p3);

//	drawTriangle(testtri);

	drawLine(p1,p2);		
	drawLine(p3,p4);
	drawLine(p1,p3);
	drawLine(p2,p4);
	drawLine(p5,p6);
	drawLine(p7,p8);
	drawLine(p5,p7);
	drawLine(p6,p8);

	drawLine(p1,p5);
	drawLine(p2,p6);
	drawLine(p3,p7);
	drawLine(p4,p8);


	stream <<  "P1\n" << width << " " << height << "\n";
	for(int y = 0; y < width; y++){
		for(int x = 0; x < width; x++){
			stream << imageBuffer[x][y];
		}
		stream << "\n";
	}

	return 0;
}

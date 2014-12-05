//John Thomason Assignment 4 Main File
#include <fstream>
#include "matrix.h"
#include "commonvars.h"
#include "point.h"
#include "triangle.h"
#include <iostream>
#include <cmath>
#include <vector>

//Global things:
const int width = 500;
const int height = 500;
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
		exit(1);
	}
	matrixStack[mStackPos] = mainMatrix;
	mStackPos++;
}

void popMatrix(){
	if(mStackPos == 0){
		std::cout << "Pop called on empty matrix stack\n";
		exit(1);
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
	float left = -10;
	float right = 10;
	float top = -10;
	float bottom = 10;

	point eye(ex,ey,ez);
	point up(ux,uy,uz);
	point center(cx,cy,cz);
	
	point emc = eye - center;

	point w = emc.s_times(1/emc.magnitude());
	point upxw = up.cross(w);	

	point u = upxw.s_times(1/upxw.magnitude()); 
	point v = w.cross(u);

	//First build the viewport matrix
	float vvals[16] = {((float)width)/2,0,0,((float)width-1)/2,
			   0,((float)height)/2,0,((float)height-1)/2,
			   0,0,1,0,
			   0,0,0,1};

	matrix V(vvals);

	//Build the orthographic/projection matrix
	float opvals[16] = {-(2*near)/(right-left),0,(left+right)/(left-right),0,
			    0,(2*near)/(top-bottom),(bottom+top)/(bottom-top),0,
			    0,0,(far+near)/(near-far),(2*far*near)/(far-near),
			    0,0,1,0};
	matrix OP(opvals);
	
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

	//Bring it all together: (I)VOPRT
	R.times(T);
	OP.times(R);
	V.times(OP);
	mainMatrix.times(V);
}

//Functions:
void drawLine(point p1, point p2){
	//The OP transform lost all Z (depth) information, storing it instead in w);
	int x1 = (int)((p1.x/p1.w));
	int y1 = (int)((p1.y/p1.w));
	float z1 = p1.w;
	int x2 = (int)((p2.x/p2.w));
	int y2 = (int)((p2.y/p2.w));
	float z2 = p2.w;

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
	int err = (dy << 1) - dx;
	int y = y1;

	//Account for positive slopes
	int ymod = ((y2 < y1) ? (-1) : (1));

	float tolerance = .1;
	
	//I may need to center this around the origin by adding width/2 or height/2 to indicies of imageBuffer
	for(int x = x1+1; x < x2; x++){
		float length = pow(pow(x2-x1,2)+pow(y2-y1,2),.5);
		float l1;
		if(extreme){
			l1 = pow(pow(x2-(x+.5),2)+pow(y2-(y+.5),2),.5);
		} else {
			l1 = pow(pow(x1-(x+.5),2)+pow(y1-(y+.5),2),.5);
		}
		float l2 = length - l1;
		float depth = (l1/length)*z1 + (l2/length)*z2;		
		if(err > 0){
			y += ymod;
			if(y >= 0 && y < height && x >= 0 && x < width){
				if(extreme){
					if(zBuffer[y][x] <= depth || (zBuffer[y][x] - depth) < tolerance){
						imageBuffer[y][x] = 1;
						zBuffer[y][x] = depth;
					}
				} else {
					if(zBuffer[x][y] <= depth || (zBuffer[x][y] - depth) < tolerance){
						imageBuffer[x][y] = 1;
						zBuffer[x][y] = depth;
					}
				}
			}
			//Left shifts are multiplication by two
			err += ((dy << 1) - (dx << 1));
		} else {
			if(y >= 0 && y < height && x >= 0 && x < width){
				if(extreme){
					if(zBuffer[y][x] <= depth || (zBuffer[y][x] - depth) < tolerance){
						imageBuffer[y][x] = 1;
						zBuffer[y][x] = depth;
					}
				} else {
					if(zBuffer[x][y] <= depth || (zBuffer[x][y] - depth) < tolerance){
						imageBuffer[x][y] = 1;
						zBuffer[x][y] = depth;
					}
				}	
			}
			err += (dy << 1);
		}
	}
	/*	
	if(y2 >= 0 && y2 < height && x2 >= 0 && x2 < width){
		(extreme ? imageBuffer[y2][x2] = 1 : imageBuffer[x2][y2] = 1);
	}*/
}

float geometricLineCheck(int x1,int y1,int x2,int y2,float testx,float testy){
	return ((y1 - y2)*testx + (x2 - x1)*testy + x1*y2 - x2*y1);
}

void drawTriangle(triangle tri){
	//Pull out the values of the points
	int x1 = (int)((tri.p1.x/tri.p1.w));
	int y1 = (int)((tri.p1.y/tri.p1.w));
 	float z1 = tri.p1.w;
	int x2 = (int)((tri.p2.x/tri.p2.w));
	int y2 = (int)((tri.p2.y/tri.p2.w));
	float z2 = tri.p2.w;
	int x3 = (int)((tri.p3.x/tri.p3.w));
	int y3 = (int)((tri.p3.y/tri.p3.w));
	float z3 = tri.p3.w;

	//CHECK FOR CLIPPING HERE?
	//--do it before orthogonal project probably (8.1.3)
	//CHECK FOR CLIPPING HERE?

	//Find the bounding box of the triangle 
	int minx = std::min(std::min(x1,x2),x3);
	int miny = std::min(std::min(y1,y2),y3);

	int maxx = std::max(std::max(x1,x2),x3);
	int maxy = std::max(std::max(y1,y2),y3);

	//And iterarte over it, filling the depth buffer and clearing the image buffer where necessary:
	for(int y = miny;y <= maxy;y++){
		for(int x = minx;x <= maxx;x++){

			float alpha = geometricLineCheck(x2,y2,x3,y3,(float)x+.5,(float)y+.5)/geometricLineCheck(x2,y2,x3,y3,(float)x1,(float)y1);
			float beta = geometricLineCheck(x3,y3,x1,y1,(float)x+.5,(float)y+.5)/geometricLineCheck(x3,y3,x1,y1,(float)x2,(float)y2);
			float gamma = geometricLineCheck(x1,y1,x2,y2,(float)x+.5,(float)y+.5)/geometricLineCheck(x1,y1,x2,y2,(float)x3,(float)y3);
			
			if( alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && gamma > 0 && gamma < 1){
				if(y >= 0 && y < height && x >= 0 && x < width){
					//Interpolate the depths to get the depth at a point and fill the z buffer
					float depth = alpha*z1 + beta*z2 + gamma*z3;
					if(zBuffer[x][y] < depth){
						zBuffer[x][y] = depth;
						imageBuffer[x][y] = 0;
					}
				}
			}
		}
	}

	//now draw the lines:
	if(tri.drawLine12) drawLine(tri.p1,tri.p2);
	if(tri.drawLine23) drawLine(tri.p2,tri.p3);
	if(tri.drawLine31) drawLine(tri.p3,tri.p1);
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
			zBuffer[i][ii] = -100; //Set z buffer to farther away than actually possible
		}
	}

	lookAt(3,-8,10,0,0,0,0,1,0,9,100);	
	
	point p1(-5,5,2);
	point p2(5,5,2);
	
	point p3(-5,-5,2);
	point p4(5,-5,2);	

	point p5(-5,5,-8);
	point p6(5,5,-8);

	point p7(-5,-5,-8);
	point p8(5,-5,-8);

	//MODIFY THE LOOKAT FUNCTION TO INCLUDE A SCREEN WIDTH

	triangle testtri(p1,p2,p3);
	triangle testtri2(p5,p6,p8);
	triangle testtri3(p5,p7,p1);
	
	drawTriangle(testtri);
	drawTriangle(testtri2);
	drawTriangle(testtri3);
	//drawTriangle(testtri);
	
	/*drawLine(p1,p2);		
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
	drawLine(p4,p8);*/


	stream <<  "P1\n" << width << " " << height << "\n";
	for(int y = 0; y < width; y++){
		for(int x = 0; x < width; x++){
			stream << imageBuffer[x][y];
		}	
		stream << "\n";
	}

	return 0;
}

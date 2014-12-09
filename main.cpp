//John Thomason Assignment 4 Main File
#include <fstream>
#include "matrix.h"
#include "commonvars.h"
#include "point.h"
#include "triangle.h"
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>

//Global things:
const int width = 600;
const int height = 600;
float TAU = 6.283185307;
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

void lookAt(float ex,float ey,float ez,float cx,float cy,float cz,float ux,float uy,float uz,float near,float far,float fov){
	float left = -fov/2;
	float right = fov/2;
	float top = -fov/2;
	float bottom = fov/2;

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
	bool swapped = false;
	
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
		swapped = true;
		int temp = x1;
		x1 = x2;
		x2 = temp;
		temp = y1;
		y1 = y2;
		y2 = temp;
		float tempp = z1;
		z1 = z2;
		z2 = tempp;
	}

	int dx = x2 - x1;
	int dy = abs(y2 - y1);
	int err = (dy << 1) - dx;
	int y = y1;

	//Account for positive slopes
	int ymod = ((y2 < y1) ? (-1) : (1));

	float tolerance = .2;
	
	//I may need to center this around the origin by adding width/2 or height/2 to indicies of imageBuffer
	for(int x = x1+1; x <= x2; x++){
		double length = pow(pow(x2-x1,2)+pow(y2-y1,2),.5);
		double l1;
		double l2;
		double depth;
		if(extreme){
			l1 = pow(pow(x2-(x+.5),2)+pow(y2-(y+.5),2),.5);
			l2 = length - l1;
			depth = (l1/length)*z1 + (l2/length)*z2;		
		} else {
			l1 = pow(pow(x1-(x+.5),2)+pow(y1-(y+.5),2),.5);
			l2 = length - l1;
			depth = (l1/length)*z2 + (l2/length)*z1;		
		}
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

			float tolerance = .3;

			float alpha = geometricLineCheck(x2,y2,x3,y3,(float)x+.5,(float)y+.5)/geometricLineCheck(x2,y2,x3,y3,(float)x1,(float)y1);
			float beta = geometricLineCheck(x3,y3,x1,y1,(float)x+.5,(float)y+.5)/geometricLineCheck(x3,y3,x1,y1,(float)x2,(float)y2);
			float gamma = geometricLineCheck(x1,y1,x2,y2,(float)x+.5,(float)y+.5)/geometricLineCheck(x1,y1,x2,y2,(float)x3,(float)y3);
			
			if( alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && gamma > 0 && gamma < 1){
				if(y >= 0 && y < height && x >= 0 && x < width){
					//Interpolate the depths to get the depth at a point and fill the z buffer
					float depth = alpha*z1 + beta*z2 + gamma*z3;
					if(zBuffer[x][y] <= depth){
						if(depth - zBuffer[x][y] > tolerance){
							imageBuffer[x][y] = 0;
						}
						zBuffer[x][y] = depth;
						/*if(alpha < .1 && tri.drawLine23){
							imageBuffer[x][y] = 1;
						}
						if(beta < .1 && tri.drawLine31){
							imageBuffer[x][y] = 1;
						}
						if(gamma < .1 && tri.drawLine12){
							imageBuffer[x][y] = 1;
						}*/
					}
				}
			}
		}
	}
	if(tri.drawLine12) drawLine(tri.p1,tri.p2);
	if(tri.drawLine23) drawLine(tri.p2,tri.p3);
	if(tri.drawLine31) drawLine(tri.p3,tri.p1);
}

void makeTriangle(point p1,point p2,point p3,bool drawLine12,bool drawLine23,bool drawLine31){
	drawTriangle(triangle (p1,p2,p3,drawLine12,drawLine23,drawLine31));
}

void makeTriangle(point p1,point p2,point p3){
	drawTriangle(triangle (p1,p2,p3));
}

void makeSquare(point p1,point p2,point p3,point p4){
	makeTriangle(p1,p2,p3,true,true,false);
	makeTriangle(p1,p4,p3,true,true,false);
}

void makeSquare(point p1,point p2,point p3,point p4,bool dL12,bool dL23,bool dL34,bool dL41){
	makeTriangle(p1,p2,p3,dL12,dL23,false);
	makeTriangle(p1,p4,p3,dL41,dL34,false);
}

void deepTriangle(point p1,point p2,point p3,float depth,bool dL12,bool dL23,bool dL31){
	point p11(p1.x,p1.y,p1.z);
	point p22(p2.x,p2.y,p2.z);
	point p33(p3.x,p3.y,p3.z);
	makeTriangle(p11,p22,p33,dL12,dL23,dL31);
	point p1d(p1.x,p1.y,p1.z-depth);
	point p2d(p2.x,p2.y,p2.z-depth);
	point p3d(p3.x,p3.y,p3.z-depth);
	makeTriangle(p1d,p2d,p3d,dL12,dL23,dL31);
	makeSquare(p11,p1d,p2d,p22,true,dL12,true,dL12);
	makeSquare(p22,p2d,p3d,p33,true,dL23,true,dL23);
	makeSquare(p33,p3d,p1d,p11,true,dL31,true,dL31);
}

void deepTriangleForShape(point p1,point p2,point p3,float depth,bool dL12,bool dL23,bool dL31){
	point p11(p1.x,p1.y,p1.z);
	point p22(p2.x,p2.y,p2.z);
	point p33(p3.x,p3.y,p3.z);
	makeTriangle(p11,p22,p33,dL12,dL23,dL31);
	point p1d(p1.x,p1.y,p1.z-depth);
	point p2d(p2.x,p2.y,p2.z-depth);
	point p3d(p3.x,p3.y,p3.z-depth);
	makeTriangle(p1d,p2d,p3d,dL12,dL23,dL31);
	makeSquare(p11,p1d,p2d,p22,false,dL12,true,dL12);
}
void deepSquare(point p1,point p2,point p3,point p4,float depth,bool dL12,bool dL23,bool dL34,bool dL41,bool dLeftFace,bool dRightFace){
	point p11(p1.x,p1.y,p1.z);
	point p22(p2.x,p2.y,p2.z);
	point p33(p3.x,p3.y,p3.z);
	point p44(p4.x,p4.y,p4.z);
	makeSquare(p11,p22,p33,p44,dL12,dL23,dL34,dL41);
	point p1d(p1.x,p1.y,p1.z-depth);
	point p2d(p2.x,p2.y,p2.z-depth);
	point p3d(p3.x,p3.y,p3.z-depth);
	point p4d(p4.x,p4.y,p4.z-depth);
	makeSquare(p1d,p2d,p3d,p4d,dL12,dL23,dL34,dL41);
	makeSquare(p11,p1d,p2d,p22,dLeftFace,dL12,dRightFace,dL12);
	makeSquare(p22,p2d,p3d,p33,dRightFace,dL23,dRightFace,dL23);
	makeSquare(p33,p3d,p4d,p44,dRightFace,dL34,dLeftFace,dL34);
	makeSquare(p44,p4d,p1d,p11,dLeftFace,dL41,dLeftFace,dL41);
}

void drawBox(){
for(int i = 0;i < 6;i++){
		pushMatrix();
			if(i < 4){
			rotate((TAU/4)*i,false,true,false);
			} else {
				if(i==4){
					rotate(TAU/4,true,false,false);
				} else {
					rotate(-TAU/4,true,false,false);
				}
			}
			makeSquare(point (-6,-6,6),point (-4,-6,6),point (-4,-4,6),point (-6,-4,6),true,false,false,true);
			makeSquare(point (-4,-6,6),point (4,-6,6),point (4,-4,6),point (-4,-4,6),true,false,true,false);
			makeSquare(point (4,-6,6),point (6,-6,6),point (6,-4,6),point (4,-4,6),true,true,false,false);
			makeSquare(point (4,-4,6),point (6,-4,6), point (6,4,6),point (4,4,6), false,true,false,true);
			makeSquare(point (4,4,6),point (6,4,6),point (6,6,6),point (4,6,6),false,true,true,false);
			makeSquare(point (-4,6,6),point (4,6,6),point (4,4,6),point (-4,4,6),true,false,true,false);
			makeSquare(point (-6,4,6),point (-4,4,6),point (-4,6,6),point (-6,6,6),false,false,true,true);
			makeSquare(point (-4,-4,6),point (-6,-4,6), point (-6,4,6),point (-4,4,6), false,true,false,true);
			for(int ii = 0;ii < 4;ii++){
				pushMatrix();
					rotate((TAU/4)*ii,false,false,true);
					makeSquare(point (-4,-4,6),point (-4,-4,5),point (-4,4,5),point (-4,4,6));
				popMatrix();
			}
			makeSquare(point (-4,-4,5),point (4,-4,5),point (4,4,5),point (-4,4,5));
		popMatrix();
	}	

}

void makeShape(float radius,float numSides,float depth){
	float x1 = radius;
        float y1 = 0;
        float xn = 0;
        float yn = 0;
        float theta = 0;
        for(int i = 1;i <= numSides;i++){
                theta = i*(TAU/numSides);
                xn = radius*cos(theta);
                yn = radius*sin(theta);
                deepTriangleForShape(point (0,0,0,1),point (x1,y1,0,1),point (xn,yn,0,1),1,false,true,false);
                x1=xn;
                y1=yn;
        }

}

void display(int frame){
	//box1
	pushMatrix();
		translate(-7,0,-20);
		rotate(frame*.1,false,true,false);
		drawBox();
		//triangle face front
		pushMatrix();
			translate(0,0,6);
			deepTriangle(point (0,-3,0,1),point (2.5,2,0,1),point (-2.5,2,0,1),1,true,true,true);
		popMatrix();
		//T face right
		pushMatrix();
			translate(6,0,0);
			rotate(TAU/4,false,true,false);
			deepSquare(point (-2.5,-2.5,0,1),point (-.5,-2.5,0,1),point (-.5,-1.5,0,1),point (-2.5,-1.5,0,1),1,true,false,true,true,true,false);
			deepSquare(point (2.5,-2.5,0,1),point (.5,-2.5,0,1),point (.5,-1.5,0,1),point (2.5,-1.5,0,1),1,true,false,true,true,true,false);
			deepSquare(point (-.5,-2.5,0,1),point (.5,-2.5,0,1),point (.5,-1.5,0,1),point (-.5,-1.5,0,1),1,true,false,false,false,false,false);
			deepSquare(point (-.5,-1.5,0,1),point (.5,-1.5,0,1),point (.5,2.5,0,1),point (-.5,2.5,0,1),1,false,true,true,true,true,true);
		popMatrix();
		//+ face left
		pushMatrix();
			translate(-6,0,0);
			rotate(-TAU/4,false,true,false);
			deepSquare(point (-2.5,-.5,0,1),point (-.5,-.5,0,1),point (-.5,.5,0,1),point(-2.5,.5,0,1),1,true,false,true,true,true,false);
			deepSquare(point (2.5,-.5,0,1),point (.5,-.5,0,1),point (.5,.5,0,1),point(2.5,.5,0,1),1,true,false,true,true,true,false);
			deepSquare(point (-.5,-2.5,0,1),point (.5,-2.5,0,1),point (.5,-.5,0,1),point (-.5,-.5,0,1),1,true,true,false,true,true,true);
			deepSquare(point (-.5,2.5,0,1),point (.5,2.5,0,1),point (.5,.5,0,1),point (-.5,.5,0,1),1,true,true,false,true,true,true);
			makeSquare(point (-.5,-.5,0),point (.5,-.5,0),point (.5,.5,0),point (-.5,.5,0),false,false,false,false);
		popMatrix();
		//diamond face back
		pushMatrix();
			translate(0,0,-6);
			rotate(TAU/2,false,true,false);
			deepTriangle(point (0,-2.5,0,1),point (2,0,0,1),point (-2,0,0,1),1,true,false,true);
			deepTriangle(point (0,2.5,0,1),point (2,0,0,1),point (-2,0,0,1),1,true,false,true);
		popMatrix();
		//dice 1 face top
		pushMatrix();
			translate(0,-6,0);
			rotate(TAU/4,true,false,false);
			makeShape(1.25,20,1);
		popMatrix();
		// / face bottom
		pushMatrix();
			translate(0,6,0);
			rotate(-TAU/4,true,false,false);
			deepSquare(point (2,-2.5,0,1),point (2.5,-2.5,0,1),point (-2,2.5,0,1),point (-2.5,2.5,0,1),1,true,true,true,true,true,true);
		popMatrix();
	popMatrix();

	//Box2
	pushMatrix();
		translate(0,0,-20);
		rotate(1.56,false,true,false);
		rotate((frame*1.3)*.1,false,true,false);
		drawBox();
		//Square face front
		pushMatrix();
			translate(0,0,6);
			deepSquare(point (-2.5,-2.5,0,1),point (2.5,-2.5,0,1),point (2.5,2.5,0,1),point (-2.5,2.5,0,1),1,true,true,true,true,true,true);
		popMatrix();
		//E face right
		pushMatrix();
			translate(6,0,0);
			rotate(TAU/4,false,true,false);
			deepSquare(point (-2.5,-2.5,0,1),point (-2,-2.5,0,1),point (-2,-2,0,1),point (-2.5,-2,0,1),1,true,false,false,true,false,false);
			deepSquare(point (-2,-2.5,0,1),point (2.5,-2.5,0,1),point (2.5,-2,0,1),point (-2,-2,0,1),1,true,true,true,false,false,true);
			deepSquare(point (-2.5,2.5,0,1),point (-2,2.5,0,1),point (-2,2,0,1),point (-2.5,2,0,1),1,true,false,false,true,false,false);
			deepSquare(point (-2,2.5,0,1),point (2.5,2.5,0,1),point (2.5,2,0,1),point (-2,2,0,1),1,true,true,true,false,false,true);
			deepSquare(point (-2,-.25,0,1),point (2.5,-.25,0,1),point (2.5,.25,0,1),point (-2,.25,0,1),1,true,true,true,false,false,true);
			deepSquare(point (-2.5,-.25,0,1),point (-2,-.25,0,1),point (-2,.25,0,1),point (-2.5,.25,0,1),1,false,false,false,true,false,false);
			deepSquare(point (-2.5,-2,0,1),point (-2,-2,0,1),point (-2,-.25,0,1),point (-2.5,-.25,0,1),1,false,true,false,true,false,true);
			deepSquare(point (-2.5,2,0,1),point (-2,2,0,1),point (-2,.25,0,1),point (-2.5,.25,0,1),1,false,true,false,true,false,true);
			makeSquare(point (-2.5,-2.5,0),point (-2.5,-2.5,-1),point (-2.5,2.5,-1),point (-2.5,2.5,0),true,true,true,true);
		popMatrix();
		//- face left
		pushMatrix();
			translate(-6,0,0);
			rotate(-TAU/4,false,true,false);
			deepSquare(point (-2.5,-.5,0,1),point (2.5,-.5,0,1),point (2.5,.5,0,1),point (-2.5,.5,0,1),1,true,true,true,true,true,true);
		popMatrix();
		//trapezoid face back
		pushMatrix();
			translate(0,0,-6);
			rotate(TAU/2,false,true,false);
			deepTriangle(point (-2.5,1.5,0,1),point (-1,-2.5,0,1),point (-1,1.5,0,1),1,true,false,true);
			deepTriangle(point (2.5,1.5,0,1),point (1,-2.5,0,1),point (1,1.5,0,1),1,true,false,true);
			deepSquare(point (-1,-2.5,0,1),point (1,-2.5,0,1),point (1,1.5,0,1),point (-1,1.5,0,1),1,true,false,true,false,false,false);
		popMatrix();
		//dice 2 face top
		pushMatrix();
			translate(0,-6,0);
			rotate(TAU/4,true,false,false);
			pushMatrix();
				translate(1.5,-1.5,0);
				makeShape(1.25,20,1);
			popMatrix();
			pushMatrix();
				translate(-1.5,1.5,0);
				makeShape(1.25,20,1);
			popMatrix();
		popMatrix();
		//hourglass face bottom
		pushMatrix();
			translate(0,6,0);
			rotate(-TAU/4,true,false,false);
			deepTriangle(point (-2.5,-2.5,0,1),point (2.5,-2.5,0,1),point (0,0,0,1),1,true,true,true);
			deepTriangle(point (-2.5,2.5,0,1),point (2.5,2.5,0,1),point (0,0,0,1),1,true,true,true);
		popMatrix();
	popMatrix();
	
	//Box3
	pushMatrix();
		translate(0,0,-30);
		rotate(-.5,false,true,false);
		rotate((frame*-1)*.1,false,true,false);
		drawBox();
		//Pentagon face front
		pushMatrix();
			translate(0,0,6);
			makeShape(2.5,5,1);
		popMatrix();
		//F face right
		pushMatrix();
			translate(6,0,0);
			rotate(TAU/4,false,true,false);
			deepSquare(point (-2.5,-2.5,0,1),point (-2,-2.5,0,1),point (-2,-2,0,1),point (-2.5,-2,0,1),1,true,false,false,true,false,false);
			deepSquare(point (-2,-2.5,0,1),point (2.5,-2.5,0,1),point (2.5,-2,0,1),point (-2,-2,0,1),1,true,true,true,false,false,true);
			deepSquare(point (-2,-.25,0,1),point (2.5,-.25,0,1),point (2.5,.25,0,1),point (-2,.25,0,1),1,true,true,true,false,false,true);
			deepSquare(point (-2.5,-.25,0,1),point (-2,-.25,0,1),point (-2,.25,0,1),point (-2.5,.25,0,1),1,false,false,false,true,false,false);
			deepSquare(point (-2.5,-2,0,1),point (-2,-2,0,1),point (-2,-.25,0,1),point (-2.5,-.25,0,1),1,false,true,false,true,false,true);
			deepSquare(point (-2.5,2.5,0,1),point (-2,2.5,0,1),point (-2,.25,0,1),point (-2.5,.25,0,1),1,true,true,false,true,true,true);
			makeSquare(point (-2.5,-2.5,0),point (-2.5,-2.5,-1),point (-2.5,2.5,-1),point (-2.5,2.5,0),true,true,true,true);
	
		popMatrix();
		//x face left
		pushMatrix();
			translate(-6,0,0);
			rotate(-TAU/4,false,true,false);
			rotate(TAU/8,false,false,true);
			deepSquare(point (-3.5,-.5,0,1),point (-.5,-.5,0,1),point (-.5,.5,0,1),point(-3.5,.5,0,1),1,true,false,true,true,true,false);
			deepSquare(point (3.5,-.5,0,1),point (.5,-.5,0,1),point (.5,.5,0,1),point(3.5,.5,0,1),1,true,false,true,true,true,false);
			deepSquare(point (-.5,-3.5,0,1),point (.5,-3.5,0,1),point (.5,-.5,0,1),point (-.5,-.5,0,1),1,true,true,false,true,true,true);
			deepSquare(point (-.5,3.5,0,1),point (.5,3.5,0,1),point (.5,.5,0,1),point (-.5,.5,0,1),1,true,true,false,true,true,true);
			makeSquare(point (-.5,-.5,0),point (.5,-.5,0),point (.5,.5,0),point (-.5,.5,0),false,false,false,false);
		popMatrix();
		//stary thing face back
		pushMatrix();
			translate(0,0,-6);
			rotate(TAU/2,false,true,false);
			deepTriangle(point (-.5,-.5,0,1),point (0,-2.5,0,1),point (.5,-.5,0,1),1,true,true,false);
			deepTriangle(point (-.5,.5,0,1),point (0,2.5,0,1),point (.5,.5,0,1),1,true,true,false);
			deepTriangle(point (-2.5,0,0,1),point (-.5,-.5,0,1),point (-.5,.5,0,1),1,true,false,true);	
			deepTriangle(point (2.5,0,0,1),point (.5,-.5,0,1),point (.5,.5,0,1),1,true,false,true);	
			makeSquare(point (-.5,-.5,0),point (.5,-.5,0),point (.5,.5,0),point (-.5,.5,0),false,false,false,false);
		popMatrix();
		//dice 3 face top
		pushMatrix();
			translate(0,-6,0);
			rotate(TAU/4,true,false,false);
			pushMatrix();
				translate(1.9,-1.9,0);
				makeShape(1.25,20,1);
			popMatrix();
			pushMatrix();
				translate(-1.9,1.9,0);
				makeShape(1.25,20,1);
			popMatrix();
			makeShape(1.25,20,1);

		popMatrix();
		//i face bottom
		pushMatrix();
			translate(0,6,0);
			rotate(-TAU/4,true,false,false);
			deepSquare(point (-1,0,0,1),point (1,0,0,1),point (1,2.5,0,1),point (-1,2.5,0,1),1,true,true,true,true,true,true);
			translate(0,-2,0);
			makeShape(1,20,1);
		popMatrix();
	popMatrix();

	//Box4
	pushMatrix();
		translate(0,0,0);
		rotate(-2,false,true,false);
		rotate((frame*.7)*.1,false,true,false);
		drawBox();
		//Hexagon face front
		pushMatrix();
			translate(0,0,6);
			makeShape(2.5,6,1);
		popMatrix();
		//H face right
		pushMatrix();
			translate(6,0,0);
			rotate(TAU/4,false,true,false);
			deepSquare(point (-2.5,-2.5,0,1),point (-2,-2.5,0,1),point (-2,-.5,0,1),point (-2.5,-.5,0,1),1,true,true,false,true,false,true);			
			deepSquare(point (-2.5,2.5,0,1),point (-2,2.5,0,1),point (-2,.5,0,1),point (-2.5,.5,0,1),1,true,true,false,true,false,true);			
			deepSquare(point (-2.5,-.5,0,1),point (-2,-.5,0,1),point (-2,.5,0,1),point (-2.5,.5,0,1),1,false,false,false,true,false,false);
			makeSquare(point (-2.5,-2.5,0),point (-2.5,-2.5,-1),point (-2.5,2.5,-1),point (-2.5,2.5,0),true,true,true,true);
			deepSquare(point (2.5,-2.5,0,1),point (2,-2.5,0,1),point (2,-.5,0,1),point (2.5,-.5,0,1),1,true,true,false,true,false,true);			
			deepSquare(point (2.5,2.5,0,1),point (2,2.5,0,1),point (2,.5,0,1),point (2.5,.5,0,1),1,true,true,false,true,false,true);			
			deepSquare(point (2.5,-.5,0,1),point (2,-.5,0,1),point (2,.5,0,1),point (2.5,.5,0,1),1,false,false,false,true,false,false);
			makeSquare(point (2.5,-2.5,0),point (2.5,-2.5,-1),point (2.5,2.5,-1),point (2.5,2.5,0),true,true,true,true);
			deepSquare(point (-2,-.5,0,1),point (2,-.5,0,1),point (2,.5,0,1),point (-2,.5,0,1),1,true,false,true,false,false,false);
		popMatrix();
		//% face left
		pushMatrix();
			translate(-6,0,0);
			rotate(-TAU/4,false,true,false);
			deepSquare(point (-2.5,-.5,0,1),point (2.5,-.5,0,1),point (2.5,.5,0,1),point(-2.5,.5,0,1),1,true,true,true,true,true,true);
			pushMatrix();
				translate(0,-2,0);
				makeShape(1,15,1);
			popMatrix();
			pushMatrix();
				translate(0,2,0);
				makeShape(1,15,1);
			popMatrix();
		popMatrix();
		//V face back
		pushMatrix();
			translate(0,0,-6);
			rotate(TAU/2,false,true,false);
			deepSquare(point (-2.5,-2.5,0,1),point (-1.5,-2.5,0,1),point (0,1,0,1),point (0,2.5,0,1),1,true,true,false,true,true,true);
			deepSquare(point (2.5,-2.5,0,1),point (1.5,-2.5,0,1),point (0,1,0,1),point (0,2.5,0,1),1,true,true,false,true,true,true);
		popMatrix();
		//dice 4 face top
		pushMatrix();
			translate(0,-6,0);
			rotate(TAU/4,true,false,false);
			pushMatrix();
				translate(1.9,-1.9,0);
				makeShape(1.25,20,1);
			popMatrix();
			pushMatrix();
				translate(-1.9,1.9,0);
				makeShape(1.25,20,1);
			popMatrix();
			pushMatrix();
				translate(-1.9,-1.9,0);
				makeShape(1.25,20,1);
			popMatrix();
			pushMatrix();
				translate(1.9,1.9,0);
				makeShape(1.25,20,1);
			popMatrix();
		popMatrix();
		//circle face bottom
		pushMatrix();
			translate(0,6,0);
			rotate(-TAU/4,true,false,false);
			makeShape(2.5,25,1);
		popMatrix();
	popMatrix();


}

int main(){

	//Greater x = Farther Right
	//Greater y = Farther Down
	//Greater z = Farther Back
	
	//for the image buffer, a greater number of the first is farther right. Greater of second is father down.
	for(int i = 100;i < 201;i++){
		std::ofstream stream;
		std::ostringstream fname;
		fname << "img" << i << ".ppm";
		stream.open(fname.str());
		for(int r = 0; r < width; r++){
			for(int ii = 0; ii < height; ii++){
				imageBuffer[r][ii] = 0;
				zBuffer[r][ii] = -100; //Set z buffer to farther away than actually possible
			}
		}
	
		mainMatrix = IDENTITY;	

		lookAt(-15,-5,20,0,0,0,0,1,0,10,100,20);	
		display(i);
	
		stream <<  "P1\n" << width << " " << height << "\n";
		for(int y = 0; y < width; y++){
			for(int x = 0; x < width; x++){
				stream << imageBuffer[x][y];
			}	
			stream << "\n";
		}	

	}

	return 0;
}

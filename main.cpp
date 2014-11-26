//John Thomason Assignment 4 Main File
#include <fstream>
#include "matrix.h"
#include "point.h"
#include <iostream>

//Global things:
const int width = 500;
const int height = 500;
int imageBuffer[width][height];
float zBuffer[width][height];

//Functions:
void drawLine(point p1, point p2){
	//WARNING! NO ACCOUNTING FOR OUTSIDE IMAGE BUFFER YET!
	int x1 = (int)p1.x;
	int y1 = (int)p1.y;
	float z1 = p1.z;
	
	int x2 = (int)p2.x;
	int y2 = (int)p2.y;
 	float z2 = p2.z;

	int dx = x2 - x1;
	int dy = y2 - y1;
	float dz = z2 - z1;	

	int err = (dy << 1) - dx;
	int y = y1;

	//I may need to center this around the origin by adding width/2 or height/2 to indicies of imageBuffer
	for(int i = x1+1; i < x2; i++){
		if(err > 0){
			y++;
			//DANGER ZONE
			imageBuffer[i][y] = 1;
			//DANGER ZONE
			//Left shifts are multiplication by two
			err += ((dy << 1) - (dx << 1));
		} else {
			//DANGER ZONE
			imageBuffer[i][y] = 1;
			//DANGER ZONE
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

	float val[16] = {0,0,0,0,
			 1,1,1,0,
			 0,0,0,1,
			 0,1,0,0};

	float val2[16] = {0,0,1,0,
			  1,0,0,0,
			  0,0,1,0,
			  0,1,0,0};

	matrix testmat(val);
	matrix testmat2(val2);
	matrix copytest(testmat);
	testmat.times(testmat2);
	point testpoint(1,1,1);	
	testpoint = testpoint.times(copytest);

	point p1 (10,10,10);
	point p2 (20,25,10);

	drawLine(p1,p2);

	stream <<  "P1\n" << width << " " << height << "\n";
	for(int y = 0; y < width; y++){
		for(int x = 0; x < width; x++){
			stream << imageBuffer[x][y];
		}
		stream << "\n";
	}

	
	return 0;
}

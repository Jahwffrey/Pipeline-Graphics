//John Thomason Assignment 4 Main File
#include <fstream>
#include "matrix.h"
#include "point.h"
#include <iostream>
//#include "point.h"

int main(){
	std::ofstream stream;
	stream.open("testimg.ppm");
	
	int width = 4;
	int height = 4;

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
	std::cout << testpoint.x << " " << testpoint.y << " " << testpoint.z << " " << testpoint.w;

	int num = 0;
	
	stream <<  "P1\n" << width << " " << height << "\n";
	for(int y = 0; y < width; y++){
		for(int x = 0; x < width; x++){
			stream << testmat.values[num] ;
			num+=1;
		}
		stream << "\n";
	}

	
	return 0;
}

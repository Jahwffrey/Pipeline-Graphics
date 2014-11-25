//John Thomason Assignment 4 Main File
#include <fstream>
#include "matrix.h"

int main(){
	std::ofstream stream;
	stream.open("testimg.ppm");
	
	int width = 255;
	int height = 255;

	stream <<  "P1\n" << width << " " << height << "\n255\n";
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			stream << "0 ";
		}
		stream << "\n";
	}

	matrix testmat;
	
	return 0;
}

all: prog

prog: main.cpp
	g++ -o prog4 main.cpp -std=c++11
	echo "****Programming Assignment 4 for CS180 by John Thomason****"
	./prog4
	display testimg.ppm
	
clean:
	rm -rf *.o prog4

CCC = g++

all: prog

prog: main.o matrix.o
	${CCC} main.o matrix.o -o prog4 -std=c++11
	echo "****Programming Assignment 4 for CS180 by John Thomason****"
	./prog4
	display testimg.ppm

main.o: main.cpp matrix.h
	${CCC} -c main.cpp -std=c++11
	

matirx.o: matrix.cpp matrix.h
	${CCC} -c matrix.cpp -std=c++11
	
clean:
	rm -rf *.o prog4

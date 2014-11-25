CCC = g++

all: prog

prog: main.o matrix.o point.o
	${CCC} main.o matrix.o point.o -o prog4
	echo "****Programming Assignment 4 for CS180 by John Thomason****"
	./prog4
	display testimg.ppm

main.o: main.cpp matrix.h point.h
	${CCC} -c -std=c++11 main.cpp
	
point.o: point.cpp point.h matrix.h
	${CCC} -c -std=c++11 point.cpp

matirx.o: matrix.cpp matrix.h
	${CCC} -c -std=c++11 matrix.cpp
	
clean:
	rm -rf *.o prog4

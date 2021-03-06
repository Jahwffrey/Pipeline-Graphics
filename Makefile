CCC = g++

all: prog

prog: main.o matrix.o point.o triangle.o
	${CCC} main.o matrix.o point.o triangle.o -o prog4
	echo "****Programming Assignment 4 for CS180 by John Thomason. Final Animation is anim.gif****"
	./prog4
	convert -delay 5 -loop 0 img*.ppm anim.gif

main.o: main.cpp matrix.h point.h commonvars.h triangle.h
	${CCC} -c -std=c++11 main.cpp

triangle.o: triangle.cpp triangle.h point.h commonvars.h
	${CCC} -c -std=c++11 triangle.cpp
	
point.o: point.cpp point.h matrix.h commonvars.h
	${CCC} -c -std=c++11 point.cpp

matirx.o: matrix.cpp matrix.h
	${CCC} -c -std=c++11 matrix.cpp
	
clean:
	rm -rf *.o prog4
	rm -rf img*

#include "matrix.h"
#include <iostream>
//im going to represent my matrices as such:
//	{1,0,0,0
//	,0,1,0,0
//	,0,0,1,0
//	,0,0,0,1}

/*
 class matrix{
	float values[16];
	}
*/

matrix::matrix(){
	for(int i = 0; i < 16; i++){
		values[i] = 0;
	}
}

matrix::matrix(float* set_values){
	for(int i = 0; i < 16; i++){
		values[i] = set_values[i];
	}
}

matrix::matrix(const matrix& copy){
	for(int i = 0; i < 16; i++){
		values[i] = copy.values[i];
	}
}

void matrix::times(matrix matt){
	float temps[16];
	for(int i = 0;i < 16; i++){
		temps[i] = values[i];
	}
	float* mat = matt.values;
	
	values[0] = mat[0]*temps[0] + mat[4]*temps[1] + mat[8]*temps[2] + mat[12]*temps[3];	
	values[1] = mat[1]*temps[0] + mat[5]*temps[1] + mat[9]*temps[2] + mat[13]*temps[3];
	values[2] = mat[2]*temps[0] + mat[6]*temps[1] + mat[10]*temps[2] + mat[14]*temps[3];
	values[3] = mat[3]*temps[0] + mat[7]*temps[1] + mat[11]*temps[2] + mat[15]*temps[3];

	values[4] = mat[0]*temps[4] + mat[4]*temps[5] + mat[8]*temps[6] + mat[12]*temps[7];	
	values[5] = mat[1]*temps[4] + mat[5]*temps[5] + mat[9]*temps[6] + mat[13]*temps[7];
	values[6] = mat[2]*temps[4] + mat[6]*temps[5] + mat[10]*temps[6] + mat[14]*temps[7];
	values[7] = mat[3]*temps[4] + mat[7]*temps[5] + mat[11]*temps[6] + mat[15]*temps[7];
	
	values[8] = mat[0]*temps[8] + mat[4]*temps[9] + mat[8]*temps[10] + mat[12]*temps[11];	
	values[9] = mat[1]*temps[8] + mat[5]*temps[9] + mat[9]*temps[10] + mat[13]*temps[11];
	values[10] = mat[2]*temps[8] + mat[6]*temps[9] + mat[10]*temps[10] + mat[14]*temps[11];
	values[11] = mat[3]*temps[8] + mat[7]*temps[9] + mat[11]*temps[10] + mat[15]*temps[11];

	values[12] = mat[0]*temps[12] + mat[4]*temps[13] + mat[8]*temps[14] + mat[12]*temps[15];	
	values[13] = mat[1]*temps[12] + mat[5]*temps[13] + mat[9]*temps[14] + mat[13]*temps[15];
	values[14] = mat[2]*temps[12] + mat[6]*temps[13] + mat[10]*temps[14] + mat[14]*temps[15];
	values[15] = mat[3]*temps[12] + mat[7]*temps[13] + mat[11]*temps[14] + mat[15]*temps[15];
}

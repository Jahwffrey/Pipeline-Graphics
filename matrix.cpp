#include "matrix.h"

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
	values = {0};
}

matrix::matrix(float* set_values){
	//array<float,16> temps = set_values?
	values = {0};
	for(int i = 0; i < 16; i++){
		values[i] = set_values[i];
	}
}

void matrix::times(matrix matt){
	float* temps = values;
	float* mat = matt.values;
	
	values[0] = temps[0]*mat[0] + temps[4]*mat[1] + temps[8]*mat[2] + temps[12]*mat[3];	
	values[1] = temps[1]*mat[0] + temps[5]*mat[1] + temps[9]*mat[2] + temps[13]*mat[3];
	values[2] = temps[2]*mat[0] + temps[6]*mat[1] + temps[10]*mat[2] + temps[14]*mat[3];
	values[3] = temps[3]*mat[0] + temps[7]*mat[1] + temps[11]*mat[2] + temps[15]*mat[3];

	values[4] = temps[0]*mat[4] + temps[4]*mat[5] + temps[8]*mat[6] + temps[12]*mat[7];	
	values[5] = temps[1]*mat[4] + temps[5]*mat[5] + temps[9]*mat[6] + temps[13]*mat[7];
	values[6] = temps[2]*mat[4] + temps[6]*mat[5] + temps[10]*mat[6] + temps[14]*mat[7];
	values[7] = temps[3]*mat[4] + temps[7]*mat[5] + temps[11]*mat[6] + temps[15]*mat[7];
	
	values[8] = temps[0]*mat[8] + temps[4]*mat[9] + temps[8]*mat[10] + temps[12]*mat[11];	
	values[9] = temps[1]*mat[8] + temps[5]*mat[9] + temps[9]*mat[10] + temps[13]*mat[11];
	values[10] = temps[2]*mat[8] + temps[6]*mat[9] + temps[10]*mat[10] + temps[14]*mat[11];
	values[11] = temps[3]*mat[8] + temps[7]*mat[9] + temps[11]*mat[10] + temps[15]*mat[11];

	values[12] = temps[0]*mat[12] + temps[4]*mat[13] + temps[8]*mat[14] + temps[12]*mat[15];	
	values[13] = temps[1]*mat[12] + temps[5]*mat[13] + temps[9]*mat[14] + temps[13]*mat[15];
	values[14] = temps[2]*mat[12] + temps[6]*mat[13] + temps[10]*mat[14] + temps[14]*mat[15];
	values[15] = temps[3]*mat[12] + temps[7]*mat[13] + temps[11]*mat[14] + temps[15]*mat[15];
}

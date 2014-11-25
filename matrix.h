#ifndef _MATRIX_H
#define _MATRIX_H
class matrix{
public:
	float* values;
	
	matrix();
	matrix(float* set_values);
	void times(matrix mat);
};
#endif

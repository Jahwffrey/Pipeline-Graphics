#ifndef _MATRIX_H
#define _MATRIX_H
class matrix{
public:
	float values[16];
	matrix();
	matrix(float* set_values);
	void times(matrix matt);
	float* get_values();
};
#endif

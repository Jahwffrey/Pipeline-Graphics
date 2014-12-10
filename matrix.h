#ifndef _MATRIX_H
#define _MATRIX_H
class matrix{
public:
	double values[16];
	matrix();
	matrix(double* set_values);
	matrix(const matrix& copy);
	void times(matrix matt);
	double* get_values();
	
	matrix operator=(const matrix& r){
		for(int i = 0;i < 16;i++){
			values[i] = r.values[i];
		}
	}
};
#endif

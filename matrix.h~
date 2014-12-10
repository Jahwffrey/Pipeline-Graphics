#ifndef _MATRIX_H
#define _MATRIX_H
class matrix{
public:
	float values[16];
	matrix();
	matrix(float* set_values);
	matrix(const matrix& copy);
	void times(matrix matt);
	float* get_values();
	
	matrix operator=(const matrix& r){
		for(int i = 0;i < 16;i++){
			values[i] = r.values[i];
		}
	}
};
#endif

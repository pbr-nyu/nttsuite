#ifndef NTT_H_
#define NTT_H_

#include "config.h"
#include "utils.h"
//#include <cmath>

/**
 * perfrom the operation (x * y) (mod m)
 *
 * @param x	1st operator
 * @param y	2nd operator
 * @param y_	helper operator
 * @param p	modulo
 * @return 	the result of the expression
 */
DATA_TYPE mult(DATA_TYPE x, DATA_TYPE y, DATA_TYPE y_, DATA_TYPE p);

void hybrid(DATA_TYPE x[VECTOR_SIZE], DATA_TYPE m, DATA_TYPE twiddle[HALF_VECTOR_SIZE], DATA_TYPE twiddle_h[VECTOR_SIZE], PARAMS_TYPE revArr[HALF_VECTOR_SIZE],  DATA_TYPE tw[VECTOR_SIZE], DATA_TYPE tw_h[VECTOR_SIZE]);
#endif /* NTT_H_ */
#ifndef NTT_H_
#define NTT_H_

#include "config.h"
#include "ac_sync.h"
//#include "utils.h"
//#include <cmath>

void peaseNTT(ac_sync & run, DATA_TYPE xt[VECTOR_SIZE], DATA_TYPE p, DATA_TYPE r, 
	DATA_TYPE twiddle[VECTOR_SIZE], DATA_TYPE twiddle_h[VECTOR_SIZE], ac_sync & complete);
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
#endif /* NTT_H_ */
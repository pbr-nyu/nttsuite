#ifndef NTT_H_
#define NTT_H_

#include "config.h"
#include "utils.h"
//#include <cmath>

/**
 * Perform the operation 'base^exp (mod m)' using the memory-efficient method
 *
 * @param base	The base of the expression
 * @param exp	The exponent of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
/*
inline DATA_TYPE modulo_sub(DATA_TYPE_SIGNED base, DATA_TYPE m){
	return base < 0? base + m : base;
}
*/
DATA_TYPE modulo_add(DATA_TYPE_SIGNED base, DATA_TYPE_SIGNED m);

/**
 * Perform the operation 'base^exp (mod m)' using the memory-efficient method
 *
 * @param base	The base of the expression
 * @param exp	The exponent of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
/*
inline DATA_TYPE modulo_sub(DATA_TYPE_SIGNED base, DATA_TYPE m){
	return base < 0? base + m : base;
}
*/
DATA_TYPE modulo_sub(DATA_TYPE_SIGNED base, DATA_TYPE_SIGNED m);

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

/**
 * Perform the butterfly operation
 *
 * @param vec	Array
 * @param idx1	The first index
 * @param idx2	The second index
 * @param tw	twiddle factor
 * @param tw_h	twiddle factor helper
 * @param p	The modulus of the expression
 * @return 	The result of the expression
 */
void butterFly(DATA_TYPE *vec, PARAMS_TYPE idx1, PARAMS_TYPE idx2, DATA_TYPE tw, DATA_TYPE tw_h, DATA_TYPE p);

void six_step(DATA_TYPE x[VECTOR_SIZE], DATA_TYPE m, DATA_TYPE twiddle[HALF_VECTOR_SIZE], DATA_TYPE twiddle_h[VECTOR_SIZE], PARAMS_TYPE revArr[HALF_VECTOR_SIZE],  DATA_TYPE tw[VECTOR_SIZE], DATA_TYPE tw_h[VECTOR_SIZE]);
#endif /* NTT_H_ */
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
inline DATA_TYPE modulo_add(DATA_TYPE_SIGNED base, DATA_TYPE m);

/**
 * Perform the operation 'base^exp (mod m)' using the memory-efficient method
 *
 * @param base	The base of the expression
 * @param exp	The exponent of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
inline DATA_TYPE modulo_sub(DATA_TYPE_SIGNED base, DATA_TYPE m);

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

void inPlaceNTT_DIT(DATA_TYPE vec[VECTOR_SIZE], DATA_TYPE p, DATA_TYPE r, DATA_TYPE twiddle[VECTOR_SIZE], DATA_TYPE twiddle_h[VECTOR_SIZE]);
#endif /* NTT_H_ */
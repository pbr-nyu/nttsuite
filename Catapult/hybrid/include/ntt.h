#ifndef NTT_H_
#define NTT_H_

#include "config.h"
#include <math/mgc_ac_math.h>
//#include <cmath>
/**
 * Perform the operation 'base^exp (mod m)' using the memory-efficient method
 *
 * @param base	The base of the expression
 * @param exp	The exponent of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
UINT64_T modExp(UINT64_T base, UINT64_T exp, UINT64_T m);

void six_step(VEC_T x[VECTOR_SIZE], UINT64_T m, UINT64_T g, VEC_T twiddle[HALF_VECTOR_SIZE], unsigned revArr[HALF_VECTOR_SIZE],  VEC_T tw[VECTOR_SIZE]);

#endif /* NTT_H_ */
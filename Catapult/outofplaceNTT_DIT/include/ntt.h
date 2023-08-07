#ifndef NTT_H_
#define NTT_H_

#include "config.h"
//#include <math/mgc_ac_math.h>
//#include <ac_math.h>
#include <cmath>
/**
 * Perform the operation 'base^exp (mod m)' using the memory-efficient method
 *
 * @param base	The base of the expression
 * @param exp	The exponent of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
UINT64_T modExp(UINT64_T base, UINT64_T exp, UINT64_T m);

void inPlaceNTT_DIF(UINT64_T vec[VECTOR_SIZE], UINT64_T p, UINT64_T r);

#endif /* NTT_H_ */
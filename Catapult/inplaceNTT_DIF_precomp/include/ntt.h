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
DATA_TYPE modulo(DATA_TYPE_SIGNED base, DATA_TYPE_SIGNED m);
 
DATA_TYPE modExp(DATA_TYPE base, DATA_TYPE exp, DATA_TYPE m);

void inPlaceNTT_DIF(DATA_TYPE vec[VECTOR_SIZE], DATA_TYPE p, DATA_TYPE r, DATA_TYPE twiddle[VECTOR_SIZE], DATA_TYPE twiddle_h[VECTOR_SIZE]);
#endif /* NTT_H_ */
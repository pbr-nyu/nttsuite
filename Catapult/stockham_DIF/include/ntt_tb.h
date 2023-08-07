#include <cmath>		/* log2(), pow() */
//#include <cstdint>		/* uint64_t */
#include <cstdlib> 		/* malloc() */

#include <mc_scverify.h>
#include <iostream>

#include "config.h"
/**
 * Perform the operation 'base^exp (mod m)' using the memory-efficient method
 *
 * @param base	The base of the expression
 * @param exp	The exponent of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
UINT64_T modExp_golden(UINT64_T base, UINT64_T exp, UINT64_T m);
/**
 * Perform the operation 'base (mod m)'
 *
 * @param base	The base of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
UINT64_T modulo_golden(int64_t base, int64_t m);
/**
 * Perform an in-place iterative breadth-first decimation-in-frequency Cooley-Tukey NTT on an input vector and return the result
 *
 * @param vec 	The input vector to be transformed
 * @param n	The size of the input vector
 * @param p	The prime to be used as the modulus of the transformation
 * @param r	The primitive root of the prime
 * @param rev	Whether to perform bit reversal on the output vector
 * @return 	The transformed vector
 */
UINT64_T *inPlaceNTT_DIF_golden(UINT64_T *vec, UINT64_T n, UINT64_T p, UINT64_T r);

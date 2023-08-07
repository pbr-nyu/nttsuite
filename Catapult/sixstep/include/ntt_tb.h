#include <cmath>		/* log2(), pow() */
#include <cstdint>		/* uint64_t */
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
uint64_t modExp(uint64_t base, uint64_t exp, uint64_t m);
/**
 * Perform the operation 'base (mod m)'
 *
 * @param base	The base of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
uint64_t modulo(int64_t base, int64_t m);
void six_step(VEC_T x[VECTOR_SIZE], UINT64_T m, UINT64_T g, VEC_T twiddle[HALF_VECTOR_SIZE], unsigned revArr[HALF_VECTOR_SIZE]);

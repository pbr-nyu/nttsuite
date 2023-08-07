#include <cmath>		/* log2(), pow() */
#include <cstdint>		/* uint64_t */
//#include <cstdlib> 		/* malloc() */

#include <iostream>
#include <mc_scverify.h>
#include "ntt.h"
#include "utils.h"
#include "config.h"

/**
 * Perform an PEASE NTT on an input vector and return the result
 *
 * @param vec 	The input vector to be transformed
 * @param n	The size of the input vector
 * @param p	The prime to be used as the modulus of the transformation
 * @param f	The primitive root of the prime
 * @param rev	Whether to perform bit reversal on the input vector
 * @return 	The transformed vector
 */
uint64_t* peaceNTT_golden(uint64_t * vec, uint64_t n, uint64_t p, uint64_t g, bool rev);

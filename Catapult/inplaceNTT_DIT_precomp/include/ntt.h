#ifndef NTT_H_
#define NTT_H_

// #include <cstdint> 	/* uint64_t */

#include <ac_int.h>
#include "config.h"




/**
 * Perform an in-place iterative breadth-first decimation-in-time Cooley-Tukey NTT on an input vector using precomputed 
 * twiddle factors and return the result
 *
 * @param vec 		The input vector to be transformed
 * @param n		The size of the input vector
 * @param p		The prime to be used as the modulus of the transformation
 * @param r		The primitive root of the prime
 * @param twiddle	The precomputed list of necessary twiddle factors
 * @param rev		Whether to perform bit reversal on the input vector
 * @return 		The transformed vector
 */
#pragma hls_design top
void inPlaceNTT_DIT_precomp(VEC_T vec[VECTOR_SIZE], PARAM_T p, PARAM_T r, VEC_T twiddle[VECTOR_SIZE]);


#endif

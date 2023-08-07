#include <cmath>		/* log2(), pow() */
#include <cstdint>		/* uint64_t */
#include <ac_math.h>
//#include <iostream>

//#include "../include/utils.h"	/* bit_reverse(), modExp(), modulo() */

#include "../include/ntt.h" 	//INCLUDE HEADER FILE
#include <ac_int.h>
#include <ac_math.h>
/**
 * Perform the operation 'base (mod m)'
 *
 * @param base	The base of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
#pragma hls_design ccore
UINT64_T modulo_dev(ac_int<64, true> base, UINT64_T m){
	ac_int<64, true> q, result;
	result = base % m;
	return (result >= 0) ? (UINT64_T)result : (UINT64_T)(result + m);
}

/**
 * Perform the operation 'base^exp (mod m)' using the memory-efficient method
 *
 * @param base	The base of the expression
 * @param exp	The exponent of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
#pragma hls_design inline
UINT64_T modExp_dev(UINT64_T base, UINT64_T exp, UINT64_T m){
	UINT64_T result = 1, q = 1;
	while(exp > 0){
		result = modulo_dev((UINT64_T)(result * base), m);
        exp--;
    }
	return result;
}
/**
 * We are using this right now mostly.
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
void inPlaceNTT_DIT(UINT64_T vec[VECTOR_SIZE],  UINT64_T p, UINT64_T r){
	UINT64_T factor1, factor2, k_, a;
	unsigned m;
	STAGE_LOOP: for(unsigned i = 1; i < VECTOR_ADDR_BIT; i++){ 

		m = 1 << i;
		k_ = (p - 1)/m;
		a = modExp_dev(r,k_,p);

		VEC_LOOP: for(unsigned  j = 0; j < VECTOR_SIZE; j+=m){
			
			COMP_LOOP: for(unsigned k = 0; k < m >> 1; k++){
				factor1 = vec[j + k];
				factor2 = modulo_dev(modExp_dev(a,k,p) * vec[j + k + m >> 1],  p);
				vec[j + k] = modulo_dev(factor1 + factor2, p);
				vec[j + k + m >> 1] = modulo_dev(factor1 - factor2, p);

			}
		}
	}
}
/**
 * ???
 *
 * @param vec 	The input vector to be transformed
 * @param n	The size of the input vector
 * @param p	The prime to be used as the modulus of the transformation
 * @param r	The primitive root of the prime
 * @param n2	The second factor of the vector decomposition
 * @return 	The transformed vector
 */
uint64_t *fourStepNTT(uint64_t *vec, uint64_t n, uint64_t p, uint64_t r, uint64_t n2){

	uint64_t n1 = n/n2;
	
	uint64_t *result;
	result = (uint64_t *) malloc(n*sizeof(uint64_t));

	for(uint64_t i = 0; i < n; i++){
		result[i] = vec[i];
	}

	//NTT ON N1
	uint64_t *temp;
	for(uint64_t i = 0; i < n1; i++){
		temp = inPlaceNTT_DIT(vec + i*n2, n2, p, r);
		for(uint64_t j = 0; j < n2; j++){
			result[i*n2 + j] = temp[j];
		}
	}

	//TWIDDLE FACTOR
	uint64_t k = (p - 1)/n;
	uint64_t a = modExp(r,k,p);

	for(uint64_t i = 0; i < n1; i++){
		for(uint64_t j = 0; j < n2; j++){
			result[i*n2 + j] = modulo(result[i*n2 + j]*modExp(a,i*j,p),p);
		}
	}
	

	//TRANSPOSE
	uint64_t *transpose;
	transpose = (uint64_t *) malloc(n*sizeof(uint64_t));

	for(uint64_t i = 0; i < n1; i++){
		for(uint64_t j = 0; j < n2; j++){
			transpose[j*n1 + i] = result[i*n2 + j];
		}
	}
	for(uint64_t i = 0; i < n; i++){
		result[i] = transpose[i];
	}

	//NTT ON N2
	for(uint64_t i = 0; i < n2; i++){
		temp = inPlaceNTT_DIT(result + i*n1, n1, p, r);
		for(uint64_t j = 0; j < n1; j++){
			result[i*n1 + j] = temp[j];
		}
	}
	
	//TRANSPOSE
	for(uint64_t i = 0; i < n2; i++){
		for(uint64_t j = 0; j < n1; j++){
			transpose[j*n2 + i] = result[i*n1 + j];
		}
	}
	for(uint64_t i = 0; i < n; i++){
		result[i] = transpose[i];
	}

	return result;

}

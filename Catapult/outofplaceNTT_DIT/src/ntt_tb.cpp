#include <cmath>		/* log2(), pow() */
#include <cstdint>		/* uint64_t */
#include <cstdlib> 		/* malloc() */

#include <iostream>

using namespace std;

/**
 * Perform the operation 'base (mod m)'
 *
 * @param base	The base of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
uint64_t modulo(int64_t base, int64_t m){
	int64_t result = base % m;
	return (result >= 0) ? result : result + m;
}
/**
 * Perform the operation 'base^exp (mod m)' using the memory-efficient method
 *
 * @param base	The base of the expression
 * @param exp	The exponent of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
uint64_t modExp(uint64_t base, uint64_t exp, uint64_t m){
	uint64_t result = 1, q = 1;
	while(exp > 0){
        result = (uint64_t)((result * base) % m);
		exp--;
    }
	return result;
}
/**
 * Perform an out-of-place decimation-in-time depth-first Cooley-Tukey NTT on an input vector and return the result
 *
 * @param vec 	The input vector to be transformed
 * @param n	The size of the input vector
 * @param p	The prime to be used as the modulus of the transformation
 * @param r	The primitive root of the prime
 * @return 	The transformed vector
 */
uint64_t *outOfPlaceNTT_DIT(uint64_t *vec, uint64_t n, uint64_t p, uint64_t r){
	if(n == 1)
		return vec;
	
	uint64_t k = (p - 1)/n;
	uint64_t a = modExp(r,k,p);
	uint64_t halfN = n >> 1;
	uint64_t *A0;
	A0 = (uint64_t *) malloc(halfN * sizeof(uint64_t));
	uint64_t *A1;
	A1 = (uint64_t *) malloc(halfN * sizeof(uint64_t));
	for(uint64_t i = 0; i < halfN; i++){
		A0[i] = vec[i*2];
		A1[i] = vec[i*2 + 1];
	}

	uint64_t *y0 = outOfPlaceNTT_DIT(A0, halfN, p, r);
	uint64_t *y1 = outOfPlaceNTT_DIT(A1, halfN, p, r);

	uint64_t *result;
	result = (uint64_t *) malloc(n*sizeof(uint64_t));

	uint64_t factor;
	for(uint64_t i = 0; i < halfN; i++){
		factor = modulo(modExp(a,i,p)*y1[i],p);
		result[i] 		= modulo(y0[i] + factor,p);	
		result[i + halfN] 	= modulo(y0[i] - factor,p);
	}
	return result;
}


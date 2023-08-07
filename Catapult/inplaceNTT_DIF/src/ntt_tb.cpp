
#include "../include/ntt_tb.h"
#include "../include/ntt.h"
#include "../include/config.h"
using namespace std;

void printVec(UINT64_T *vec){

	std::cout << "[";
	for(unsigned i = 0; i < VECTOR_SIZE; i++){

		std::cout << vec[i] << ",";

	}
	std::cout << "]" << std::endl;

}
/**
 * Perform the operation 'base (mod m)'
 *
 * @param base	The base of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
UINT64_T modulo(int64_t base, int64_t m){
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
#pragma hls_design inline
UINT64_T modExp(UINT64_T base, UINT64_T exp, UINT64_T m){
	UINT64_T result = 1, q = 1;
	while(exp > 0){
        result = (UINT64_T)((result * base) % m);
		exp--;
    }
	return result;
}
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
UINT64_T *inPlaceNTT_DIF_golden(UINT64_T *vec, UINT64_T n, uint64_t p, uint64_t r){

	UINT64_T *result;
	result = (UINT64_T *) malloc(n*sizeof(UINT64_T));

	for(UINT64_T i = 0; i < n; i++){
		result[i] = vec[i];
	}

	UINT64_T m,k_,a,factor1,factor2;
	for(UINT64_T i = log2(n); i >= 1; i--){

		m = pow(2,i);

		k_ = (p - 1)/m;
		a = modExp(r,k_,p);

		for(UINT64_T j = 0; j < n; j+=m){

			for(UINT64_T k = 0; k < m/2; k++){

				factor1 = result[UINT64_T(j + k)];
				factor2 = result[UINT64_T(j + k + m/2)];

				result[j + k] 		= modulo(int64_t(factor1 + factor2),p);
				result[j + k + m/2]	= modulo(modExp(a,k,p)*modulo(int64_t(factor1 - factor2),p),p);

			}
		}
	}
	
    return result;
}

CCS_MAIN(int argc, char **argv){

	UINT64_T p = 17;
	UINT64_T r = 3;	

	VEC_T vec[VECTOR_SIZE], output[VECTOR_SIZE];
	uint64_t * vec2 = (uint64_t *)malloc(VECTOR_SIZE * sizeof(uint64_t));
	uint64_t * golden_output;
	for (size_t i = 0; i < VECTOR_SIZE; i++){
		vec[i] = (VEC_T)i;
		vec2[i] = (uint64_t)i;
	}
	

	//printVec(inPlaceNTT_DIT_precomp(vec,n,p,r),n);
	CCS_DESIGN(inPlaceNTT_DIF)(vec, p, r, output);
    	golden_output = inPlaceNTT_DIF_golden(vec2, VECTOR_SIZE, p, r);
	printVec(golden_output);
/*	
	//uint64_t *vec = randVec(n,10);

	uint64_t *out1 = naiveNTT(vec,n,p,r);
	printVec(out1,n);

	uint64_t *out2 = fourStepNTT(vec,n,p,r,2);
	printVec(out2,n);
*/
	cout << __FILE__ << ":" << __LINE__ << " - End of testbench." << endl;
	CCS_RETURN(0);

}

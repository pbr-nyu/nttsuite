
#include "../include/ntt_tb.h"
#include "../include/ntt.h"
#include "../include/config.h"
using namespace std;

void printVec(uint64_t *vec){

	std::cout << "[";
	for(uint64_t i = 0; i < VECTOR_SIZE; i++){

		std::cout << vec[i] << ",";

	}
	std::cout << "]" << std::endl;

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

	uint64_t result = 1;
	
	while(exp > 0){

		if(exp % 2){

			result = modulo(result*base, m);

		}

		exp = exp >> 1;
		base = modulo(base*base,m);
	}

	return result;

}

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
 * Perform an in-place iterative breadth-first decimation-in-frequency Cooley-Tukey NTT on an input vector and return the result
 *
 * @param vec 	The input vector to be transformed
 * @param n	The size of the input vector
 * @param p	The prime to be used as the modulus of the transformation
 * @param r	The primitive root of the prime
 * @param rev	Whether to perform bit reversal on the output vector
 * @return 	The transformed vector
 */
uint64_t *inPlaceNTT_DIF_golden(uint64_t *vec, uint64_t n, uint64_t p, uint64_t r){

	uint64_t *result;
	result = (uint64_t *) malloc(n*sizeof(uint64_t));

	for(uint64_t i = 0; i < n; i++){
		result[i] = vec[i];
	}

	uint64_t m,k_,a,factor1,factor2;
	for(uint64_t i = log2(n); i >= 1; i--){

		m = pow(2,i);

		k_ = (p - 1)/m;
		a = modExp(r,k_,p);

		for(uint64_t j = 0; j < n; j+=m){

			for(uint64_t k = 0; k < m/2; k++){

				factor1 = result[j + k];
				factor2 = result[j + k + m/2];

				result[j + k] 		= modulo(factor1 + factor2,p);
				result[j + k + m/2]	= modulo(modExp(a,k,p)*modulo(factor1 - factor2,p),p);

			}
		}
	}
	
	/*if(rev){
		return bit_reverse(result,n);
	}else{
		return result;
	}*/
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
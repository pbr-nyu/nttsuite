#include <cmath>		/* pow() */
#include <cstdint>		/* uint64_t */
#include <ctime>		/* time() */

#include <unistd.h>
#include <iostream>

#include "../include/ntt.h"	/* naiveNTT(), outOfPlaceNTT_DIT() */
//#include "../include/utils.h"	/* printVec() */

//#include <ac_math.h>
//#include <ac_channel.h>
#include <mc_scverify.h>

using namespace std;
/**
 * Print an array of arbitrary length in a readable format
 *
 * @param vec	The array to be displayed
 * @param n	The length of the array
 */
void printVec(VEC_T *vec){

	std::cout << "[";
	for(unsigned i = 0; i < VECTOR_SIZE; i++){

		std::cout << vec[i] << ",";

	}
	std::cout << "]" << std::endl;

}
void bit_reverse_golden(VEC_T in[VECTOR_SIZE], VEC_T out[VECTOR_SIZE]){

	int reverse_num;
	for(unsigned i = 0; i < VECTOR_SIZE; i++){

		reverse_num = 0;
		for(unsigned j = 0; j < VECTOR_ADDR_BIT; j++){

			reverse_num = reverse_num << 1;
			if(i & (1 << j)){
				reverse_num = reverse_num | 1;
			}
		}

		out[reverse_num] = in[i];

	}


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
void inPlaceNTT_DIT_precomp_golden(VEC_T vec[VECTOR_SIZE],  PARAM_T p, PARAM_T r, VEC_T twiddle[VECTOR_SIZE], 
	VEC_T result[VECTOR_SIZE]){

	VEC_T bit_reverse_result_golden[VECTOR_SIZE];
	bit_reverse_golden(vec,bit_reverse_result_golden);

	PARAM_T m,factor1,factor2;
	for(unsigned i = 1; i <= VECTOR_ADDR_BIT; i++){ 

		m = pow(2,i);

		for(unsigned j = 0; j < VECTOR_SIZE; j+=m){

			for(unsigned k = 0; k < m/2; k++){

				factor1 = result[j + k];
				factor2 = modulo(twiddle[2^(i-1) - 1 + k] * result[j + k + m/2],p);

				result[j + k] 		= modulo(factor1 + factor2, p);
				result[j + k + m/2] 	= modulo(factor1 - factor2, p);

			}
		}
	}

}
CCS_MAIN(int argc, char *argv[]){

	PARAM_T p = 17;
	PARAM_T r = 3;	

	UINT64_T vec[VECTOR_SIZE], output[VECTOR_SIZE], golden_output[VECTOR_SIZE];
	for (size_t i = 0; i < VECTOR_SIZE; i++)
		vec[i] = (VEC_T)i;
	

	//printVec(inPlaceNTT_DIT_precomp(vec,n,p,r),n);
	CCS_DESIGN(inPlaceNTT_DIT_precomp)(vec, p, r, vec, output);
	printVec(output);
/*	
	//uint64_t *vec = randVec(n,10);

	uint64_t *out1 = naiveNTT(vec,n,p,r);
	printVec(out1,n);

	uint64_t *out2 = fourStepNTT(vec,n,p,r,2);
	printVec(out2,n);
*/
	CCS_RETURN(0);

}

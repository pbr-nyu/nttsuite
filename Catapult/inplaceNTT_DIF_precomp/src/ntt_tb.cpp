#include <cstdlib> 		/* malloc() */
#include <iostream>

#include <mc_scverify.h>

#include "../include/ntt.h"
#include "../include/config.h"
#include "../include/utils.h"

using namespace std;


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
DATA_TYPE *inPlaceNTT_DIF_golden(DATA_TYPE *vec, DATA_TYPE p, DATA_TYPE r){

	DATA_TYPE *result;
	result = (DATA_TYPE*) malloc(VECTOR_SIZE*sizeof(DATA_TYPE));

	for(unsigned i = 0; i < VECTOR_SIZE; i++){
		result[i] = vec[i];
	}

	DATA_TYPE k_,a,factor1,factor2;
	unsigned m;
	for(unsigned i = VECTOR_ADDR_BIT; i >= 1; i--){

		m = 1 << i;

		k_ = (p - 1)/m;
		a = modExp(r,k_,p);

		for(unsigned j = 0; j < VECTOR_SIZE; j+=m){

			for(unsigned k = 0; k < m/2; k++){

				factor1 = result[(unsigned)(j + k)];
				factor2 = result[(unsigned)(j + k + m/2)];

				result[(unsigned)(j + k)] 		= modulo((factor1 + factor2),p);
				result[(unsigned)(j + k + m/2)]	= ((DATA_TYPE_TMP)((DATA_TYPE_TMP)modExp(a,k,p) * modulo(factor1 - factor2,p))) % p;

			}
		}
	}
	
    return result;
}

void randVec(DATA_TYPE * vec, DATA_TYPE * vec2, DATA_TYPE max){
	for(unsigned i = 0; i < VECTOR_SIZE; i++){
		DATA_TYPE value = rand() % (max + 1);
		vec[i] = value;
		vec2[i] = value;
	}
}


void gettwiddle(DATA_TYPE *twiddle, DATA_TYPE *twiddle_h, DATA_TYPE p, DATA_TYPE r){

	DATA_TYPE witer = 1;
    	DATA_TYPE w_ 	= modExp(r, (p - 1) / VECTOR_SIZE, p);

	for(int i=0; i < VECTOR_SIZE; i++){
		twiddle[i] = witer;
		twiddle_h[i] = ((DATA_TYPE_TMP)witer << PARAM_WIDTH) / p;
		witer = ((DATA_TYPE_TMP)witer * w_) % p;
	}

}

CCS_MAIN(int argc, char **argv){

	DATA_TYPE p = (479  << 21) + 1;
	DATA_TYPE r = 3;	

	DATA_TYPE vec[VECTOR_SIZE], vec2[VECTOR_SIZE], twiddle[VECTOR_SIZE], twiddle_h[VECTOR_SIZE];
	DATA_TYPE naiveResult[VECTOR_SIZE];
	DATA_TYPE *nttResult;
	randVec(vec, vec2, 1000);
	gettwiddle(twiddle, twiddle_h, p, r);

	//printVec(inPlaceNTT_DIT_precomp(vec,n,p,r),n);
	CCS_DESIGN(inPlaceNTT_DIF)(vec, p, r, twiddle, twiddle_h);
	naiveNTT(vec2, VECTOR_SIZE, p, r, twiddle, twiddle_h, naiveResult);
	bit_reverse(vec, VECTOR_SIZE, nttResult);
	compVec(naiveResult, nttResult, true);
	cout << __FILE__ << ":" << __LINE__ << " - End of testbench." << endl;
	CCS_RETURN(0);

}
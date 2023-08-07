#include <iostream>


#include "../include/ntt.h"
#include "../include/config.h"
#include "../include/utils.h"

using namespace std;
void randVec(DATA_TYPE * vec, DATA_TYPE * vec2, DATA_TYPE max){
	for(DATA_TYPE i = 0; i < VECTOR_SIZE; i++){
		DATA_TYPE value = rand() % (max + 1);
		value = i;
		vec[i] = value;
		vec2[i] = value;
	}
}

/**
 * Perform a basic NTT on an input vector and return the result
 *
 * @param vec 	The input vector to be transformed
 * @param n	The size of the input vector
 * @param p	The prime to be used as the modulus of the transformation
 * @param r	The primitive root of the prime
 * @return 	The transformed vector
 */
DATA_TYPE *naiveNTT(DATA_TYPE *vec, DATA_TYPE n, DATA_TYPE p, DATA_TYPE r, DATA_TYPE *twiddle, DATA_TYPE *twiddle_h, DATA_TYPE *result){

	DATA_TYPE temp;
	for(unsigned i = 0; i < n; i++){

		temp = 0;
		for(unsigned j = 0; j < n; j++){
	
			DATA_TYPE multRes = mult(vec[j], twiddle[(i * j) % VECTOR_SIZE], twiddle_h[(i * j) % VECTOR_SIZE], p);
			temp = (temp + multRes) % p;
			/*temp = temp + modulo(vec[j]*modExp(a, i*j, p),p);*/
			/*temp = temp + vec[j]*modExp(a, i*j, p);*/
		}
		result[i] = temp;
	}
	return result;

}

void gettwiddle(DATA_TYPE *twiddle, DATA_TYPE *twiddle_h, DATA_TYPE p, DATA_TYPE r){

	DATA_TYPE witer = 1;
    	DATA_TYPE w_ 	= modExp(r, (p - 1) / VECTOR_SIZE, p);

	for(PARAMS_TYPE i=0; i < VECTOR_SIZE; i++){
		twiddle[i] = witer;
		twiddle_h[i] = ((DATA_TYPE_TMP)witer << VECTOR_WIDTH)/ p;
		witer = ((DATA_TYPE_TMP)witer * w_) % p;
	}

}

CCS_MAIN(int argc, char** argv){

	const unsigned VECTOR_COUNT = 3;  // number of vectors to test
	unsigned       correct_count = 0;

    // Modulo
	DATA_TYPE p = (479  << 21) + 1;
	// Root
	DATA_TYPE r = 3;	

    	// input vector and twiddles
    DATA_TYPE vec[VECTOR_SIZE], vec2[VECTOR_SIZE], twiddle[VECTOR_SIZE], twiddle_h[VECTOR_SIZE];
	DATA_TYPE naiveResult[VECTOR_SIZE], nttResult[VECTOR_SIZE];
	ac_sync        run, complete;

	gettwiddle(twiddle, twiddle_h, p, r);

	for(unsigned itr = 0; itr < VECTOR_COUNT; itr++){
		randVec(vec, vec2, 1000);
		bit_reverse(vec, VECTOR_SIZE, nttResult);
		run.sync_out();
#ifdef CCS_SCVERIFY
		if (itr==0) {
		// due to the nb_sync_in, we need to ignore the first output compare
		// because result will not have been initialized with any values
		//testbench::result_ignore = true;
		}
#endif
		CCS_DESIGN(peaseNTT)(run, nttResult, p, r, twiddle, twiddle_h, complete);
		complete.sync_in();
		naiveNTT(vec2, VECTOR_SIZE, p, r, twiddle, twiddle_h, naiveResult);
		correct_count += compVec(nttResult, naiveResult, VECTOR_SIZE, true);
	}
	
	  cout << __FILE__ <<":Testbench Complete with " << 
	  	VECTOR_COUNT -  correct_count<< " mismatches, after running " << 
		  VECTOR_COUNT << " iteration(s)." << endl;
	CCS_RETURN(VECTOR_COUNT - correct_count);
}
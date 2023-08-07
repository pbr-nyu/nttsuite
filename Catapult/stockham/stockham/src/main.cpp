#include <cstdlib> 		/* malloc() */
#include <iostream>


#include "../include/ntt.h"
#include "../include/config.h"
#include "../include/utils.h"

using namespace std;
void randVec(DATA_TYPE * vec, DATA_TYPE * vec2, DATA_TYPE max){
	for(unsigned i = 0; i < VECTOR_SIZE; i++){
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
void naiveNTT(DATA_TYPE *vec, DATA_TYPE n, DATA_TYPE p, DATA_TYPE r, DATA_TYPE *twiddle, DATA_TYPE *twiddle_h, DATA_TYPE *result){

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

int main(int argc, char **argv){

	// Modulo
	DATA_TYPE p = (479  << 21) + 1;
	// Root
	DATA_TYPE r = 3;	

	// input vector and twiddles
	DATA_TYPE vec[VECTOR_SIZE], vec2[VECTOR_SIZE], twiddle[VECTOR_SIZE], twiddle_h[VECTOR_SIZE];
	DATA_TYPE naiveResult[VECTOR_SIZE], nttResult[VECTOR_SIZE];
	randVec(vec, vec2, 1000);
	gettwiddle(twiddle, twiddle_h, p, r);

	
	stockham_dit(vec, p, twiddle, twiddle_h);
	naiveNTT(vec2, VECTOR_SIZE, p, r, twiddle, twiddle_h, naiveResult);
	compVec(vec, naiveResult, VECTOR_SIZE, true);

	//cout << __FILE__ << ":" << __LINE__ << " - End of testbench." << endl;
	//CCS_RETURN(0);
	return 0;

}
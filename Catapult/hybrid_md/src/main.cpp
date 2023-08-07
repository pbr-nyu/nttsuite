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


void gettwiddle(DATA_TYPE *twiddle, DATA_TYPE *twiddle_h, DATA_TYPE p, DATA_TYPE r, PARAMS_TYPE n){

	DATA_TYPE witer = 1;
    	DATA_TYPE w_ 	= modExp(r, (p - 1) / n, p);

	for(int i=0; i < n; i++){
		twiddle[i]	= witer;
		twiddle_h[i]	= ((DATA_TYPE_TMP)witer << PARAM_WIDTH) / p;
		witer		= ((DATA_TYPE_TMP)witer * w_) % p;
	}

}

int main(int argc, char **argv){

	// Modulo
	DATA_TYPE p = (479  << 21) + 1;
	// Root
	DATA_TYPE r = 3;	

	// input vector and twiddles
	DATA_TYPE vec[VECTOR_SIZE], vec2[VECTOR_SIZE], twiddle[HALF_VECTOR_SIZE], twiddle_h[HALF_VECTOR_SIZE], tw[VECTOR_SIZE], tw_h[VECTOR_SIZE];
	PARAMS_TYPE revArr[HALF_VECTOR_SIZE];
	DATA_TYPE naiveResult[VECTOR_SIZE], nttResult[VECTOR_SIZE];
	randVec(vec, vec2, 1000);
	gettwiddle(twiddle, twiddle_h, p, r, HALF_VECTOR_SIZE);
	gettwiddle(tw, tw_h, p, r, VECTOR_SIZE);
	getBitReverseArr(HALF_VECTOR_SIZE, revArr);

	hybrid(vec, p, twiddle, twiddle_h, revArr, tw, tw_h);
	naiveNTT(vec2, VECTOR_SIZE, p, r, tw, tw_h, naiveResult);
	compVec(vec, naiveResult, VECTOR_SIZE, true);

	//cout << __FILE__ << ":" << __LINE__ << " - End of testbench." << endl;
	//CCS_RETURN(0);
	return 0;

}
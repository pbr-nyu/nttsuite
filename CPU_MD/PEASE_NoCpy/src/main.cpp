#include <cstdlib> 		/* malloc() */
#include <iostream>
#include <sys/time.h>


#include "../include/ntt.h"
#include "../include/config.h"
#include "../include/utils.h"

//#define DEBUG

using namespace std;

void randVec(DATA_TYPE * vec, DATA_TYPE * vec2, DATA_TYPE max){
	for(unsigned i = 0; i < VECTOR_SIZE; i++){
		DATA_TYPE value = rand() % (max + 1);
		value = i;
		vec[i] = value;
		vec2[i] = value;
	}
}


void gettwiddle(DATA_TYPE *twiddle, DATA_TYPE *twiddle_h, DATA_TYPE p, DATA_TYPE r, DATA_TYPE n){

	DATA_TYPE witer = 1;
    	DATA_TYPE w_ 	= modExp(r, (p - 1) / n, p);

	for(int i=0; i < n; i++){
		twiddle[i]	= witer;
		twiddle_h[i]	= ((DATA_TYPE_TMP)witer << VECTOR_WIDTH) / p;
		witer		= ((DATA_TYPE_TMP)witer * w_) % p;
	}

}

int main(int argc, char **argv){

	// Modulo
	DATA_TYPE p = (479  << 21) + 1;
	// Root
	DATA_TYPE r = 3;	

	// input vector and twiddles
	DATA_TYPE vec[VECTOR_SIZE], vec2[VECTOR_SIZE], tw[VECTOR_SIZE], tw_h[VECTOR_SIZE];
	DATA_TYPE naiveResult[VECTOR_SIZE], nttResult[VECTOR_SIZE];
	randVec(vec, vec2, 1000);
	gettwiddle(tw, tw_h, p, r, VECTOR_SIZE);


#ifdef DEBUG
	bit_reverse(vec, VECTOR_SIZE, nttResult);
	peaseNTT(nttResult, p, r, tw, tw_h);
	naiveNTT(vec2, VECTOR_SIZE, p, r, tw, tw_h, naiveResult);
	compVec(nttResult, naiveResult, VECTOR_SIZE, true);
#else
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	// Sample times
	int times = 1000;
	for(int i=0; i < times; i++)
		peaseNTT(vec, p, r, tw, tw_h);
	gettimeofday(&end, NULL);
	long seconds = (end.tv_sec - begin.tv_sec);
	long micros = ((seconds * 1000000) + end.tv_usec) - (begin.tv_usec);
	cout<<micros/times<<endl;
#endif

	//cout << __FILE__ << ":" << __LINE__ << " - End of testbench." << endl;
	//CCS_RETURN(0);
	return 0;

}

#include "../include/ntt.h"

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
        result = ((UINT64_T)(result * base) % m);
        exp--;
    }
	return result;
}

#pragma hls_design top
void stockham_DIT(UINT64_T xt[VECTOR_SIZE], UINT64_T p, UINT64_T g, UINT64_T twiddle[VECTOR_SIZE]){

    UINT64_T yt[VECTOR_SIZE];
    UINT64_T *x = xt;
    UINT64_T *y = yt;

    UINT64_T *tmp;
    tmp = x;
    x = y;
    y = tmp;

    unsigned s = VECTOR_SIZE >> 1;
    unsigned shift = VECTOR_ADDR_BIT - 1;

    OUTER_LOOP:for(unsigned nn = 1; nn < VECTOR_ADDR_BIT ; nn++, s = s >> 1, shift = shift - 1){
        const int m = 1 << (nn - 1);
        INNER_LOOP:for (int t = 0; t < (VECTOR_SIZE >> 1); t++) {
                const int upper = ((~(s - 1) & t));
                UINT64_T w = twiddle[(upper >> shift) << shift];
                const int q = (s - 1) & t;
                const UINT64_T a = y[q + (upper << 1)];
                const UINT64_T b = (y[q + (upper << 1) + s] * w) % p;
                x[q + upper] = modulo_dev(a + b, p);
                x[q + upper + s * m] = modulo_dev(a - b, p);
        }
	    tmp = x;
        x = y;
        y = tmp;
    }   
    if(VECTOR_ADDR_BIT % 2){
        COPY_LOOP:for(int i=0; i<VECTOR_SIZE; i++)
            xt[i] = y[i];
    }
}
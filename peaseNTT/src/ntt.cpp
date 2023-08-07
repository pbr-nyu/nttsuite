#include "../include/ntt.h"

/**
 * Perform the operation 'base^exp (mod m)' using the memory-efficient method
 *
 * @param base	The base of the expression
 * @param exp	The exponent of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
#pragma hls_design inline
inline DATA_TYPE modulo_add(DATA_TYPE base, DATA_TYPE m){
	return base > m? base - m : base;

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
inline DATA_TYPE modulo_sub(DATA_TYPE_SIGNED base, DATA_TYPE m){
	return base < 0? base + m : base;

}

/**
 * perfrom the operation (x * y) (mod m)
 *
 * @param x 1st operator
 * @param y 2nd operator
 * @param y_    helper operator
 * @param p modulo
 * @return  the result of the expression
 */
DATA_TYPE mult(DATA_TYPE x, DATA_TYPE y, DATA_TYPE y_, DATA_TYPE p){
    DATA_TYPE z = (DATA_TYPE_TMP)x * y; 
    DATA_TYPE t = ((DATA_TYPE_TMP)x * y_) >> PARAM_WIDTH; 
    DATA_TYPE z_ = (DATA_TYPE_TMP)t * p; 
    DATA_TYPE res = z - z_;
    if(res >= p)
        res = res - p;
    return res;
}

/**
 * Perform the butterfly operation
 *
 * @param vec	Array
 * @param idx1	The first index
 * @param idx2	The second index
 * @param tw	twiddle factor
 * @param tw_h	twiddle factor helper
 * @param p	The modulus of the expression
 * @return 	The result of the expression
 */
#pragma hls_design inline
void butterFly(DATA_TYPE xt[VECTOR_SIZE], DATA_TYPE yt[VECTOR_SIZE], PARAMS_TYPE r, DATA_TYPE p, PARAMS_TYPE base, DATA_TYPE twiddle[VECTOR_SIZE], DATA_TYPE twiddle_h[VECTOR_SIZE]){
        DATA_TYPE tw = twiddle[base & r];
        DATA_TYPE tw_h = twiddle_h[base & r];
	DATA_TYPE f1 = xt[r << 1];
	DATA_TYPE f2 = mult(xt[(r << 1) + 1], tw, tw_h, p);
	yt[r]       = modulo_add(f1 + f2, p);
	yt[r + (VECTOR_SIZE >> 1)]  = modulo_sub(f1 - f2, p);
}


void cpyVec(DATA_TYPE* src, DATA_TYPE*dst, PARAMS_TYPE length){
    for(PARAMS_TYPE i=0; i<length; i++){
        dst[i] = src[i];
    }
}

 
#pragma hls_design top
void peaseNTT(DATA_TYPE xt[VECTOR_SIZE], DATA_TYPE p, DATA_TYPE r, DATA_TYPE twiddle[VECTOR_SIZE], DATA_TYPE twiddle_h[VECTOR_SIZE]){

    	DATA_TYPE factor1, factor2;
	PARAMS_TYPE mid = VECTOR_SIZE >> 1;
	DATA_TYPE yt[VECTOR_SIZE];
	STAGE_LOOP: for (PARAMS_TYPE c = VECTOR_ADDR_BIT; c >= 1; c--){

        	PARAMS_TYPE base = -1 << (c - 1);

        	INNER_LOOP: for (PARAMS_TYPE r = 0; r < mid; r += 4){
			butterFly(xt, yt, r, p, base, twiddle, twiddle_h);
			butterFly(xt, yt, r + 1, p, base, twiddle, twiddle_h);
			butterFly(xt, yt, r + 2, p, base, twiddle, twiddle_h);
			butterFly(xt, yt, r + 3, p, base, twiddle, twiddle_h);
        	}
        	cpyVec(yt, xt, VECTOR_SIZE);
    	}
}
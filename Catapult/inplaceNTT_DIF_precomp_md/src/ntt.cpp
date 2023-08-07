#include "../include/ntt.h"

/**
 * Perform the operation 'base^exp (mod m)' using the memory-efficient method
 *
 * @param base	The base of the expression
 * @param exp	The exponent of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
#pragma hls_design ccore
DATA_TYPE modulo_add(DATA_TYPE_SIGNED base, DATA_TYPE m){
	return base > m? (DATA_TYPE)(base - m) : (DATA_TYPE)base;
}

/**
 * Perform the operation 'base^exp (mod m)' using the memory-efficient method
 *
 * @param base	The base of the expression
 * @param exp	The exponent of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
#pragma hls_design ccore
DATA_TYPE modulo_sub(DATA_TYPE_SIGNED base, DATA_TYPE m){
	return base < 0 ? (DATA_TYPE)(base + m) : (DATA_TYPE)(base);
}

/**
 * perfrom the operation (x * y) (mod m)
 *
 * @param x	1st operator
 * @param y	2nd operator
 * @param y_	helper operator
 * @param p	modulo
 * @return 	the result of the expression
 */
#pragma hls_design ccore
DATA_TYPE mult(DATA_TYPE x, DATA_TYPE y, DATA_TYPE y_, DATA_TYPE p){
	DATA_TYPE z = (DATA_TYPE_TMP)x * y; 
	DATA_TYPE t = ((DATA_TYPE_TMP)x * y_) >> PARAM_WIDTH; 
	DATA_TYPE z_ = (DATA_TYPE_TMP)t * p; 
	DATA_TYPE res = z - z_;
	if(res >= p)
		res = res - p;
	return res;
}
#pragma hls_design top
void inPlaceNTT_DIF_precomp(ac_sync & run, DATA_TYPE vec[VECTOR_SIZE], DATA_TYPE p, DATA_TYPE r, 
	DATA_TYPE twiddle[VECTOR_SIZE], DATA_TYPE twiddle_h[VECTOR_SIZE], ac_sync & complete){
    	DATA_TYPE factor1, factor2;

	unsigned m;
	if (run.nb_sync_in()){
		STAGE_LOOP: for(unsigned i = VECTOR_SIZE_LOG2; i > 0; i--){ 
			m = 1 << i;
			COMP_LOOP: for(unsigned k = 0; k < m/2; k++){
				DATA_TYPE twiddle_f = twiddle[(1 << (VECTOR_SIZE_LOG2 - i)) * k];
				DATA_TYPE twiddle_help = twiddle_h[(1 << (VECTOR_SIZE_LOG2 - i)) * k];
				VEC_LOOP: for(unsigned  j = 0; j < VECTOR_SIZE; j+=m){
					
					factor1 = vec[j + k];
					factor2 = vec[j + k + m/2];
					vec[j + k] = modulo_add(factor1 + factor2, p);
					vec[j + k + m/2] = mult(modulo_sub(factor1 - factor2, p), twiddle_f, twiddle_help, p);
				}
			}
		}
		complete.sync_out();
	}
}
#include "../include/ntt.h"
//#include "../include/utils.h"
/**
 * perfrom the operation (x * y) (mod m)
 *
 * @param x	1st operator
 * @param y	2nd operator
 * @param y_	helper operator
 * @param p	modulo
 * @return 	the result of the expression
 */
DATA_TYPE mult_dev(DATA_TYPE x, DATA_TYPE y, DATA_TYPE y_, DATA_TYPE p){
	DATA_TYPE z = (DATA_TYPE_TMP)x * y; 
	DATA_TYPE t = ((DATA_TYPE_TMP)x * y_) >> PARAM_WIDTH; 
	DATA_TYPE z_ = (DATA_TYPE_TMP)t * p; 
	DATA_TYPE res = z - z_;
	if(res >= p)
		res = res - p;
	return res;
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
inline DATA_TYPE modulo_add_dev(DATA_TYPE_SIGNED base, DATA_TYPE m){
	if(base > m)
		return base - m;
	else
		return base;
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
inline DATA_TYPE modulo_sub_dev(DATA_TYPE_SIGNED base, DATA_TYPE m){
	if(base < 0)
		return base + m;
	else
		return base;

}

#pragma hls_design top
void inPlaceNTT_DIF(DATA_TYPE vec[VECTOR_SIZE], DATA_TYPE p, DATA_TYPE r, DATA_TYPE twiddle[VECTOR_SIZE], DATA_TYPE twiddle_h[VECTOR_SIZE]){
    	DATA_TYPE factor1, factor2;
	unsigned m;
	STAGE_LOOP: for(unsigned i = VECTOR_ADDR_BIT; i > 0; i--){ 
		m = 1 << i;
		COMP_LOOP: for(unsigned k = 0; k < m/2; k++){
			DATA_TYPE twiddle_f = twiddle[(1 << (VECTOR_ADDR_BIT - i)) * k];
			DATA_TYPE twiddle_help = twiddle_h[(1 << (VECTOR_ADDR_BIT - i)) * k];
			VEC_LOOP: for(unsigned  j = 0; j < VECTOR_SIZE; j+=m){
				factor1 = vec[j + k];
				factor2 = vec[j + k + m/2];
				vec[j + k] = modulo_add_dev(factor1 + factor2, p);
				vec[j + k + m/2] = mult_dev(modulo_sub_dev(factor1 - factor2, p), twiddle_f, twiddle_help, p);
			}
			
		}
		
	}
	
}

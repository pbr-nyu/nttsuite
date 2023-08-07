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
#pragma hls_design inline
inline DATA_TYPE mult(DATA_TYPE x, DATA_TYPE y, DATA_TYPE y_, DATA_TYPE p){
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
void butterFly(DATA_TYPE *vec, PARAMS_TYPE idx1, PARAMS_TYPE idx2, DATA_TYPE tw, DATA_TYPE tw_h, DATA_TYPE p){
    DATA_TYPE f1 = vec[idx1];
    DATA_TYPE f2 = vec[idx2];
    vec[idx1] = modulo_add(f1 + f2, p);
    vec[idx2] = mult(modulo_sub(f1 - f2, p), tw, tw_h, p);
}
 
#pragma hls_design top
void ntt_flat(DATA_TYPE vec[VECTOR_SIZE], DATA_TYPE p, DATA_TYPE r, DATA_TYPE twiddle[VECTOR_SIZE], DATA_TYPE twiddle_h[VECTOR_SIZE]){

    	DATA_TYPE factor1, factor2;
	PARAMS_TYPE m;
	PARAMS_TYPE mid = VECTOR_SIZE >> 1;

	STAGE_LOOP: for(PARAMS_TYPE i = 1; i <= VECTOR_ADDR_BIT; i++){
		PARAMS_TYPE shift_idx = VECTOR_ADDR_BIT - i;
		PARAMS_TYPE gp = 1 << (i - 1);
		PARAMS_TYPE op =  VECTOR_SIZE >> i;
		PARAMS_TYPE point = op << 1;
		INNER_LOOP: for(PARAMS_TYPE j=0; j < mid; j++){
			PARAMS_TYPE g = j >> shift_idx;
			PARAMS_TYPE k = j - (g << shift_idx);
			PARAMS_TYPE idx1= point * g + k;
			PARAMS_TYPE idx2= idx1 + op;
			DATA_TYPE tf = twiddle[gp * k];
			DATA_TYPE tf_h = twiddle_h[gp * k];
			//cout<<idx1<<" "<<idx2<<" "<<gp*t<<endl;
			butterFly(vec, idx1, idx2, tf, tf_h, p);
		}
    }
}
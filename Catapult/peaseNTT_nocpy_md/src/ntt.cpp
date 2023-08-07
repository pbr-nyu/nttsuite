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
DATA_TYPE modulo_add(DATA_TYPE base, DATA_TYPE m){
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
DATA_TYPE modulo_sub(DATA_TYPE_SIGNED base, DATA_TYPE_SIGNED m){
	return base < 0? (DATA_TYPE)(base + m) : (DATA_TYPE)base;
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
	DATA_TYPE z = (DATA_TYPE)((DATA_TYPE_TMP)x * y); 
	DATA_TYPE t = (DATA_TYPE)(((DATA_TYPE_TMP)x * y_) >> VECTOR_WIDTH); 
	DATA_TYPE z_ = (DATA_TYPE)((DATA_TYPE_TMP)t * p); 
	DATA_TYPE res = z - z_;
	if(res >= p)
		res = res - p;
	return (DATA_TYPE)res;
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
#pragma hls_design ccore
void butterFly1(DATA_TYPE xt[VECTOR_SIZE], DATA_TYPE yt[VECTOR_SIZE], PARAMS_TYPE r, DATA_TYPE p, PARAMS_TYPE base, DATA_TYPE tw, DATA_TYPE tw_h){
	DATA_TYPE f1 = xt[r << 1];
	DATA_TYPE f2 = (DATA_TYPE)mult(xt[(r << 1) + 1], tw, tw_h, p);
	yt[r]       = (DATA_TYPE)modulo_add(f1 + f2, p);
	yt[r + (VECTOR_SIZE >> 1)]  = (DATA_TYPE)modulo_sub(f1 - f2, p);
}

#pragma hls_design ccore
void butterFly2(DATA_TYPE xt[VECTOR_SIZE], DATA_TYPE yt[VECTOR_SIZE], PARAMS_TYPE r, DATA_TYPE p, PARAMS_TYPE base, DATA_TYPE twiddle[VECTOR_SIZE], DATA_TYPE twiddle_h[VECTOR_SIZE]){
    DATA_TYPE tw = twiddle[base & r];
    DATA_TYPE tw_h = twiddle_h[base & r];
	DATA_TYPE f1 = xt[r << 1];
	DATA_TYPE f2 = (DATA_TYPE)mult(xt[(r << 1) + 1], tw, tw_h, p);
	yt[r]       = (DATA_TYPE)modulo_add(f1 + f2, p);
	yt[r + (VECTOR_SIZE >> 1)]  = (DATA_TYPE)modulo_sub(f1 - f2, p);
}
 

#pragma hls_design top
void peaseNTT(ac_sync & run, DATA_TYPE xt[VECTOR_SIZE], DATA_TYPE p, DATA_TYPE r, 
	DATA_TYPE twiddle[VECTOR_SIZE], DATA_TYPE twiddle_h[VECTOR_SIZE], ac_sync & complete){

    DATA_TYPE factor1, factor2;
	PARAMS_TYPE mid = VECTOR_SIZE >> 1;
	DATA_TYPE yt[VECTOR_SIZE];
	PARAMS_TYPE c;

	if (run.nb_sync_in()){
		STAGE_LOOP: for (c = VECTOR_ADDR_BIT; c >= 6; c -= 2){

			PARAMS_TYPE base = -1 << (c - 1);

			INNER_LOOP1: for (PARAMS_TYPE r = 0; r < mid; r += 16){
				DATA_TYPE tw = twiddle[base & r];
				DATA_TYPE tw_h = twiddle_h[base & r];
				butterFly1(xt, yt, r + 0, p, base, tw, tw_h);
				butterFly1(xt, yt, r + 1, p, base, tw, tw_h);
				butterFly1(xt, yt, r + 2, p, base, tw, tw_h);
				butterFly1(xt, yt, r + 3, p, base, tw, tw_h);
				butterFly1(xt, yt, r + 4, p, base, tw, tw_h);
				butterFly1(xt, yt, r + 5, p, base, tw, tw_h);
				butterFly1(xt, yt, r + 6, p, base, tw, tw_h);
				butterFly1(xt, yt, r + 7, p, base, tw, tw_h);
				butterFly1(xt, yt, r + 8, p, base, tw, tw_h);
				butterFly1(xt, yt, r + 9, p, base, tw, tw_h);
				butterFly1(xt, yt, r + 10, p, base, tw, tw_h);
				butterFly1(xt, yt, r + 11, p, base, tw, tw_h);
				butterFly1(xt, yt, r + 12, p, base, tw, tw_h);
				butterFly1(xt, yt, r + 13, p, base, tw, tw_h);
				butterFly1(xt, yt, r + 14, p, base, tw, tw_h);
				butterFly1(xt, yt, r + 15, p, base, tw, tw_h);
			}
			base = -1 << (c - 2);
			INNER_LOOP2: for (PARAMS_TYPE r = 0; r < mid; r += 16){
				DATA_TYPE tw = twiddle[base & r];
				DATA_TYPE tw_h = twiddle_h[base & r];
				butterFly1(yt, xt, r + 0, p, base, tw, tw_h);
				butterFly1(yt, xt, r + 1, p, base, tw, tw_h);
				butterFly1(yt, xt, r + 2, p, base, tw, tw_h);
				butterFly1(yt, xt, r + 3, p, base, tw, tw_h);
				butterFly1(yt, xt, r + 4, p, base, tw, tw_h);
				butterFly1(yt, xt, r + 5, p, base, tw, tw_h);
				butterFly1(yt, xt, r + 6, p, base, tw, tw_h);
				butterFly1(yt, xt, r + 7, p, base, tw, tw_h);
				butterFly1(yt, xt, r + 8, p, base, tw, tw_h);
				butterFly1(yt, xt, r + 9, p, base, tw, tw_h);
				butterFly1(yt, xt, r + 10, p, base, tw, tw_h);
				butterFly1(yt, xt, r + 11, p, base, tw, tw_h);
				butterFly1(yt, xt, r + 12, p, base, tw, tw_h);
				butterFly1(yt, xt, r + 13, p, base, tw, tw_h);
				butterFly1(yt, xt, r + 14, p, base, tw, tw_h);
				butterFly1(yt, xt, r + 15, p, base, tw, tw_h);
			}
		}

		STAGE_LOOP1: for (; c >= 2; c -= 2){
			PARAMS_TYPE base = -1 << (c - 1);
				INNER_LOOP3: for (PARAMS_TYPE r = 0; r < mid; r += 16){
				butterFly2(xt, yt, r + 0, p, base, twiddle, twiddle_h);
				butterFly2(xt, yt, r + 1, p, base, twiddle, twiddle_h);
				butterFly2(xt, yt, r + 2, p, base, twiddle, twiddle_h);
				butterFly2(xt, yt, r + 3, p, base, twiddle, twiddle_h);
				butterFly2(xt, yt, r + 4, p, base, twiddle, twiddle_h);
				butterFly2(xt, yt, r + 5, p, base, twiddle, twiddle_h);
				butterFly2(xt, yt, r + 6, p, base, twiddle, twiddle_h);
				butterFly2(xt, yt, r + 7, p, base, twiddle, twiddle_h);
				butterFly2(xt, yt, r + 8, p, base, twiddle, twiddle_h);
				butterFly2(xt, yt, r + 9, p, base, twiddle, twiddle_h);
				butterFly2(xt, yt, r + 10, p, base, twiddle, twiddle_h);
				butterFly2(xt, yt, r + 11, p, base, twiddle, twiddle_h);
				butterFly2(xt, yt, r + 12, p, base, twiddle, twiddle_h);
				butterFly2(xt, yt, r + 13, p, base, twiddle, twiddle_h);
				butterFly2(xt, yt, r + 14, p, base, twiddle, twiddle_h);
				butterFly2(xt, yt, r + 15, p, base, twiddle, twiddle_h);
			}

			base = -1 << (c - 2);
			INNER_LOOP4: for (PARAMS_TYPE r = 0; r < mid; r += 16){
				butterFly2(yt, xt, r + 0, p, base, twiddle, twiddle_h);
				butterFly2(yt, xt, r + 1, p, base, twiddle, twiddle_h);
				butterFly2(yt, xt, r + 2, p, base, twiddle, twiddle_h);
				butterFly2(yt, xt, r + 3, p, base, twiddle, twiddle_h);
				butterFly2(yt, xt, r + 4, p, base, twiddle, twiddle_h);
				butterFly2(yt, xt, r + 5, p, base, twiddle, twiddle_h);
				butterFly2(yt, xt, r + 6, p, base, twiddle, twiddle_h);
				butterFly2(yt, xt, r + 7, p, base, twiddle, twiddle_h);
				butterFly2(yt, xt, r + 8, p, base, twiddle, twiddle_h);
				butterFly2(yt, xt, r + 9, p, base, twiddle, twiddle_h);
				butterFly2(yt, xt, r + 10, p, base, twiddle, twiddle_h);
				butterFly2(yt, xt, r + 11, p, base, twiddle, twiddle_h);
				butterFly2(yt, xt, r + 12, p, base, twiddle, twiddle_h);
				butterFly2(yt, xt, r + 13, p, base, twiddle, twiddle_h);
				butterFly2(yt, xt, r + 14, p, base, twiddle, twiddle_h);
				butterFly2(yt, xt, r + 15, p, base, twiddle, twiddle_h);
			}
		}
		complete.sync_out();
	}
}

 /*
#pragma hls_design top
void peaseNTT(DATA_TYPE xt[VECTOR_SIZE], DATA_TYPE p, DATA_TYPE r, DATA_TYPE twiddle[VECTOR_SIZE], DATA_TYPE twiddle_h[VECTOR_SIZE]){

    	DATA_TYPE factor1, factor2;
	PARAMS_TYPE mid = VECTOR_SIZE >> 1;
	DATA_TYPE yt[VECTOR_SIZE];
	PARAMS_TYPE c;
	STAGE_LOOP: for (c = VECTOR_ADDR_BIT; c >= 4; c -= 2){

        	PARAMS_TYPE base = -1 << (c - 1);

        	INNER_LOOP1: for (PARAMS_TYPE r = 0; r < mid; r += 4){
			DATA_TYPE tw = twiddle[base & r];
			DATA_TYPE tw_h = twiddle_h[base & r];
			butterFly1(xt, yt, r + 0, p, base, tw, tw_h);
			butterFly1(xt, yt, r + 1, p, base, tw, tw_h);
			butterFly1(xt, yt, r + 2, p, base, tw, tw_h);
			butterFly1(xt, yt, r + 3, p, base, tw, tw_h);
        	}

        	base = -1 << (c - 2);
        	INNER_LOOP2: for (PARAMS_TYPE r = 0; r < mid; r += 4){
			DATA_TYPE tw = twiddle[base & r];
			DATA_TYPE tw_h = twiddle_h[base & r];
			butterFly1(yt, xt, r + 0, p, base, tw, tw_h);
			butterFly1(yt, xt, r + 1, p, base, tw, tw_h);
			butterFly1(yt, xt, r + 2, p, base, tw, tw_h);
			butterFly1(yt, xt, r + 3, p, base, tw, tw_h);
        	}

    	}
	STAGE_LOOP1: for (; c >= 2; c -= 2){
		PARAMS_TYPE base = -1 << (c - 1);
        	INNER_LOOP3: for (PARAMS_TYPE r = 0; r < mid; r += 4){
			butterFly2(xt, yt, r + 0, p, base, twiddle, twiddle_h);
			butterFly2(xt, yt, r + 1, p, base, twiddle, twiddle_h);
			butterFly2(xt, yt, r + 2, p, base, twiddle, twiddle_h);
			butterFly2(xt, yt, r + 3, p, base, twiddle, twiddle_h);
        	}

        	base = -1 << (c - 2);
        	INNER_LOOP4: for (PARAMS_TYPE r = 0; r < mid; r += 4){
			butterFly2(yt, xt, r + 0, p, base, twiddle, twiddle_h);
			butterFly2(yt, xt, r + 1, p, base, twiddle, twiddle_h);
			butterFly2(yt, xt, r + 2, p, base, twiddle, twiddle_h);
			butterFly2(yt, xt, r + 3, p, base, twiddle, twiddle_h);
        	}
	}
}
*/


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
/*
inline DATA_TYPE modulo_sub(DATA_TYPE_SIGNED base, DATA_TYPE m){
	return base < 0? base + m : base;
}
*/
inline DATA_TYPE modulo_sub(DATA_TYPE_SIGNED base, DATA_TYPE_SIGNED m){
    DATA_TYPE_SIGNED res = base % m;
	return res < 0? base + m : base;
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
void butterFly(DATA_TYPE xt[HALF_VECTOR_SIZE], DATA_TYPE yt[HALF_VECTOR_SIZE], PARAMS_TYPE r, DATA_TYPE p, PARAMS_TYPE base, DATA_TYPE twiddle[HALF_VECTOR_SIZE], DATA_TYPE twiddle_h[HALF_VECTOR_SIZE]){
        DATA_TYPE tw = twiddle[base & r];
        DATA_TYPE tw_h = twiddle_h[base & r];
	    DATA_TYPE f1 = xt[r << 1];
	    DATA_TYPE f2 = mult(xt[(r << 1) + 1], tw, tw_h, p);
	    yt[r]       = modulo_add(f1 + f2, p);
	    yt[r + (HALF_VECTOR_SIZE >> 1)]  = modulo_sub(f1 - f2, p);
}

#include <iostream>

#pragma hls_design top
void hybrid(DATA_TYPE x[VECTOR_SIZE], DATA_TYPE m, DATA_TYPE twiddle[HALF_VECTOR_SIZE], DATA_TYPE twiddle_h[HALF_VECTOR_SIZE], PARAMS_TYPE revArr[HALF_VECTOR_SIZE],  DATA_TYPE tw[VECTOR_SIZE], DATA_TYPE tw_h[VECTOR_SIZE]){

    DATA_TYPE xx[HALF_VECTOR_SIZE][HALF_VECTOR_SIZE];
    DATA_TYPE yy[HALF_VECTOR_SIZE][HALF_VECTOR_SIZE];

    // Step1: transpose x

    S1_OUTER_LOOP:for (PARAMS_TYPE k = 0; k < HALF_VECTOR_SIZE; k++) { // transpose x
        for (PARAMS_TYPE p = k; p < HALF_VECTOR_SIZE; p++) {
            yy[k][p] = x[k + p * HALF_VECTOR_SIZE];
            yy[p][k] = x[p + k * HALF_VECTOR_SIZE];
        }
    }


    S2_COPY_LOOP:for (PARAMS_TYPE p = 0; p < HALF_VECTOR_SIZE; p++) // FFT all p-line of x
    {
        for(PARAMS_TYPE i=0; i < HALF_VECTOR_SIZE; i++){
            xx[p][revArr[i]] = yy[p][i];
        }
    }


    const PARAMS_TYPE mid = HALF_VECTOR_SIZE >> 1;
    // Step2: FFT all p1-line of a by Fm
    S2_OUTER_LOOP: 
    for (PARAMS_TYPE c = HALF_VECTOR_ADDR_BIT; c >= 2; c -= 2){

        	PARAMS_TYPE base = -1 << (c - 1);

        	S2_INNER_LOOP1: for (PARAMS_TYPE r = 0; r < mid; r += 4){
                for(PARAMS_TYPE p = 0; p < HALF_VECTOR_SIZE; p++){
                    butterFly(xx[p], yy[p], r + 0, m, base, twiddle, twiddle_h);
                    butterFly(xx[p], yy[p], r + 1, m, base, twiddle, twiddle_h);
                    butterFly(xx[p], yy[p], r + 2, m, base, twiddle, twiddle_h);
                    butterFly(xx[p], yy[p], r + 3, m, base, twiddle, twiddle_h);
                }
        	}

        	base = -1 << (c - 2);
        	S2_INNER_LOOP2: for (PARAMS_TYPE r = 0; r < mid; r += 4){
                for(PARAMS_TYPE p = 0; p < HALF_VECTOR_SIZE; p++){
                    butterFly(yy[p], xx[p], r + 0, m, base, twiddle, twiddle_h);
                    butterFly(yy[p], xx[p], r + 1, m, base, twiddle, twiddle_h);
                    butterFly(yy[p], xx[p], r + 2, m, base, twiddle, twiddle_h);
                    butterFly(yy[p], xx[p], r + 3, m, base, twiddle, twiddle_h);
                }
        	}

    }
    if(HALF_VECTOR_ADDR_BIT % 2){
        	PARAMS_TYPE base = -1;
        	S2_INNER_LOOP3: for (PARAMS_TYPE r = 0; r < mid; r += 4){
                for(PARAMS_TYPE p = 0; p < HALF_VECTOR_SIZE; p++){
                    butterFly(xx[p], yy[p], r + 0, m, base, twiddle, twiddle_h);
                    butterFly(xx[p], yy[p], r + 1, m, base, twiddle, twiddle_h);
                    butterFly(xx[p], yy[p], r + 2, m, base, twiddle, twiddle_h);
                    butterFly(xx[p], yy[p], r + 3, m, base, twiddle, twiddle_h);
                }
            }
    }


    // Step3-4: multiply twiddle factor and transpose x
    S34_OUTER_LOOP:for (PARAMS_TYPE p = 0; p < HALF_VECTOR_SIZE; p++) { 
        //const uint64_t theta0 = modExp(g, (m - 1) / VECTOR_SIZE * p, m);
        //uint64_t wkp = modExp(theta0, p, m);
        for (PARAMS_TYPE k = p; k < HALF_VECTOR_SIZE; k++) {
            PARAMS_TYPE tf = tw[p * k];
            PARAMS_TYPE tf_h = tw_h[p * k];
            DATA_TYPE a = mult(yy[p][k], tf, tf_h, m);
            DATA_TYPE b = mult(yy[k][p], tf, tf_h, m);
            xx[p][k] = b;
            xx[k][p] = a;
            //wkp = modulo_dev(wkp * theta0, m);
        }
    }


    // Step5: FFT all k1-line of d by Fn
    S5_COPY_LOOP:for (PARAMS_TYPE p = 0; p < HALF_VECTOR_SIZE; p++) // FFT all p-line of x
    {
        for(PARAMS_TYPE i=0; i < HALF_VECTOR_SIZE; i++){
            yy[p][revArr[i]] = xx[p][i];
        }
    }

    S5_OUTER_LOOP: 
    for (PARAMS_TYPE c = HALF_VECTOR_ADDR_BIT; c >= 2; c -= 2){

        	PARAMS_TYPE base = -1 << (c - 1);

        	S5_INNER_LOOP1: for (PARAMS_TYPE r = 0; r < mid; r += 4){
                for(PARAMS_TYPE p = 0; p < HALF_VECTOR_SIZE; p++){
                    butterFly(yy[p], xx[p], r + 0, m, base, twiddle, twiddle_h);
                    butterFly(yy[p], xx[p], r + 1, m, base, twiddle, twiddle_h);
                    butterFly(yy[p], xx[p], r + 2, m, base, twiddle, twiddle_h);
                    butterFly(yy[p], xx[p], r + 3, m, base, twiddle, twiddle_h);
                }
        	}

        	base = -1 << (c - 2);
        	S5_INNER_LOOP2: for (PARAMS_TYPE r = 0; r < mid; r += 4){
                for(PARAMS_TYPE p = 0; p < HALF_VECTOR_SIZE; p++){
                    butterFly(xx[p], yy[p], r + 0, m, base, twiddle, twiddle_h);
                    butterFly(xx[p], yy[p], r + 1, m, base, twiddle, twiddle_h);
                    butterFly(xx[p], yy[p], r + 2, m, base, twiddle, twiddle_h);
                    butterFly(xx[p], yy[p], r + 3, m, base, twiddle, twiddle_h);
                }
        	}

    }
    if(HALF_VECTOR_ADDR_BIT % 2){
        	PARAMS_TYPE base = -1;
        	S5_INNER_LOOP3: for (PARAMS_TYPE r = 0; r < mid; r += 4){
                for(PARAMS_TYPE p = 0; p < HALF_VECTOR_SIZE; p++){
                    butterFly(yy[p], xx[p], r + 0, m, base, twiddle, twiddle_h);
                    butterFly(yy[p], xx[p], r + 1, m, base, twiddle, twiddle_h);
                    butterFly(yy[p], xx[p], r + 2, m, base, twiddle, twiddle_h);
                    butterFly(yy[p], xx[p], r + 3, m, base, twiddle, twiddle_h);
                }
            }
    }

    // Step6: transpose x
    S6_OUTER_LOOP:for (PARAMS_TYPE k = 0; k < HALF_VECTOR_SIZE; k++) {
        for (PARAMS_TYPE p = k; p < HALF_VECTOR_SIZE; p++) {
            x[p + k * HALF_VECTOR_SIZE] = xx[p][k];
            x[k + p * HALF_VECTOR_SIZE] = xx[k][p];
        }
    }

}
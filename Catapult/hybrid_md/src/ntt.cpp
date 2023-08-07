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
#pragma hls_design inline
/*
inline DATA_TYPE modulo_sub(DATA_TYPE_SIGNED base, DATA_TYPE m){
	return base < 0? base + m : base;
}
*/
inline DATA_TYPE modulo_sub(DATA_TYPE_SIGNED base, DATA_TYPE_SIGNED m){
	return base < 0? (DATA_TYPE)(base + m) : (DATA_TYPE)base;
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
void butterFly(DATA_TYPE xt[HALF_VECTOR_SIZE], DATA_TYPE yt[HALF_VECTOR_SIZE], PARAMS_TYPE r, DATA_TYPE p, PARAMS_TYPE base, DATA_TYPE tw, DATA_TYPE tw_h){
	    DATA_TYPE f1 = xt[r << 1];
	    DATA_TYPE f2 = mult(xt[(r << 1) + 1], tw, tw_h, p);
	    yt[r]       = modulo_add(f1 + f2, p);
	    yt[r + (HALF_VECTOR_SIZE >> 1)]  = modulo_sub(f1 - f2, p);
}


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


    S2_REVERSE_LOOP: for(PARAMS_TYPE i=0; i < HALF_VECTOR_SIZE; i++){
        PARAMS_TYPE revIdx = revArr[i];
        for (PARAMS_TYPE p = 0; p < HALF_VECTOR_SIZE; p++) // FFT all p-line of x
        {
            xx[p][revIdx] = yy[p][i];
        }
    }


    const PARAMS_TYPE mid = HALF_VECTOR_SIZE >> 1;
    // Step2: FFT all p1-line of a by Fm
    S2_OUTER_LOOP: 
    for (PARAMS_TYPE c = HALF_VECTOR_ADDR_BIT; c >= 1; c -= 1){

        	PARAMS_TYPE base = -1 << (c - 1);

        	S2_INNER_LOOP: for (PARAMS_TYPE r = 0; r < mid; r++){
                DATA_TYPE tf = twiddle[r & base];
                DATA_TYPE tfh= twiddle_h[r & base];
                for(PARAMS_TYPE p = 0; p < HALF_VECTOR_SIZE; p++){
                    butterFly(xx[p+1], yy[p+1], r, m, base, tf, tfh);
                }
        	}

            S2_CPY_LOOP:for(PARAMS_TYPE q = 0; q < HALF_VECTOR_SIZE; q++){
                for(PARAMS_TYPE p = 0; p < HALF_VECTOR_SIZE; p++){
                    yy[p][q] = xx[p][q];
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
            DATA_TYPE a = mult(xx[p][k], tf, tf_h, m);
            DATA_TYPE b = mult(xx[k][p], tf, tf_h, m);
            yy[p][k] = b;
            yy[k][p] = a;
            //wkp = modulo_dev(wkp * theta0, m);
        }
    }


    // Step5: FFT all k1-line of d by Fn
    S5_REVERSE_LOOP: for(PARAMS_TYPE i=0; i < HALF_VECTOR_SIZE; i++){
        PARAMS_TYPE revIdx = revArr[i];
        for (PARAMS_TYPE p = 0; p < HALF_VECTOR_SIZE; p++) // FFT all p-line of x
        {
            xx[p][revIdx] = yy[p][i];
        }
    }

    S5_OUTER_LOOP: 
    for (PARAMS_TYPE c = HALF_VECTOR_ADDR_BIT; c >= 1; c -= 1){

        	PARAMS_TYPE base = -1 << (c - 1);

        	S5_INNER_LOOP: for (PARAMS_TYPE r = 0; r < mid; r++){
                DATA_TYPE tf = twiddle[r & base];
                DATA_TYPE tfh= twiddle_h[r & base];
                for(PARAMS_TYPE p = 0; p < HALF_VECTOR_SIZE; p++){
                    butterFly(xx[p], yy[p], r, m, base, tf, tfh);
                }
        	}

            S5_CPY_LOOP:for(PARAMS_TYPE q = 0; q < HALF_VECTOR_SIZE; q++){
                for(PARAMS_TYPE p = 0; p < HALF_VECTOR_SIZE; p++){
                    yy[p][q] = xx[p][q];
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
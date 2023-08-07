#include "../include/ntt.h"

#pragma hls_design inline
inline DATA_TYPE modulo_add(DATA_TYPE base, DATA_TYPE m){
	return base > m? (DATA_TYPE)(base - m) : (DATA_TYPE)base;

}

#pragma hls_design inline
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
void butterFly(DATA_TYPE *vec, PARAMS_TYPE idx1, PARAMS_TYPE idx2, DATA_TYPE tw, DATA_TYPE tw_h, DATA_TYPE p){
    DATA_TYPE f1 = vec[idx1];
    DATA_TYPE f2 = vec[idx2];
    vec[idx1] = modulo_add(f1 + f2, p);

    vec[idx2] = mult(modulo_sub(f1 - f2, p), tw, tw_h, p);//%mult(modulo_sub(f1 - f2, p), tw, tw_h, p);
    //std::cout<<vec[idx1]<<std::endl;
}

#pragma hls_design top
void six_step(DATA_TYPE x[VECTOR_SIZE], DATA_TYPE m, DATA_TYPE twiddle[HALF_VECTOR_SIZE], DATA_TYPE twiddle_h[HALF_VECTOR_SIZE], PARAMS_TYPE revArr[HALF_VECTOR_SIZE],  DATA_TYPE tw[VECTOR_SIZE], DATA_TYPE tw_h[VECTOR_SIZE]){

    DATA_TYPE xx[HALF_VECTOR_SIZE][HALF_VECTOR_SIZE];
    DATA_TYPE yy[HALF_VECTOR_SIZE][HALF_VECTOR_SIZE];

    // Step1: transpose x

    S1_OUTER_LOOP:for (PARAMS_TYPE k = 0; k < HALF_VECTOR_SIZE; k++) { // transpose x
        for (PARAMS_TYPE p = k; p < HALF_VECTOR_SIZE; p++) {
            xx[k][p] = x[k + p * HALF_VECTOR_SIZE];
            xx[p][k] = x[p + k * HALF_VECTOR_SIZE];
        }
    }


    const PARAMS_TYPE mid = HALF_VECTOR_SIZE >> 1;
    // Step2: FFT all p1-line of a by Fm
    S2_OUTER_LOOP: 
	for(PARAMS_TYPE i = 1; i <= HALF_VECTOR_ADDR_BIT; i++){
		PARAMS_TYPE shift_idx = HALF_VECTOR_ADDR_BIT - i;
		PARAMS_TYPE gp = 1 << (i - 1);
		PARAMS_TYPE op =  HALF_VECTOR_SIZE >> i;
		PARAMS_TYPE point = op << 1;
		S2_INNER_LOOP: for(PARAMS_TYPE j=0; j < mid; j++){
			PARAMS_TYPE g = j >> shift_idx;
			PARAMS_TYPE k = j - (g << shift_idx);
			PARAMS_TYPE idx1= point * g + k;
			PARAMS_TYPE idx2= idx1 + op;
			DATA_TYPE tf = twiddle[gp * k];
			DATA_TYPE tf_h = twiddle_h[gp * k];
			//cout<<idx1<<" "<<idx2<<" "<<gp*t<<endl;
			S2_GROUP_LOOP:for(PARAMS_TYPE p = 0; p < HALF_VECTOR_SIZE; p++){
				butterFly(xx[p], idx1, idx2, tf, tf_h, m);
			}
		}
	}

    S2_COPY_LOOP:for (PARAMS_TYPE p = 0; p < HALF_VECTOR_SIZE; p++) // FFT all p-line of x
    {
        for(PARAMS_TYPE i=0; i < HALF_VECTOR_SIZE; i++){
            yy[p][revArr[i]] = xx[p][i];
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
    S5_OUTER_LOOP: 
	for(PARAMS_TYPE i = 1; i <= HALF_VECTOR_ADDR_BIT; i++){
		PARAMS_TYPE shift_idx = HALF_VECTOR_ADDR_BIT - i;
		PARAMS_TYPE gp = 1 << (i - 1);
		PARAMS_TYPE op =  HALF_VECTOR_SIZE >> i;
		PARAMS_TYPE point = op << 1;
		S5_INNER_LOOP: for(PARAMS_TYPE j=0; j < mid; j++){
			PARAMS_TYPE g = j >> shift_idx;
			PARAMS_TYPE k = j - (g << shift_idx);
			PARAMS_TYPE idx1= point * g + k;
			PARAMS_TYPE idx2= idx1 + op;
			DATA_TYPE tf = twiddle[gp * k];
			DATA_TYPE tf_h = twiddle_h[gp * k];
			//cout<<idx1<<" "<<idx2<<" "<<gp*t<<endl;
			S5_GROUP_LOOP:for(PARAMS_TYPE p = 0; p < HALF_VECTOR_SIZE; p++){
				butterFly(xx[p], idx1, idx2, tf, tf_h, m);
			}
		}
	}


    // Bit Reverse
    S5_COPY_LOOP: for (PARAMS_TYPE p = 0; p < HALF_VECTOR_SIZE; p++){
        for(PARAMS_TYPE i=0; i < HALF_VECTOR_SIZE; i++){
            yy[p][revArr[i]] = xx[p][i];
        }

    }

    // Step6: transpose x
    S6_OUTER_LOOP:for (PARAMS_TYPE k = 0; k < HALF_VECTOR_SIZE; k++) {
        for (PARAMS_TYPE p = k; p < HALF_VECTOR_SIZE; p++) {
            x[p + k * HALF_VECTOR_SIZE] = yy[p][k];
            x[k + p * HALF_VECTOR_SIZE] = yy[k][p];
        }
    }

}

#include "../include/ntt.h"

#pragma hls_design inline
UINT64_T modulo_dev(INT64_T base, INT64_T m){

	INT64_T result = base % m;
	
	return result >= 0? (UINT64_T) result : (UINT64_T) (result + m);

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
UINT64_T modExp(UINT64_T base, UINT64_T exp, UINT64_T m){

	UINT64_T result = 1;
	
	while(exp > 0){

		if(exp % 2){

			result = (result * base) % m;

		}

		exp = exp >> 1;
		base = (base * base) % m;
	}

	return result;
}

#pragma hls_design inline
void butterFly(UINT64_T *vec, UINT64_T *res, unsigned r, unsigned mid, UINT64_T twiddle, UINT64_T m){
    UINT64_T f11 = vec[r << 1];
    UINT64_T f12 = (twiddle * vec[(r << 1) + 1]) % m;
    res[r]    = (f11 + f12) % m;  
    res[r + mid] = modulo_dev(f11 - f12, m);
}


#pragma hls_design top
void six_step(VEC_T x[VECTOR_SIZE], UINT64_T m, UINT64_T g, VEC_T twiddle[HALF_VECTOR_SIZE], unsigned revArr[HALF_VECTOR_SIZE],  VEC_T tw[VECTOR_SIZE]){

    VEC_T xx[HALF_VECTOR_SIZE][HALF_VECTOR_SIZE];
    VEC_T yy[HALF_VECTOR_SIZE][HALF_VECTOR_SIZE];

    // Step1: transpose x
    S1_OUTER_LOOP:for (int k = 0; k < HALF_VECTOR_SIZE; k++) { // transpose x
        for (int p = k; p < HALF_VECTOR_SIZE; p++) {
            xx[k][p] = x[k + p * HALF_VECTOR_SIZE];
            xx[p][k] = x[p + k * HALF_VECTOR_SIZE];
        }
    }

    // Step2: FFT all p-line of d by Fn
    S2_REVERSE_LOOP:for(int i=0; i < HALF_VECTOR_SIZE; i++){
        unsigned revIdx = revArr[i];

        for (int p = 0; p < HALF_VECTOR_SIZE; p+=4) // FFT all p-line of x
        {
            yy[p][i] = xx[p][revIdx];
            yy[p+1][i] = xx[p+1][revIdx];
            yy[p+2][i] = xx[p+2][revIdx];
            yy[p+3][i] = xx[p+3][revIdx];
        }
    }

    const unsigned mid = HALF_VECTOR_SIZE >> 1;
    S2_OUTER_LOOP:
    for(int c=HALF_VECTOR_ADDR_BIT; c>=2; c-=2){
            unsigned base = -1 << (c - 1);
            for(unsigned r=0; r < mid; r++){
                UINT64_T lst= twiddle[r & base];
                for(int p=0; p < HALF_VECTOR_SIZE; p+=8){
                    butterFly(yy[p], xx[p], r, mid, lst, m);
                    butterFly(yy[p+1], xx[p+1], r, mid, lst, m);
                    butterFly(yy[p+2], xx[p+2], r, mid, lst, m);
                    butterFly(yy[p+3], xx[p+3], r, mid, lst, m);
                    butterFly(yy[p+4], xx[p+4], r, mid, lst, m);
                    butterFly(yy[p+5], xx[p+5], r, mid, lst, m);
                    butterFly(yy[p+6], xx[p+6], r, mid, lst, m);
                    butterFly(yy[p+7], xx[p+7], r, mid, lst, m);
                }
            }

            base = -1 << (c - 2);
            for(unsigned r=0; r < mid; r++){
                UINT64_T lst= twiddle[r & base];
                for(int p=0; p < HALF_VECTOR_SIZE; p+=8){
                    butterFly(xx[p], yy[p], r, mid, lst, m);
                    butterFly(xx[p+1], yy[p+1], r, mid, lst, m);
                    butterFly(xx[p+2], yy[p+2], r, mid, lst, m);
                    butterFly(xx[p+3], yy[p+3], r, mid, lst, m);
                    butterFly(xx[p+4], yy[p+4], r, mid, lst, m);
                    butterFly(xx[p+5], yy[p+5], r, mid, lst, m);
                    butterFly(xx[p+6], yy[p+6], r, mid, lst, m);
                    butterFly(xx[p+7], yy[p+7], r, mid, lst, m);
                }
            }
    }

#if (HALF_VECTOR_ADDR_BIT % 2) == 1
    S2_OUTER_LOOP_EXTRA: for(unsigned r=0; r < mid; r++){
        UINT64_T lst= twiddle[r];
        for(int p=0; p < HALF_VECTOR_SIZE; p+=4){
                    butterFly(yy[p], xx[p], r, mid, lst, m);
                    butterFly(yy[p+1], xx[p+1], r, mid, lst, m);
                    butterFly(yy[p+2], xx[p+2], r, mid, lst, m);
                    butterFly(yy[p+3], xx[p+3], r, mid, lst, m);
                    butterFly(yy[p+4], xx[p+4], r, mid, lst, m);
                    butterFly(yy[p+5], xx[p+5], r, mid, lst, m);
                    butterFly(yy[p+6], xx[p+6], r, mid, lst, m);
                    butterFly(yy[p+7], xx[p+7], r, mid, lst, m);
        }
    }
#endif

    // Step3-4: multiply twiddle factor and transpose x
    S34_OUTER_LOOP:for (int p = 0; p < HALF_VECTOR_SIZE; p++) { 
        //const uint64_t theta0 = modExp(g, (m - 1) / VECTOR_SIZE * p, m);
        //uint64_t wkp = modExp(theta0, p, m);
        for (int k = p + 1; k < HALF_VECTOR_SIZE; k++) {
            UINT64_T wkp = tw[p * k];
#if (HALF_VECTOR_ADDR_BIT % 2) == 1
            const UINT64_T a = (xx[p][k] * wkp) % m;
            const UINT64_T b = (xx[k][p] * wkp) % m;
#else
            const UINT64_T a = (yy[p][k] * wkp) % m;
            const UINT64_T b = (yy[k][p] * wkp) % m;
#endif
            xx[p][k] = b;
            xx[k][p] = a;
            //wkp = modulo_dev(wkp * theta0, m);
        }
    }
    S34_OUTER_LOOP_EXTRA:for (int p = 0; p < HALF_VECTOR_SIZE; p++) { 
            UINT64_T wkp = tw[p * p];
#if (HALF_VECTOR_ADDR_BIT % 2) == 1
            const UINT64_T a = (xx[p][p] * wkp) % m;
#else
            const UINT64_T a = (yy[p][p] * wkp) % m;
#endif
            xx[p][p] = a;
    }


    // Step5: FFT all p-line of x
    S5_REVERSE_LOOP:for(int i=0; i < HALF_VECTOR_SIZE; i++){
        unsigned revIdx = revArr[i];

        for (int p = 0; p < HALF_VECTOR_SIZE; p+=4) // FFT all p-line of x
        {
            yy[p][i] = xx[p][revIdx];
            yy[p+1][i] = xx[p+1][revIdx];
            yy[p+2][i] = xx[p+2][revIdx];
            yy[p+3][i] = xx[p+3][revIdx];
        }
    }


    S5_OUTER_LOOP:
    for(int c=HALF_VECTOR_ADDR_BIT; c>=2; c-=2){
            unsigned base = -1 << (c - 1);
            for(unsigned r=0; r < mid; r++){
                UINT64_T lst= twiddle[r & base];
                for(int p=0; p < HALF_VECTOR_SIZE; p+=8){
                    butterFly(yy[p], xx[p], r, mid, lst, m);
                    butterFly(yy[p+1], xx[p+1], r, mid, lst, m);
                    butterFly(yy[p+2], xx[p+2], r, mid, lst, m);
                    butterFly(yy[p+3], xx[p+3], r, mid, lst, m);
                    butterFly(yy[p+4], xx[p+4], r, mid, lst, m);
                    butterFly(yy[p+5], xx[p+5], r, mid, lst, m);
                    butterFly(yy[p+6], xx[p+6], r, mid, lst, m);
                    butterFly(yy[p+7], xx[p+7], r, mid, lst, m);
                }
            }

            base = -1 << (c - 2);
            for(unsigned r=0; r < mid; r++){
                UINT64_T lst= twiddle[r & base];
                for(int p=0; p < HALF_VECTOR_SIZE; p+=8){
                    butterFly(xx[p], yy[p], r, mid, lst, m);
                    butterFly(xx[p+1], yy[p+1], r, mid, lst, m);
                    butterFly(xx[p+2], yy[p+2], r, mid, lst, m);
                    butterFly(xx[p+3], yy[p+3], r, mid, lst, m);
                    butterFly(xx[p+4], yy[p+4], r, mid, lst, m);
                    butterFly(xx[p+5], yy[p+5], r, mid, lst, m);
                    butterFly(xx[p+6], yy[p+6], r, mid, lst, m);
                    butterFly(xx[p+7], yy[p+7], r, mid, lst, m);
                }
            }
    }

#if (HALF_VECTOR_ADDR_BIT % 2) == 1
    S5_OUTER_LOOP_EXTRA: for(unsigned r=0; r < mid; r++){
        UINT64_T lst= twiddle[r];
        for(int p=0; p < HALF_VECTOR_SIZE; p+=8){
                    butterFly(yy[p], xx[p], r, mid, lst, m);
                    butterFly(yy[p+1], xx[p+1], r, mid, lst, m);
                    butterFly(yy[p+2], xx[p+2], r, mid, lst, m);
                    butterFly(yy[p+3], xx[p+3], r, mid, lst, m);
                    butterFly(yy[p+4], xx[p+4], r, mid, lst, m);
                    butterFly(yy[p+5], xx[p+5], r, mid, lst, m);
                    butterFly(yy[p+6], xx[p+6], r, mid, lst, m);
                    butterFly(yy[p+7], xx[p+7], r, mid, lst, m);
        }
    }
#endif

    // Step6: transpose x
    S6_OUTER_LOOP:for (int k = 0; k < HALF_VECTOR_SIZE; k++) {
        for (int p = k; p < HALF_VECTOR_SIZE; p++) {
#if (HALF_VECTOR_ADDR_BIT % 2) == 1
            x[p + k * HALF_VECTOR_SIZE] = xx[p][k];
            x[k + p * HALF_VECTOR_SIZE] = xx[k][p];
#else
            x[p + k * HALF_VECTOR_SIZE] = yy[p][k];
            x[k + p * HALF_VECTOR_SIZE] = yy[k][p];
#endif
        }
    }


}
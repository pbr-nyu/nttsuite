
#include "../include/ntt.h"

#pragma hls_design inline
UINT64_T modulo_dev(INT64_T base, UINT64_T m){

	INT64_T result = base % m;
	
	return result >= 0? (UINT64_T) result : (UINT64_T) (result + m);

}

UINT64_T modulo_dev_func(INT64_T base, UINT64_T m){

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
void butterFly(UINT64_T *vec, int idx1, int idx2, UINT64_T twiddle, UINT64_T p){
    UINT64_T f1 = vec[idx1];
    UINT64_T f2 = vec[idx2];
    vec[idx1] = modulo_dev(f1 + f2, p);
    vec[idx2] = modulo_dev(twiddle * modulo_dev(f1 - f2, p), p);
}


#pragma hls_design ccore
template <int ID>
void BF(VEC_T vec[HALF_VECTOR_SIZE], unsigned idx1, unsigned idx2, UINT64_T twiddle, INT64_T p){
    UINT64_T f1 = vec[idx1];
    UINT64_T f2 = vec[idx2];
    vec[idx1] = modulo_dev_func(f1 + f2, p);
    vec[idx2] = modulo_dev_func(twiddle * modulo_dev_func(f1 - f2, p), p);
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


    const unsigned mid = HALF_VECTOR_SIZE >> 1;
    // Step2: FFT all p1-line of a by Fm
    S2_OUTER_LOOP: 
    for(unsigned i = 1; i <= HALF_VECTOR_ADDR_BIT; i++)
    {
            unsigned shift_idx = HALF_VECTOR_ADDR_BIT - i;
            unsigned gp = 1 << (i - 1);
            unsigned op =  HALF_VECTOR_SIZE >> i;
            unsigned point = op << 1;
            for(unsigned j=0; j < mid; j++){
                unsigned gg = j >> shift_idx;
                unsigned kk = j - (gg << shift_idx);
                unsigned idx1= point * gg + kk;
                unsigned idx2= idx1 + op;
                const UINT64_T tf = twiddle[gp * kk];
                for (unsigned p = 0; p < HALF_VECTOR_SIZE; p+=1){
                    butterFly(xx[p], idx1, idx2, m, tf);
                    //butterFly(xx[p+1], idx1, idx2, m, tf);
                    //butterFly(xx[p+1], idx1, idx2, tf, m);

                    //const UINT64_T f1 = xx[p][idx1];
                    //const UINT64_T f2 = xx[p][idx2];
                    /*
                    const UINT64_T f11 = xx[p+1][idx1];
                    const UINT64_T f12 = xx[p+1][idx2];
                    const UINT64_T f21 = xx[p+2][idx1];
                    const UINT64_T f22 = xx[p+2][idx2];
                    const UINT64_T f31 = xx[p+3][idx1];
                    const UINT64_T f32 = xx[p+3][idx2];
                    */
                    //xx[p][idx1] = modulo_dev(f1 + f2, m);
                    //xx[p][idx2] = modulo_dev(tf * modulo_dev(f1 - f2, m), m);
                    /*
                    xx[p+1][idx1] = modulo_dev(f11 + f12, m);
                    xx[p+1][idx2] = modulo_dev(tf * modulo_dev(f11 - f12, m), m);
                    xx[p+2][idx1] = modulo_dev(f21 + f22, m);
                    xx[p+2][idx2] = modulo_dev(tf * modulo_dev(f21 - f22, m), m);
                    xx[p+3][idx1] = modulo_dev(f31 + f32, m);
                    xx[p+3][idx2] = modulo_dev(tf * modulo_dev(f31 - f32, m), m);
                    */
                }

            }
    }

        // Bit Reverse
    S2_COPY_LOOP:for (int p = 0; p < HALF_VECTOR_SIZE; p++) // FFT all p-line of x
    {
        for(int i=0; i < HALF_VECTOR_SIZE; i++){
            yy[p][revArr[i]] = xx[p][i];
        }

    }


    // Step3-4: multiply twiddle factor and transpose x
    S34_OUTER_LOOP:for (int p = 0; p < HALF_VECTOR_SIZE; p++) { 
        //const uint64_t theta0 = modExp(g, (m - 1) / VECTOR_SIZE * p, m);
        //uint64_t wkp = modExp(theta0, p, m);
        for (int k = p; k < HALF_VECTOR_SIZE; k++) {
            uint64_t wkp = tw[p * k];
            const uint64_t a = (yy[p][k] * wkp) % m;
            const uint64_t b = (yy[k][p] * wkp) % m;
            xx[p][k] = b;
            xx[k][p] = a;
            //wkp = modulo_dev(wkp * theta0, m);
        }
    }


    // Step5: FFT all k1-line of d by Fn
    S5_OUTER_LOOP: for(unsigned i = 1; i <= HALF_VECTOR_ADDR_BIT; i++)
    {

            unsigned shift_idx = HALF_VECTOR_ADDR_BIT - i;
            unsigned gp = 1 << (i - 1);
            unsigned op =  HALF_VECTOR_SIZE >> i;
            unsigned point = op << 1;
            for(unsigned j=0; j < mid; j++){
                unsigned gg = j >> shift_idx;
                unsigned kk = j - (gg << shift_idx);
                unsigned idx1= point * gg + kk;
                unsigned idx2= idx1 + op;
                const UINT64_T tf = twiddle[gp * kk];
                for (unsigned p = 0; p < HALF_VECTOR_SIZE; p++){
                    const UINT64_T f1 = xx[p][idx1];
                    const UINT64_T f2 = xx[p][idx2];
                    xx[p][idx1] = modulo_dev(f1 + f2, m);
                    xx[p][idx2] = modulo_dev(tf * modulo_dev(f1 - f2, m), m);
                }
            }
    }


    // Bit Reverse
    S5_COPY_LOOP: for (int p = 0; p < HALF_VECTOR_SIZE; p++){
        for(int i=0; i < HALF_VECTOR_SIZE; i++){
            yy[p][revArr[i]] = xx[p][i];
        }

    }



    // Step6: transpose x
    S6_OUTER_LOOP:for (int k = 0; k < HALF_VECTOR_SIZE; k++) {
        for (int p = k; p < HALF_VECTOR_SIZE; p++) {
            x[p + k * HALF_VECTOR_SIZE] = yy[p][k];
            x[k + p * HALF_VECTOR_SIZE] = yy[k][p];
        }
    }

}
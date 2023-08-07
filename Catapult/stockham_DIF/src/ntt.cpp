
#include "../include/ntt.h"

#pragma hls_design ccore
UINT64_T modulo_dev(ac_int<64, true> base, UINT64_T m){
	ac_int<64, true> q, result;
	result = base % m;
	return (result >= 0) ? (UINT64_T)result : (UINT64_T)(result + m);
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
UINT64_T modExp_dev(UINT64_T base, UINT64_T exp, UINT64_T m){
	UINT64_T result = 1, q = 1;
	while(exp > 0){
        result = ((UINT64_T)(result * base) % m);
        exp--;
    }
	return result;
}
#pragma hls_design top
void DIT_RELOOP(VEC_T vec[VECTOR_SIZE], UINT64_T p, UINT64_T r, VEC_T twiddle[VECTOR_SIZE]){
	/*COPY_LOOP:for(unsigned i = 0; i < VECTOR_SIZE; i++){
		result[i] = vec[i];
	}*/
	STAGE_LOOP: for(unsigned i = 1; i <= VECTOR_ADDR_BIT; i++){
		unsigned gp = 1 << (i - 1);
		unsigned op =  VECTOR_SIZE >> i;
		unsigned point = op << 1;
		GROUP_LOOP: for(unsigned j=0; j < gp; j++){
			IDX_LOOP: for(unsigned t=0; t < op; t++){
				unsigned idx1= point * j + t;
				unsigned idx2= idx1 + op;
				//cout<<idx1<<" "<<idx2<<" "<<gp*t<<endl;
				//butterFly(result, idx1, idx2, tf, p);
				UINT64_T f1 = vec[idx1];
				UINT64_T f2 = vec[idx2];
				vec[idx2] = (twiddle[t << (i-1)] * modulo_dev(f1 - f2, p)) % p;
				vec[idx1] = modulo_dev(f1 + f2, p);		
			}
		}
    	}
	
	/*if(rev){
		return bit_reverse(result,n);
	}else{
		return result;
	}*/
}
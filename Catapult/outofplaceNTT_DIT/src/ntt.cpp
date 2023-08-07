#include "../include/ntt.h"
/**
 * Perform the operation 'base (mod m)'
 *
 * @param base	The base of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
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
#pragma hls_design inline
uint16_t getDivideIndex(uint16_t index){
    uint16_t result = index, base = VECTOR_SIZE_LOG2 / 2;
    for(unsigned i = 0; i < VECTOR_SIZE_LOG2; i++){
        if(result & 1){
            if(result < base)
                result = (result - 1) / 2;
            else
                result = (result - 1) / 2 + base;
        } else {
            if(result <= base)
                result = result / 2;
            else
                result = result / 2 + base;
        }
        base = base / 2;
    }
}
#pragma hls_design top
void outofplaceNTT_DIT(UINT64_T vec[VECTOR_SIZE], UINT64_T p, UINT64_T r){
    UINT64_T mem[VECTOR_SIZE_LOG2][VECTOR_SIZE];
    for(unsigned j = 0; j < VECTOR_SIZE/2; j++){
            mem[0][j] = vec[j * 2];
            mem[0][j + VECTOR_SIZE/2] = vec[j * 2 + 1];
    }
    for(unsigned i = 1; i < VECTOR_SIZE_LOG2; i++){
        for(unsigned j = 0; j < VECTOR_SIZE/2; j++){
            mem[i][j] = mem[i-1][j * 2];
            mem[i][j + VECTOR_SIZE/2] = mem[i-1][j * 2 + 1];
        }
    } 
}
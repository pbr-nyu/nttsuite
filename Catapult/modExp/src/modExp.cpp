#include "../include/modExp.h"
/**
 * Perform the operation 'base^exp (mod m)' using the memory-efficient method
 *
 * @param base	The base of the expression
 * @param exp	The exponent of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
#pragma hls_design top
void modExp(const UINT64_T base, const UINT64_T exp, const UINT64_T m, UINT64_T &result){
    UINT64_T exp_count = exp;
	result = 1;
	while(exp_count > 0){
        result = ((UINT64_T)(result * base) % m);
        exp_count--;
    }
	//return result;
}

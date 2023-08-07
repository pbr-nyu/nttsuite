#include "../include/modExp.h"
//#include <cstdint>
#include <iostream>
//#define __float128 float
#include <mc_scverify.h>
using namespace std;
/**
 * Perform the operation 'base^exp (mod m)' using the memory-efficient method
 *
 * @param base	The base of the expression
 * @param exp	The exponent of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
UINT64_T modExp_golden(UINT64_T base, UINT64_T exp, UINT64_T m){
	UINT64_T result = 1;
	while(exp > 0){
		if(exp % 2){
			result = ((UINT64_T)(result * base)) % m;
		}
		exp = exp >> 1;
		base = ((UINT64_T)(base * base)) % m;
	}
	return result;
}
CCS_MAIN(int argc, char **argv){
    UINT64_T base = 2, exp = 7, m = 5, result = 0;
    UINT64_T base_g = 2, exp_g = 7, m_g = 5, result_g = 0;
    CCS_DESIGN(modExp)(base, exp, m, result);
    result_g = modExp_golden(base_g, exp_g, m_g);
    cout<<"Result: "<<result<<" Golden: "<<result_g<<endl;
    CCS_RETURN(0);
}



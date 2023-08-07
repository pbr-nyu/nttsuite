#ifndef MODEXP_H_
#define MODEXP_H_

//#include <ac_fixed.h>
//#include <mgc_ac_math.h>
//#include <ac_math.h>
#include <ac_int.h>
//#define VECTOR_WIDTH 64
//typedef ac_int<VECTOR_WIDTH, false>   VEC_T;
typedef ac_int<64, false>   UINT64_T;

void modExp(UINT64_T, UINT64_T, UINT64_T, UINT64_T &);

#endif /* MODEXP_H_ */


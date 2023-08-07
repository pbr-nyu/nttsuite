#ifndef NTT_H_
#define NTT_H_

#include "config.h"
//#include <math/mgc_ac_math.h>
#include <cmath>


void peaceNTT(UINT64_T vec[VECTOR_SIZE], UINT64_T p, UINT64_T g, UINT64_T result[VECTOR_SIZE], UINT64_T twiddle[VECTOR_SIZE]);

#endif /* NTT_H_ */
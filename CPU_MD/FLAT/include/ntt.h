#ifndef NTT_H_
#define NTT_H_

#include "config.h"
#include "utils.h"
//#include <cmath>

void ntt_flat(DATA_TYPE vec[VECTOR_SIZE], DATA_TYPE p, DATA_TYPE r, DATA_TYPE twiddle[VECTOR_SIZE], DATA_TYPE twiddle_h[VECTOR_SIZE], DATA_TYPE result[VECTOR_ADDR_BIT + 1][VECTOR_SIZE]);
#endif /* NTT_H_ */
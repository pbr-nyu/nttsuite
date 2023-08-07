#ifndef CONFIG_H_
#define CONFIG_H_

#include <ac_int.h>

const unsigned VECTOR_SIZE = 1024;
const unsigned VECTOR_WIDTH = 64;
const unsigned VECTOR_ADDR_BIT = 10;
const unsigned PARAM_WIDTH = 64;
typedef ac_int<64, false>   UINT64_T;
typedef ac_int<VECTOR_WIDTH, false>   VEC_T;
typedef ac_int<PARAM_WIDTH, false>   PARAM_T;
typedef ac_int<VECTOR_ADDR_BIT, false>  VEC_INDEX_T;
//typedef ac_fixed<64,64, false> UFIXED64_T;
//typedef ac_fixed<8, 8, false> UFIXED8_T;

#endif /* CONFIG_H_ */
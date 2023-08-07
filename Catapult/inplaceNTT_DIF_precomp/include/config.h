#ifndef CONFIG_H_
#define CONFIG_H_

#include <ac_int.h>

const unsigned VECTOR_SIZE = 1024;
const unsigned VECTOR_WIDTH = 32;
const unsigned VECTOR_ADDR_BIT = 10;
const unsigned PARAM_WIDTH = 32;
typedef ac_int<VECTOR_WIDTH, false>   DATA_TYPE;
typedef ac_int<VECTOR_WIDTH * 2, false>   DATA_TYPE_TMP; //temporary result type after DATA_TYPE * DATA_TYPE
typedef ac_int<VECTOR_WIDTH, true>	DATA_TYPE_SIGNED;
typedef ac_int<VECTOR_WIDTH, false>   VEC_T;
typedef ac_int<PARAM_WIDTH, false>   PARAM_T;
typedef ac_int<VECTOR_ADDR_BIT, false>  VEC_INDEX_T;
//typedef ac_fixed<64,64, false> UFIXED64_T;
//typedef ac_fixed<8, 8, false> UFIXED8_T;

#endif /* CONFIG_H_ */
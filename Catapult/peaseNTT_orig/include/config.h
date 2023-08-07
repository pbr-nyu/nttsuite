#ifndef CONFIG_H_
#define CONFIG_H_

#include <cstdint>
#include <cmath>
#include <ac_int.h>

const unsigned VECTOR_SIZE = 1024;
const unsigned VECTOR_WIDTH = 64;
const unsigned VECTOR_ADDR_BIT = 10;
const unsigned PARAM_WIDTH = 64;
typedef ac_int<64, false>   UINT64_T;
typedef ac_int<64, true>    INT64_T;
//typedef ac_fixed<64,64, false> UFIXED64_T;
//typedef ac_fixed<8, 8, false> UFIXED8_T;

#endif /* CONFIG_H_ */
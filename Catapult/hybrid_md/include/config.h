#ifndef CONFIG_H_
#define CONFIG_H_

#include <inttypes.h>
#include <ac_int.h>

#define VECTOR_SIZE  4096
#define HALF_VECTOR_SIZE 64
#define VECTOR_ADDR_BIT  12
#define HALF_VECTOR_ADDR_BIT 6

#define PARAM_WIDTH  20
#define VECTOR_WIDTH 32
//typedef uint32_t DATA_TYPE;
//typedef uint64_t DATA_TYPE_TMP;
//typedef int32_t	DATA_TYPE_SIGNED;
//
//typedef uint32_t PARAMS_TYPE;
typedef ac_int<PARAM_WIDTH, true>  PARAMS_TYPE;
typedef ac_int<VECTOR_WIDTH, false> DATA_TYPE;
typedef ac_int<VECTOR_WIDTH, true> DATA_TYPE_SIGNED;
typedef ac_int<(VECTOR_WIDTH << 1), false> DATA_TYPE_TMP;
//typedef ac_fixed<64,64, false> UFIXED64_T;
//typedef ac_fixed<8, 8, false> UFIXED8_T;

#endif /* CONFIG_H_ */
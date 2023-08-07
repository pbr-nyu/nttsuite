#ifndef CONFIG_H_
#define CONFIG_H_

#include <ac_math.h>

#define VECTOR_SIZE  16384
#define HALF_VECTOR_SIZE 128
#define VECTOR_ADDR_BIT  14
#define HALF_VECTOR_ADDR_BIT 7
#define VECTOR_WIDTH 32
#define PARAM_WIDTH  20
//typedef uint32_t DATA_TYPE;
//typedef uint64_t DATA_TYPE_TMP;
//typedef int32_t	DATA_TYPE_SIGNED;
typedef ac_int<VECTOR_WIDTH, false> DATA_TYPE;
typedef ac_int<VECTOR_WIDTH, true> DATA_TYPE_SIGNED;
typedef ac_int<(VECTOR_WIDTH << 1), false> DATA_TYPE_TMP;
typedef ac_int<PARAM_WIDTH, false> PARAMS_TYPE;

#endif /* CONFIG_H_ */
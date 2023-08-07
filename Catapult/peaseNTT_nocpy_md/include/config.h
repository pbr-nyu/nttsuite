#ifndef CONFIG_H_
#define CONFIG_H_

#include <ac_int.h>
//#include <inttypes.h>

#define VECTOR_SIZE  4096
#define VECTOR_ADDR_BIT  12
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
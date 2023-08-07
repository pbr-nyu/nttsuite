#ifndef CONFIG_H_
#define CONFIG_H_

#include <inttypes.h>

const unsigned VECTOR_SIZE = 1024;
const unsigned VECTOR_WIDTH = 64;
const unsigned VECTOR_ADDR_BIT = 10;
const unsigned PARAM_WIDTH = 32;

typedef uint32_t DATA_TYPE;
typedef uint64_t DATA_TYPE_TMP;
typedef int32_t	DATA_TYPE_SIGNED;
//typedef ac_fixed<64,64, false> UFIXED64_T;
//typedef ac_fixed<8, 8, false> UFIXED8_T;

#endif /* CONFIG_H_ */
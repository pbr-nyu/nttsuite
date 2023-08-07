#ifndef CONFIG_H_
#define CONFIG_H_

#include <ac_int.h>

const unsigned VECTOR_SIZE          = 4096;
const unsigned VECTOR_WIDTH         = 32;
const unsigned VECTOR_SIZE_LOG2     = 12;
const unsigned PARAM_WIDTH          = 32;

typedef ac_int<32, false>   DATA_TYPE;
typedef ac_int<64, false>   DATA_TYPE_TMP;
typedef ac_int<32, true>	DATA_TYPE_SIGNED;

#endif /* CONFIG_H_ */
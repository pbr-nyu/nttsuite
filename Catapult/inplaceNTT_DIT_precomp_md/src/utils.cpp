#include "../include/utils.h"
#include "../include/config.h"

using namespace std;
/**
 * Return vector with each element of the input at its bit-reversed position
 *
 * @param vec The vector to bit reverse
 * @param n   The length of the vector, must be a power of two
 * @return    The bit reversed vector
 */
DATA_TYPE *bit_reverse(DATA_TYPE *vec, DATA_TYPE *result){

	unsigned reverse_num;
	for(unsigned i = 0; i < VECTOR_SIZE; i++){
		reverse_num = 0;
		for(unsigned j = 0; j < VECTOR_SIZE_LOG2; j++){
			reverse_num = reverse_num << 1;
			if(i & (1 << j)){
				reverse_num = reverse_num | 1;
			}
		}
		result[reverse_num] = vec[i];
	}
	return result;
}

/**
 * Compare two vectors element-wise and return whether they are equivalent
 *
 * @param vec1	The first vector to compare
 * @param vec2 	The second vector to compare
 * @param n 	The length of the vectors
 * @param debug	Whether to print debug information (will run entire vector)
 * @return 	Whether the two vectors are element-wise equivalent
 */
unsigned compVec(DATA_TYPE *vec1, DATA_TYPE *vec2, unsigned n, bool debug){

	bool comp = true;
	unsigned       error_count = 0;
	for(DATA_TYPE i = 0; i < n; i++){
		if(vec1[i] != vec2[i]){
			comp = false;
			error_count++;
			if(debug){
				std::cout << "(vec1[" << i << "] : " << vec1[i] << ")";
				std::cout << "!= (vec2[" << i << "] : " << vec2[i] << ")";
				std::cout << std::endl;
			}else{
				break;
			}
		}
	}
	if(comp){
		std::cout<<"Test Passed"<<std::endl;
	} else{
		std::cout<<"Test Failed"<<std::endl;
	}

	return error_count;
}

/**
 * Perform the operation 'base^exp (mod m)' using the memory-efficient method
 *
 * @param base	The base of the expression
 * @param exp	The exponent of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
DATA_TYPE modExp(DATA_TYPE base, DATA_TYPE exp, DATA_TYPE m){
	DATA_TYPE result = 1;
	while(exp > 0){
		if(exp % 2){
			result = ((DATA_TYPE_TMP)result * base) % m;
		}
		exp = exp >> 1;
		base = ((DATA_TYPE_TMP)base * base) % m;
	}
	return result;
}
/**
 * Perform the operation 'base (mod m)'
 *
 * @param base	The base of the expression
 * @param m	The modulus of the expression
 * @return 	The result of the expression
 */
DATA_TYPE modulo(DATA_TYPE_SIGNED base, DATA_TYPE_SIGNED m){
	DATA_TYPE_SIGNED result = base % m;
	//return result;
	//return base - m*floor(base/m);
	return result >= 0? (DATA_TYPE)result : (DATA_TYPE)(result + m);
}

/**
 * Print an array of arbitrary length in a readable format
 *
 * @param vec	The array to be displayed
 * @param n	The length of the array
 */
void printVec(DATA_TYPE *vec, DATA_TYPE n){
	std::cout << "[";
	for(DATA_TYPE i = 0; i < n; i++){
		std::cout << vec[i] << ",";
	}
	std::cout << "]" << std::endl;
}



#include <cmath>		/* log2() */
#include <cstdint> 		/* int64_t, DATA_TYPE */
#include <cstdlib>		/* srand(), rand() */
#include <ctime>		/* time() */
#include <iostream> 		/* std::cout, std::endl */

#include "../include/utils.h" 	//INCLUDE HEADER FILE

/**
 * Return vector with each element of the input at its bit-reversed position
 *
 * @param vec The vector to bit reverse
 * @param n   The length of the vector, must be a power of two
 * @return    The bit reversed vector
 */
void bit_reverse(DATA_TYPE *vec, DATA_TYPE n, DATA_TYPE *result){

	DATA_TYPE num_bits = log2(n);

	unsigned reverse_num;
	for(unsigned i = 0; i < n; i++){

		reverse_num = 0;
		for(unsigned j = 0; j < num_bits; j++){

			reverse_num = reverse_num << 1;
			if(i & (1 << j)){
				reverse_num = reverse_num | 1;
			}
		}

		result[reverse_num] = vec[i];

	}
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
bool compVec(DATA_TYPE *vec1, DATA_TYPE *vec2, unsigned n, bool debug){

	bool comp = true;
	for(DATA_TYPE i = 0; i < n; i++){

		if(vec1[i] != vec2[i]){
			comp = false;

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

	return comp;
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
	return result >= 0? result : result + m;
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

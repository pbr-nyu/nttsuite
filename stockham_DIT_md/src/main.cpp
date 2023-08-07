#include "../include/main.h"

using namespace std;

void randVec(DATA_TYPE * vec, DATA_TYPE * vec2, DATA_TYPE * vec3, DATA_TYPE max){
	for(unsigned i = 0; i < VECTOR_SIZE; i++){
		DATA_TYPE value = rand() % (max + 1);
		value = i;
		vec[i] = value;
		vec2[i] = value;
		vec3[i] = value;
	}
}

/**
 * Perform a basic NTT on an input vector and return the result
 *
 * @param vec 	The input vector to be transformed
 * @param n	The size of the input vector
 * @param p	The prime to be used as the modulus of the transformation
 * @param r	The primitive root of the prime
 * @return 	The transformed vector
 */
DATA_TYPE *naiveNTT(DATA_TYPE *vec, DATA_TYPE n, DATA_TYPE p, DATA_TYPE r){

	DATA_TYPE k = (p - 1)/n;
	DATA_TYPE a = modExp(r,k,p);

	DATA_TYPE *result;
	result = (DATA_TYPE *) malloc(n*sizeof(DATA_TYPE));

	DATA_TYPE temp;
	for(DATA_TYPE i = 0; i < n; i++){

		temp = 0;
		for(DATA_TYPE j = 0; j < n; j++){
	
			temp = modulo(temp + modulo(vec[j]*modExp(a, i*j, p),p),p);
			/*temp = temp + modulo(vec[j]*modExp(a, i*j, p),p);*/
			/*temp = temp + vec[j]*modExp(a, i*j, p);*/

		}
		result[i] = modulo(temp,p);

	}

	return result;

}

/**
 * Perform an in-place iterative breadth-first decimation-in-time Cooley-Tukey NTT on an input vector and return the result
 *
 * @param vec 	The input vector to be transformed
 * @param n	The size of the input vector
 * @param p	The prime to be used as the modulus of the transformation
 * @param r	The primitive root of the prime
 * @param rev	Whether to perform bit reversal on the input vector
 * @return 	The transformed vector
 */
DATA_TYPE *inPlaceNTT_DIT(DATA_TYPE *vec, DATA_TYPE n, DATA_TYPE p, DATA_TYPE r, bool rev){

	DATA_TYPE *result;
	result = (DATA_TYPE *) malloc(n*sizeof(DATA_TYPE));

	if(rev){
		result = bit_reverse(vec,n);
	}else{
		for(DATA_TYPE i = 0; i < n; i++){	
			result[i] = vec[i];
		}
	}

	DATA_TYPE m,k_,a,factor1,factor2;
	for(DATA_TYPE i = 1; i <= log2(n); i++){ 

		m = pow(2,i);

		k_ = (p - 1)/m;
		a = modExp(r,k_,p);

		for(DATA_TYPE j = 0; j < n; j+=m){

			for(DATA_TYPE k = 0; k < m/2; k++){

				factor1 = result[j + k];
				factor2 = modulo(modExp(a,k,p)*result[j + k + m/2],p);
			
				result[j + k] 		= modulo(factor1 + factor2, p);
				result[j + k+m/2] 	= modulo(factor1 - factor2, p);

			}
		}

		//printVec(result,n);
	}

	return result;

}

int main(int argc, char *argv[]){

	DATA_TYPE k = 3;

	DATA_TYPE n = VECTOR_SIZE;
 	//DATA_TYPE p = 68719403009;
  	//DATA_TYPE r = 36048964756;
 	DATA_TYPE p = (479  << 21) + 1;
  	DATA_TYPE r = 3;

	// DATA_TYPE vec[VECTOR_SIZE], vec2[VECTOR_SIZE];
	// DATA_TYPE *result1, *result2;

	// for (int i = 0; i < VECTOR_SIZE; i++){
	// 	vec[i] = i;
	// 	vec2[i] = i;
	// }
	// result2 = naiveNTT(vec2, n, p, r);
	// for(int i=0;i<100;i++){
	// 	result1 = stockham_dit(vec, n,  p, r);
	// 	free(result1);
	// }
	// printVec(result1, VECTOR_SIZE);

	DATA_TYPE vec[VECTOR_SIZE], vec2[VECTOR_SIZE], vec3[VECTOR_SIZE];
	randVec(vec, vec2, vec3, 1000);
	DATA_TYPE *sdif_result, *sdit_result, *naive_result;

	sdif_result = stockham_dif(vec, n, p, r, true);
	sdit_result = stockham_dif(vec2, n, p, r, true);
	naive_result = naiveNTT(vec3, n, p, r);

	bool comp1 = compVec(sdif_result, naive_result, VECTOR_SIZE, true);
	if (comp1) {
		cout << "Stockham DIF Test Passed!" << endl;
	}
	bool comp2 = compVec(sdit_result, naive_result, VECTOR_SIZE, true);
	if (comp2) {
		cout << "Stockham DIT Test Passed!" << endl;
	}
	//cout<<"Compare result: "<<compVec(result1, result2, n, true);
//	free(sdif_result);
//	free(sdit_result);
	free(naive_result);
/*	
	//DATA_TYPE *vec = randVec(n,10);

	DATA_TYPE *out1 = naiveNTT(vec,n,p,r);
	printVec(out1,n);

	DATA_TYPE *out2 = fourStepNTT(vec,n,p,r,2);
	printVec(out2,n);
*/
	return 0;

}
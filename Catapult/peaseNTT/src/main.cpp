#include "../include/main.h"
#include "../include/utils.h"

using namespace std;

void cpyVec(uint64_t * src, uint64_t * dst, uint64_t n){
    for(int i = 0; i < n; i++)
        dst[i] = src[i];
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
uint64_t *naiveNTT(uint64_t *vec, uint64_t n, uint64_t p, uint64_t r){

	uint64_t k = (p - 1)/n;
	uint64_t a = modExp(r,k,p);

	uint64_t *result;
	result = (uint64_t *) malloc(n*sizeof(uint64_t));

	uint64_t temp;
	for(uint64_t i = 0; i < n; i++){

		temp = 0;
		for(uint64_t j = 0; j < n; j++){
	
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
uint64_t *inPlaceNTT_DIT(uint64_t *vec, uint64_t n, uint64_t p, uint64_t r, bool rev){

	uint64_t *result;
	result = (uint64_t *) malloc(n*sizeof(uint64_t));

	if(rev){
		result = bit_reverse(vec,n);
	}else{
		for(uint64_t i = 0; i < n; i++){	
			result[i] = vec[i];
		}
	}

	uint64_t m,k_,a,factor1,factor2;
	for(uint64_t i = 1; i <= log2(n); i++){ 

		m = pow(2,i);

		k_ = (p - 1)/m;
		a = modExp(r,k_,p);

		for(uint64_t j = 0; j < n; j+=m){

			for(uint64_t k = 0; k < m/2; k++){

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

uint64_t* peaceNTT_golden(uint64_t * vec, uint64_t n, uint64_t p, uint64_t g, bool rev){

    uint64_t t = (uint64_t)log2(n);
    uint64_t * xt = vec;
    uint64_t * yt = (uint64_t*)calloc(n, sizeof(uint64_t));

    if(rev)
        xt = bit_reverse(vec, n);

    cout<<"Peace NTT\nt: "<<t<<endl;

    uint64_t r_strt = 0;
    uint64_t r_end  = exp2(t - 1);
    uint64_t mid = n / 2;
    for (uint64_t c = t; c >= 1; c--){

        uint64_t w = modExp(g, (p - 1) / exp2(t - c + 1), p);
        uint64_t base = exp2(c - 1);

        for (uint64_t r = r_strt; r < r_end; r++){
            uint64_t r1 = floor(r / base);
            uint64_t f1 = xt[r*2];
            uint64_t f2 = modulo(modExp(w, r1, p) * xt[r*2 + 1], p);
            yt[r]       = modulo(f1 + f2, p);
            yt[r+ mid]  = modulo(f1 - f2, p);
        }
        cpyVec(yt, xt, n);
        cout<<"Complte stage: "<<c<<" computation.\n";
        //printVec(xt,n);
    }
    return yt;
}

CCS_MAIN(int argc, char *argv[]){

	uint64_t k = 3;

	uint64_t n = VECTOR_SIZE;
  	uint64_t p = 68719403009;
  	uint64_t r = 36048964756;

	uint64_t vec[VECTOR_SIZE], vec2[VECTOR_SIZE];
	uint64_t *result1, *result2;

	for (unsigned i = 0; i < VECTOR_SIZE; i++){
		vec[i] = i;
		vec2[i] = i;
	}
	result2 = inPlaceNTT_DIT(vec2, VECTOR_SIZE, p, r, false);
	result1 = peaceNTT_golden(vec, VECTOR_SIZE,  p, r, false);
	//printVec(result, VECTOR_SIZE);
	UINT64_T vec_dev[VECTOR_SIZE], twiddle_dev[VECTOR_SIZE], result_dev[VECTOR_SIZE];
	uint64_t w0 = (UINT64_T)modExp(g, (p - 1) / n, p);
	uint64_t witer = 1;
	for(int i=0; i < n; i++){
		twiddle_dev[i] = (UINT64_T)witer;
		witer = modulo(witer * w0);
	}
	UINT64_T p_dev = (479 << 21) + 1, r_dev = 3;
	for (unsigned i = 0; i < VECTOR_SIZE; i++){
		twiddle_dev[i] = i;
		vec_dev[i] = i;
	}
	CCS_DESIGN(peaceNTT)(vec_dev, p_dev, r_dev, result_dev, twiddle_dev);
	cout<<"Compare result: "<<compVec(result1, result2, n, true);
	free(result1);
	free(result2);
/*	
	//uint64_t *vec = randVec(n,10);

	uint64_t *out1 = naiveNTT(vec,n,p,r);
	printVec(out1,n);

	uint64_t *out2 = fourStepNTT(vec,n,p,r,2);
	printVec(out2,n);
*/
	CCS_RETURN(0);

}
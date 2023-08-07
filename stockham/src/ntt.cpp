#include "../include/ntt.h"
#include "../include/utils.h"
#include <inttypes.h>
#include <sys/time.h>   // for gettimeofday()
using namespace std;

void cpyVec(DATA_TYPE* src, DATA_TYPE*dst, int length){
    for(int i=0; i<length; i++){
        dst[i] = src[i];
    }
}

void fft0(int n, int s, bool eo, DATA_TYPE* x, DATA_TYPE* y, DATA_TYPE p, DATA_TYPE g)
{
    const int m = n/2;
    const DATA_TYPE theta0 = modExp(g, (p - 1) / n, p);
    DATA_TYPE w = 1;

    if (n == 1) { if (eo) for (int q = 0; q < s; q++) y[q] = x[q]; }
    else {
        for (int t = 0; t < m; t++) {
            for (int q = 0; q < s; q++) {
                const DATA_TYPE a = x[q + s*(t + 0)];
                const DATA_TYPE b = x[q + s*(t + m)];
                y[q + s * (2 * t + 0)] = modulo(a + b, p);
                y[q + s * (2 * t + 1)] = modulo(modulo(a - b, p) * w, p);
            }
            w = modulo(w * theta0 ,p);
        }
        fft0(n >> 1, s << 1, !eo, y, x, p, g);
    }
}

void fft1(int n, int s, bool eo, DATA_TYPE* x, DATA_TYPE* y, DATA_TYPE p, DATA_TYPE g)
{
    const int m = n/2;
    const DATA_TYPE theta0 = modExp(g, (p - 1) / n, p);
    DATA_TYPE w = 1;

    if (n == 1) { if (eo) for (int q = 0; q < s; q++) x[q] = y[q]; }
    else {
        fft1(n >> 1, s << 1, !eo, y, x, p, g);
        for (int t = 0; t < m; t++) {
            for (int q = 0; q < s; q++) {
                const DATA_TYPE a = y[q + s*(2 * t + 0)];
                const DATA_TYPE b = modulo(y[q + s*(2 * t + 1)] * w, p);
                x[q + s * (t + 0)] = modulo(a + b, p);
                x[q + s * (t + m)] = modulo(a - b, p);
            }
            w = modulo(w * theta0 ,p);
        }
    }
}

DATA_TYPE* stockham_dit(DATA_TYPE * vec, DATA_TYPE n, DATA_TYPE p, DATA_TYPE g, bool rev){

    int t = (int)log2(n);
    DATA_TYPE * xt = vec;
    DATA_TYPE * yt = (DATA_TYPE*)calloc(n, sizeof(DATA_TYPE));

//    cout<<"Stockham NTT\nt: "<<t<<endl;
    DATA_TYPE * twiddle = (DATA_TYPE*)malloc(n * sizeof(DATA_TYPE));
    DATA_TYPE w0 = modExp(g, (p - 1) / n, p);
    DATA_TYPE witer = 1;
    for(int i = 0; i < n; i++){
        twiddle[i] = witer;
        witer = modulo(witer * w0, p);
    }

    //fft1(n, 1, 0, xt, yt, p, g);
    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    DATA_TYPE * x = xt;
    DATA_TYPE * y = yt;
    DATA_TYPE *tmp = x;
    x = y;
    y = tmp;
    int s = n >> 1;
    int shift = t - 1;
    for(int nn = 2; nn <= n ; nn = nn << 1, s = s >> 1, shift = shift - 1){
        const int m = nn / 2;
        const DATA_TYPE theta0 = modExp(g, (p - 1) / nn, p);

            //fft0(n/2, 2*s, !eo, y, x, p, g);
        for (int t = 0; t < (VECTOR_SIZE >> 1); t++) {
                const int upper = (~(s - 1)) & t;
                DATA_TYPE w = twiddle[(upper >> shift) << shift];
                const int q     = (s - 1) & t;
                    const DATA_TYPE a = y[q + (upper << 1)];
                    const DATA_TYPE b = modulo(y[q + (upper << 1) + s] * w, p);
                    x[q + upper] = modulo(a + b, p);
                    x[q + upper + s * m] = modulo(a - b, p);
                /*
                for (int q = 0; q < s; q++) {
                    const DATA_TYPE a = y[q + s*(2*t + 0)];
                    const DATA_TYPE b = modulo(y[q + s*(2*t + 1)] * w, p);
                    x[q + s*(t + 0)] = modulo(a + b, p);
                    x[q + s*(t + m)] = modulo(a - b, p);
                }
                */
        }
        DATA_TYPE *tmp = x;
        x = y;
        y = tmp;
    }   
    if(t % 2){
        for(int i=0; i<n; i++)
            vec[i] = y[i];
    }
    gettimeofday(&end, NULL);
    long seconds = (end.tv_sec - begin.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (begin.tv_usec);
    static long time = 0;
    static long times = 0;
    time += micros;
    times += 1;
    cout<<time/times<<endl;
    return vec;
}

DATA_TYPE* stockham_dif(DATA_TYPE * vec, DATA_TYPE n, DATA_TYPE p, DATA_TYPE g, bool rev){

    int t = (int)log2(n);
    DATA_TYPE * xt = vec;
    DATA_TYPE * yt = (DATA_TYPE*)calloc(n, sizeof(DATA_TYPE));

//    cout<<"Stockham NTT\nt: "<<t<<endl;
    DATA_TYPE * twiddle = (DATA_TYPE*)malloc(n * sizeof(DATA_TYPE));
    DATA_TYPE w0 = modExp(g, (p - 1) / n, p);
    DATA_TYPE witer = 1;
    for(int i = 0; i < n; i++){
        twiddle[i] = witer;
        witer = modulo(witer * w0, p);
    }

    //fft1(n, 1, 0, xt, yt, p, g);
   struct timeval begin, end;
   gettimeofday(&begin, NULL);
    DATA_TYPE * x = xt;
    DATA_TYPE * y = yt;
    int s = 1;
    bool eo = 1;
    if((t % 2) == 0){
        eo = 0;
    }
    for(int nn = n; nn > 0 ; nn = nn >> 1, s = s << 1){
        const int m = nn / 2;
        const DATA_TYPE theta0 = modExp(g, (p - 1) / nn, p);
        DATA_TYPE w = 1;

        if (nn == 1) { if (eo) for (int q = 0; q < s; q++) y[q] = x[q]; }
        else {
            //fft0(n/2, 2*s, !eo, y, x, p, g);
            for (int t = 0; t < m; t++) {
                for (int q = 0; q < s; q++) {
                    const DATA_TYPE a = x[q + s*(t + 0)];
                    const DATA_TYPE b = x[q + s*(t + m)];
                    y[q + s*(2 * t + 0)] = modulo(a + b, p);
                    y[q + s*(2 * t + 1)] = modulo(modulo(a - b, p) * w, p);
                }
                w = modulo(w * theta0, p);
            }
        }
        DATA_TYPE *tmp = x;
        x = y;
        y = tmp;
    }   
    long seconds = (end.tv_sec - begin.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (begin.tv_usec);
    static long time = 0;
    static long times = 0;
    time += micros;
    times += 1;
    cout<<time/times<<endl;

    return vec;
}

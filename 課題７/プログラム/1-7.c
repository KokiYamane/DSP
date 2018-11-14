#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define DATASIZE 50

// 複素数構造体
typedef struct{
    double re;
    double im;
}complex_t;

// 複素数の加算
complex_t complex_addition(complex_t a, complex_t b) {
    complex_t ans;
    ans.re = a.re + b.re;
    ans.im = a.im + b.im;
    return ans;
}

// 複素数の減算
complex_t complex_subtraction(complex_t a, complex_t b) {
    complex_t ans;
    ans.re = a.re - b.re;
    ans.im = a.im - b.im;
    return ans;
}

// 複素数の乗算
complex_t complex_multiplication(complex_t a, complex_t b) {
    complex_t ans;
    ans.re = (a.re * b.re) - (a.im * b.im);
    ans.im = (a.re * b.im) + (a.im * b.re);
    return ans;
}

// 複素数の除算
complex_t complex_division(complex_t a, complex_t b) {
    complex_t ans;
    return ans;
}

/////////////////////////////////////////////////////
// bitリバーサル ビット演算
/////////////////////////////////////////////////////
void bit_r(int *bit, int N, int r)
{
    int i,j;
    for(i=0;i<N;i++) { // iは入力x_iの通常の順番
        bit[i]=0;
        for(j=0;j<r;j++) {
            bit[i]+= ((i >> j ) & 1) << (N-j-1);// iをj個右シフトし，
                                                //そのLSBをMSBから詰めていく
        }
    }
}

void hamming_window(double data[DATASIZE])
{
    for(int i = 0; i < DATASIZE; i++) {
        data[i] *= 0.54 - 0.46 * cos(2 * M_PI * i / DATASIZE);
    }
}

void dft(const complex_t *xn, complex_t *Xk, int discrete)
{
    //DFTとIDFTの切り替え
    int a = 1, b = 1;
    if(discrete) {
        a = -1;
        b = DATASIZE;
    }

    //出力配列初期化
    for(int i = 0; i < DATASIZE; i++) {
        Xk[i].re = 0;
        Xk[i].im = 0;
    }

    //DFT(IDFT)計算
    for(int k = 0; k < DATASIZE; k++) {
        for(int n = 0; n < DATASIZE; n++) {
            Xk[k].re += (       xn[n].re * cos(2 * M_PI / DATASIZE * n * k)
                          + a * xn[n].im * sin(2 * M_PI / DATASIZE * n * k)) / b;
            Xk[k].im += (       xn[n].im * cos(2 * M_PI / DATASIZE * n * k)
                          - a * xn[n].re * sin(2 * M_PI / DATASIZE * n * k)) / b;
        }
    }
}

/////////////////////////////////////////////////////
// 例えばプログラム：FFT(IFFT)
/////////////////////////////////////////////////////
void fft(complex_t *in, complex_t *wnk, int N, int r)
{
    int r_big=1, r_sma=N/2; //初期値を与えておく
    int i, j, k, in1, in2, nk;
    complex_t dummy;
    for(i=0;i<r;i++) {// ビット回繰り返し（段数回）
        for(j = 0; j < r_big; j++) {//繰り返しが段々増える
            for(k = 0; k < r_sma; k++) {//繰り返しが段々減る：(j+k)の合計=N/2回の繰り返し
            in1 = r_big * 2 * k + j;
            //バタ上段の入力順番
            in2 = in1 + r_big;
            //バタ下段の入力順番
            nk = j * r_sma;
            //回転子の番号
            dummy = complex_multiplication(in[in2], wnk[nk]);//バタ入力下段×重みWnk
            in[in2] = complex_subtraction(in[in1], dummy);//バタ演算の下段出力(そのまま次段の入力)
            in[in1] = complex_addition(in[in1], dummy);
            //バタ演算の上段出力(そのまま次段の入力)
            }
        }
        r_big *= 2;
        // 2^i :1,2,4...N/2まで増加
        r_sma /= 2;
        // 2^(r-1-i):N/2...4.2.1まで減少
    }
}

void main()
{
    printf("H30 4J41 yamane kouki\n");
    printf("Select mode.\n");
    printf("DFT:0/IDFT:1\n");
    int discrete;
    scanf("%d",&discrete);
    printf("Do you use window fanction?\n");
    printf("YES:1/NO:0\n");
    int useWindow;
    scanf("%d",&useWindow);

    //ファイルオープン
    FILE *fp;
    fp = fopen("a_wav.txt","r");
    if(fp == NULL) {
        printf("can't open tha read file.");
        exit(1);
    }

    //ファイル読み込み
    double *data;
    data = (double *)malloc(1 * sizeof(double));
    if(data == NULL) {
        printf("It failed to allocate memory.");
        exit(1);
    }
    int datasize = 0;
    while(fscanf(fp, "%lf", &data[datasize]) != EOF) {
        datasize++;
        data = (double *)realloc(data, (datasize + 1) * sizeof(double));
        if(data == NULL) {
            printf("It failed to allocate memory.");
            exit(1);
        }
    }
    fclose(fp);

    //窓関数
    if(useWindow) hamming_window(data);

    // int data[] = {3,3,-1,-1};
    complex_t xn[DATASIZE];
    complex_t Xk[DATASIZE];
    for(int i = 0; i < DATASIZE; i++) {
        xn[i].re = data[i];
        xn[i].im = 0;
    }

    //DFT計算
    fft(xn,Xk,DATASIZE,8);

    //出力
    FILE *wfp;
    wfp = fopen("out.txt","w");
    if(wfp == NULL) {
        printf("can't open tha white file.");
        exit(1);
    }
    printf(" cnt,      real, imaginaly, amplitude,     phase\n");
    fprintf(wfp, " cnt,      real, imaginaly, amplitude,     phase\n");
    for(int i = 0; i < DATASIZE; i++) {
        double amplitude = 20 * log10(sqrt(pow(Xk[i].re, 2) + pow(Xk[i].im, 2)));
        double phase = atan2(Xk[i].im, Xk[i].re);
        printf("%4d,%10.6f,%10.6f,%10.6f,%10.6f\n", i, Xk[i].re, Xk[i].im, amplitude, phase);
        fprintf(wfp, "%4d,%10.6f,%10.6f,%10.6f,%10.6f\n", i, Xk[i].re, Xk[i].im, amplitude, phase);
    }
    fclose(wfp);
    while(1);
}
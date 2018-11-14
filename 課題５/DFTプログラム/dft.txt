#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define DATASIZE 50

typedef struct{
    double re;
    double im;
}conplex_t;

void hamming_window(double data[DATASIZE])
{
    for(int i = 0; i < DATASIZE; i++) {
        data[i] *= 0.54 - 0.46 * cos(2 * M_PI * i / DATASIZE);
    }
}

void dft(const conplex_t *xn, conplex_t *Xk, int discrete)
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
    if(useWindow) {
        hamming_window(data);
    }

    // int data[] = {3,3,-1,-1};
    conplex_t xn[DATASIZE];
    conplex_t Xk[DATASIZE];
    for(int i = 0; i < DATASIZE; i++) {
        xn[i].re = data[i];
        xn[i].im = 0;
    }

    //DFT計算
    dft(xn,Xk,discrete);

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
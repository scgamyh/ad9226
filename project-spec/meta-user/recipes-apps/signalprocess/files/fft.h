#ifndef FFT_H_
#define FFT_H_
//
// 快速傅立叶变换 Fast Fourier Transform
// By rappizit@yahoo.com.cn
// 2007-07-20
// 版本 2.0
// 改进了《算法导论》的算法，旋转因子取 ωn-kj  (ωnkj 的共轭复数)
// 且只计算 n / 2 次，而未改进前需要计算 (n * lg n) / 2 次。
// 


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include"global.h"

 void _fft(CPLX buf[], CPLX out[], int n, int step);

int fft(CPLX buf[], int n);
#endif
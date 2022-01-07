#pragma once

typedef struct matrix{
    int me[4];
}Matrix;

typedef struct vector{
    int ve[2];
}Vector;

long fibo(int n);
Matrix mul_matrix(int n);
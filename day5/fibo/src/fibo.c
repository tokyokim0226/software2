#include <stdio.h>
#include <assert.h>
#include "fibo.h"

long fibo(int n){
    assert(n <= 90);
    Matrix m;
    if(n == 0){
        return 0;
    }else if(n == 1){
        return 1;
    }else{
        
        m = mul_matrix(n-1);
    }
    return m.me[0];
}

Matrix mul_matrix(int n){
    Matrix m1,m2;
    if(n == 1){
        m1.me[0] = 1;
        m1.me[1] = 1;
        m1.me[2] = 1;
        m1.me[3] = 0;
    }else if(n % 2 == 0){
        m2 = mul_matrix(n / 2);
        m1.me[0] = m2.me[0]*m2.me[0] + m2.me[1]*m2.me[2];
        m1.me[1] = m2.me[0]*m2.me[1] + m2.me[1]*m2.me[3];
        m1.me[2] = m2.me[2]*m2.me[0] + m2.me[3]*m2.me[2];
        m1.me[3] = m2.me[2]*m2.me[1] + m2.me[3]*m2.me[3];
    }else{
        m2 = mul_matrix((n - 1) / 2);
        m1.me[0] = m2.me[0]*m2.me[0] + m2.me[1]*m2.me[2] + m2.me[0]*m2.me[1] + m2.me[1]*m2.me[3];
        m1.me[1] = m2.me[0]*m2.me[0] + m2.me[1]*m2.me[2];
        m1.me[2] = m2.me[2]*m2.me[0] + m2.me[3]*m2.me[2] + m2.me[2]*m2.me[1] + m2.me[3]*m2.me[3];
        m1.me[3] = m2.me[2]*m2.me[0] + m2.me[3]*m2.me[2];
    }
    return m1;
}


#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "optimize.h"
#include "func.h"

int optimize(const int dim, double x[], 
             void (*calc_grad)(const double [], double [], Data *data), double (*calc_value)(const double [], Data*, int), Data *data)
{
    // 勾配ベクトルを記録する領域を確保
    double *g = malloc(dim * sizeof(double));
    // ヘッセ行列やその逆行列を記録する領域を確保
    double *l = malloc(dim*dim*sizeof(double));
    //行列の積の結果(ニュートン法更新のための列ベクトル)を確保する領域を確保
    double *m = malloc(dim*sizeof(double)); 

    int iter = 0;
    while (++iter < 500000) {
	
	// 引数で渡された関数を使って勾配ベクトルを計算
	(*calc_grad)(x, g, data);

    //ヘッセ行列の計算
    calc_hesse(x,l,data);
   
    //逆行列の計算
    calc_inv(l,data);
	
    // 行列の積を計算
    calc_mul(l,g,m,dim);

	// ニュートン法による更新
	for (int i = 0; i < dim; i++) {
	    x[i] -= m[i];
	}

    double error = calc_value(x,data,3)/data[0].datanum;

    if(error <= 0.36){
        break;
    }
    }
    printf("a is %f, b is %f, c is %f\n",x[0],x[1],x[2]);
    printf("the temperature at mtfuji is %7.4f\n", 3.776*3.776*x[0] + 3.776*x[1] + x[2]);
    free(g);
    free(l);
    return iter;
}


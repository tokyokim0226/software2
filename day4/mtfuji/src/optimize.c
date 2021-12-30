#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "optimize.h"
#include "func.h"

int optimize(const double alpha, const int dim, double x[], 
             void (*calc_grad)(const double [], double [], Data *data), double (*calc_value)(const double [], Data*, int), Data *data)
{
    // 勾配ベクトルを記録する領域を確保
    double *g = malloc(dim * sizeof(double));
    
    int iter = 0;
    while (++iter < 5000000) {
	
	// 引数で渡された関数を使って勾配ベクトルを計算
	(*calc_grad)(x, g, data);
	
	const double error = f_value(x,data,2)/data[0].datanum;

	// printf("%3d error sum = %7.4f", iter, error);
	// for (int i = 0; i < dim; i++) {
	//     printf(", x[%d] = %7.4f", i, x[i]);
	// }
    // printf(", the temperature at mtfuji is %7.4f", 3.776*x[0] + x[1]);
	// printf("\n");
	
	// 最急降下法による更新
	for (int i = 0; i < dim; i++) {
	    x[i] -= alpha * g[i];
	}
    
    if(error <= 0.39){
        break;
    }
    }
    printf("the temperature at mtfuji is %7.4f\n", 3.776*x[0] + x[1]);
    free(g);
    return iter;
}


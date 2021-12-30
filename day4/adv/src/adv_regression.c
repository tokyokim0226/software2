#include <stdio.h>
#include <stdlib.h>
#include "optimize.h"
#include "func.h"

int main(const int argc, const char **argv)
{
    
    const char *filename = "./data.csv";
    Data *data = read_file(filename);
    
    const int dim = f_dimension();
    
    double *x = malloc(dim * sizeof(double));
    for (int i = 0; i < dim; i++) {
	x[i] = 0;
    }
    
    const int iter = optimize(dim, x, f_gradient, f_value, data);

    printf("number of iterations = %d\n", iter);

    for(int i = 0; i < data[0].datanum; i++){
        free(data[i].loc);
    } 
    free(data);

    free(x);
    

    return 0;
}


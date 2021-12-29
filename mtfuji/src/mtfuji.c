#include <stdio.h>
#include <stdlib.h>
#include "optimize.h"
#include "func.h"
#include "sortlib.h"

int main(const int argc, const char **argv)
{
    const double alpha = (argc == 2) ? atof(argv[1]) : 0.00001;
    
    const char *filename = "./data.csv";
    Data *data = read_file(filename);
    
    const int dim = f_dimension();
    
    double *x = malloc(dim * sizeof(double));
    for (int i = 0; i < dim; i++) {
	x[i] = 0;
    }
    
    printf("alpha = %f\n", alpha);
    
    const int iter = optimize(alpha, dim, x, f_gradient, f_value, data);

    printf("number of iterations = %d\n", iter);
    
    //標高順でソート
    int datanum =  data[0].datanum;
    qsort(data,data[0].datanum,sizeof(data[0]),comp_alt);
    for(int i = 0; i < datanum; i++){
        printf("%-20s,%f,%f\n",data[i].loc,data[i].alt,data[i].temp);
    }

    for(int i = 0; i < data[0].datanum; i++){
        free(data[i].loc);
    } 
    free(data);

    free(x);
    

    return 0;
}


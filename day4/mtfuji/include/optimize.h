#ifndef OPTIMIZE_OPTIMIZE_H
#define OPTIMIZE_OPTIMIZE_H

#include "func.h"
int optimize(const double alpha, const int dim, double x[], 
	     void (*calc_grad)(const double [], double [], Data *data), double (*calc_value)(const double [], Data*,int), Data *data);

#endif

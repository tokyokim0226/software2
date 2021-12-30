#pragma once

#include "func.h"
int optimize(const int dim, double x[], 
	     void (*calc_grad)(const double [], double [], Data *data), double (*calc_value)(const double [], Data*,int), Data *data);


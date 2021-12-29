#ifndef OPTIMIZE_FUNC_H
#define OPTIMIZE_FUNC_H

typedef struct {
    int datanum; // the number of data
    char *loc;   // location name
    double alt;  // altitude (km)
    double temp; // temperature (centigrade)
} Data;

Data* read_file(const char *filename);
int f_dimension();
void f_gradient(const double x[], double g[],Data *data);
double f_value(const double x[],Data *data,int index);

#endif

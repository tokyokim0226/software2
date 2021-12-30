#pragma once

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

//ヘッセ行列を求める
void calc_hesse(const double x[],double l[],Data *data);

//3次正方行列の逆行列を求める
void calc_inv(double l[], Data *data);

//行列とベクトルの積を求める
void calc_mul(const double l[],const double g[],double m[], int dim);



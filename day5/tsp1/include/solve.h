#pragma once
#include "city.h"

typedef struct ans{
    double dist;
    int *route;
} Answer;

double solve(const City *city, int n, int *route);

Answer search(const City *city, int n, int *route);

void make_initial(int n,int *route,int seed);        // 初期値を作成する関数
double get_sum_distance(const City *city,int n,int *route);



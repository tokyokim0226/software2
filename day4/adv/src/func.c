#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"
// 必要に応じてヘッダファイルを追加する

Data* read_file(const char*filename){
    char tmp[100];
    int datanum = 0;
    FILE *fp = fopen(filename,"r");
    if(fp == NULL){
        printf("cannot open file\n");
        exit(EXIT_FAILURE);
    }
    while(fscanf(fp,"%[^\n]%*1[\n]",tmp) != EOF){     
        datanum++;
    }
    fseek(fp,0L,SEEK_SET);
    Data *data = malloc(sizeof(Data)*datanum);
    data[0].datanum = datanum;
    for(int i = 0; i < data[0].datanum; i++){
        data[i].loc = malloc(sizeof(char)* 100);
        fscanf(fp,"%[^,],%lf,%lf\n",tmp,&(data[i].alt),&(data[i].temp));
        strcpy(data[i].loc,tmp);
    }
    fclose(fp);
    
    return data;
}

int f_dimension()
{
    return 3;
}

void f_gradient(const double x[], double g[],Data *data)
{
    g[0] = -2*f_value(x,data,0);
    g[1] = -2*f_value(x,data,1);
    g[2] = -2*f_value(x,data,2);
}

double f_value(const double x[],Data *data,int index){
    double sum = 0;
    for(int i = 0; i < data[0].datanum; i++){
        if(index == 0){     //for gradient(a)
            sum += data[i].alt*data[i].alt*(data[i].temp - x[0]*data[i].alt*data[i].alt -x[1]*data[i].alt - x[2]);
        }else if(index == 1){       // for gradient(b)
            sum += data[i].alt*(data[i].temp - x[0]*data[i].alt*data[i].alt -x[1]*data[i].alt - x[2]);
        }else if(index == 2){       // for gradient(c)
            sum += (data[i].temp - x[0]*data[i].alt*data[i].alt -x[1]*data[i].alt - x[2]);
        }else{                      // for error sum
            sum += (data[i].temp - x[0]*data[i].alt*data[i].alt -x[1]*data[i].alt - x[2])*(data[i].temp - x[0]*data[i].alt*data[i].alt -x[1]*data[i].alt - x[2]);
        }
    }
    return sum;
}

void calc_hesse(const double x[],double l[],Data *data)
{
    double sum = 0;
    for(int i = 0; i < data[0].datanum; i++){
        sum += data[i].alt*data[i].alt*data[i].alt*data[i].alt;
    }
    l[0] = 2 * sum;
    sum = 0;
    for(int i = 0; i < data[0].datanum; i++){
        sum += data[i].alt*data[i].alt*data[i].alt;
    }
    l[1] = 2 * sum;
    l[3] = l[1];
    sum = 0;
    for(int i = 0; i < data[0].datanum; i++){
        sum += data[i].alt*data[i].alt;
    }
    l[2] = 2 * sum;
    l[6] = l[2];
    l[4] = l[2];
    sum = 0;
    for(int i = 0; i < data[0].datanum; i++){
        sum += data[i].alt;
    }
    l[5] = 2 * sum;
    l[7] = l[5];
    sum = 0;
    for(int i = 0; i < data[0].datanum; i++){
        sum += 1;
    }
    l[8] = 2 * sum;
}

void calc_inv(double l[], Data *data){
    double d = l[0]*l[4]*l[8] + l[3]*l[7]*l[2] + l[1]*l[5]*l[6] - l[2]*l[4]*l[6] - l[1]*l[3]*l[8] - l[0]*l[5]*l[7];
    double tmp[9] = {0};
    tmp[0] = l[4]*l[8] - l[7]*l[5];
    tmp[1] = -1 * (l[1]*l[8] - l[7]*l[2]);
    tmp[2] = l[1]*l[5] - l[4]*l[2];
    tmp[3] = -1 * (l[3]*l[8] - l[6]*l[5]);
    tmp[4] = l[0]*l[8] - l[6]*l[2];
    tmp[5] = -1 * (l[0]*l[5] - l[3]*l[2]);
    tmp[6] = l[3]*l[7] - l[6]*l[4];
    tmp[7] = -1 * (l[0]*l[7] - l[6]*l[1]);
    tmp[8] = l[0]*l[4] - l[3]*l[1];

    for(int i = 0; i < 9; i++){
        l[i] = tmp[i]/d;
    }
}

void calc_mul(const double l[],const double g[],double m[], int dim){
    for(int i = 0; i < dim; i++){
        m[i] = 0;
    }
    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            m[i] += l[i + 3*j]*g[j];
        }
    }    
}


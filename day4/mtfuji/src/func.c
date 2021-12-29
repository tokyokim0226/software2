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
    return 2;
}

void f_gradient(const double x[], double g[],Data *data)
{
    g[0] = -2*f_value(x,data,0);
    g[1] = -2*f_value(x,data,1);
}

double f_value(const double x[],Data *data,int index){
    double sum = 0;
    for(int i = 0; i < data[0].datanum; i++){
        if(index == 0){     //for gradient(a)
            sum += data[i].alt*(data[i].temp - x[0]*data[i].alt - x[1]);
        }else if(index == 1){       // for gradient(b)
            sum += (data[i].temp - x[0]*data[i].alt - x[1]);
        }else{                      // for error sum
            sum += (data[i].temp - x[0]*data[i].alt - x[1])*(data[i].temp - x[0]*data[i].alt - x[1]);
        }
    }
    return sum;
}


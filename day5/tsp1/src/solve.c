#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "city.h"
#include "map.h"
#include "solve.h"

double solve(const City *city,int n, int *route)
{
    int init_num = 1000000;  //初期解の個数

    Answer *ans = (Answer *)malloc(sizeof(Answer)*init_num);
    for(int i = 0; i < init_num; i++){
        make_initial(n,route,i);
        ans[i] = search(city, n, route);    
        // printf("number.%d:",i);
        // printf("dist:%f",ans[i].dist);
        // for(int j = 0; j < n;j++){
        //     printf("%d--> ",ans[i].route[j]);
        // }

        // printf("\n");
    }
    
    double tmp = 0;

    int i = 0;
        for(int j = i; j < init_num; j++){
            if(ans[0].dist > ans[j].dist){
                tmp = ans[0].dist;
                ans[0].dist = ans[j].dist;
                ans[j].dist = tmp;
                memcpy(ans[0].route, ans[j].route, sizeof(int)*n);
            }
        }
    
    memcpy(route, ans[0].route, sizeof(int)*n);

    return ans[0].dist;
}

Answer search(const City *city, int n, int *route)
{
    double sum_d1 = 0;      // 最小値を記憶する
    sum_d1 = get_sum_distance(city,n,route);
    //printf("sum_d1 = %lf\n",sum_d1);
    double sum_d2 = 0;
    int *initial_route =(int *)calloc(n,sizeof(int));
    int *route1 = (int *)calloc(n,sizeof(int));
    double tmp;
    memcpy(route1,route,sizeof(int)*n);
    memcpy(initial_route,route,sizeof(int)*n);
    for(int i = 0; i < n-1; i++){
        for(int j = i + 1; j < n; j++){
            route1[i] = initial_route[j];
            route1[j] = initial_route[i];
            sum_d2 = get_sum_distance(city,n,route1);
            // for(int i = 0; i < n;i++){
            //     printf("%d->",route1[i]);
            // }
            // printf("sum_d2 = %lf\n",sum_d2);
            if(sum_d1 > sum_d2){
                tmp = sum_d1;
                sum_d1 = sum_d2;
                sum_d2 = tmp;
                memcpy(route,route1,sizeof(int)*n);
            }
            memcpy(route1,initial_route,sizeof(int)*n);
        }
    }
    free(initial_route);
    free(route1);
	
	int *retarg = (int*)malloc(sizeof(int)*n);
	memcpy(retarg, route, sizeof(int)*n);
    
	return (Answer){.dist = sum_d1, .route = retarg};
    
}

void make_initial(int n, int* route,int seed){
    int *pattern = (int *)calloc(n,sizeof(int));
    srand((unsigned int)time(NULL) + seed*10);
    int buf = rand()%n;
    for(int i = 0; i < n; i++){
        while(pattern[buf] == 1){
            buf = rand()%n;
        }
        route[i] = buf;
        pattern[buf] = 1;
    }
    free(pattern);
}

double get_sum_distance(const City *city,int n,int *route){
    double sum_d = 0;
	for (int i = 0 ; i < n ; i++){
	    const int c0 = route[i];
	    const int c1 = route[(i+1)%n]; // nは0に戻る
	    sum_d += distance(city[c0],city[c1]);
	}
    return sum_d;
}
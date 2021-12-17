#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "physics2.h"

int main(int argc, char **argv)
{
  const Condition cond =
    {
     .width  = 75,
     .height = 40,
     .G = 1.0,
     .dt = 1.0,
     .cor = 0.8
    };
  
 if(argc != 3){
     printf("the number of argument should be 3\n");
     return -1;
 }
  size_t objnum = atoi(argv[1]);
  char buf[200];
  Object objects[objnum];
  
  int sharp_count = 0;
  FILE *fp = fopen(argv[2],"r");
  if(fp == NULL){
        printf("cannot open file\n");
        return -1;
  }else{
    while(buf[0] == '#'){
        fscanf(fp, "%[^\n]%*1[\n]", buf);
        sharp_count++;
    }
    fseek(fp,0L,SEEK_SET);
    for(int i = 0; i < sharp_count - 1; i++){
        fscanf(fp, "%[^\n]%*1[\n]", buf);
    }
    for(int i = 0; i < objnum; i++){
        //fscanf(fp, "%[^\n]%*1[\n]", buf);
        //sscanf(buf,"%lf %lf %lf %lf %lf\n", &objects[i].m, &objects[i].x, &objects[i].vx, &objects[i].y, &objects[i].vy);
        fscanf(fp,"%lf %lf %lf %lf %lf %[^\n]%*1[\n]", &objects[i].m, &objects[i].x, &objects[i].vx, &objects[i].y, &objects[i].vy, buf);
    }
    fclose(fp);
  }
  // シミュレーション. ループは整数で回しつつ、実数時間も更新する
  const double stop_time = 400;
  double t = 0;
  for (size_t i = 0 ; t <= stop_time ; i++){
    t = i * cond.dt;
    my_update_velocities(objects, objnum, cond);
    my_update_positions(objects, objnum, cond);
    my_bounce(objects, objnum, cond); // 壁があると仮定した場合に壁を跨いでいたら反射させる
    
    // 表示の座標系は width/2, height/2 のピクセル位置が原点となるようにする
    my_plot_objects(objects, objnum, t, cond);
    
    usleep(200 * 1000); // 200 x 1000us = 200 ms ずつ停止
    printf("\e[%dA", cond.height+3);// 壁とパラメータ表示分で3行
  }
  return EXIT_SUCCESS;
}

// 実習: 以下に my_ で始まる関数を実装する
// 最終的に phisics2.h 内の事前に用意された関数プロトタイプをコメントアウト
void my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond){
    for(int y = -1*cond.height/2 -1; y < cond.height/2 + 1 ; y++){
        for(int x = -1*cond.width/2 - 1 ; x < cond.width/2 + 1 ; x++){
            if(x == -1*cond.width/2 - 1){
                if((y == cond.height/2 ) || (y == -1*cond.height/2 - 1)){
                    printf("+");
                }else{
                    printf("|");
                }
            }else if(x == cond.width/2){
                if((y == cond.height/2 ) || (y == -1*cond.height/2 - 1)){
                    printf("+");
                }else{
                    printf("|");
                }
                printf("\n");
            }else if((y == cond.height/2 ) || (y == -1*cond.height/2 - 1)){
                printf("-");
            }else{
                for(int i = 0; i < numobj; i++){
                    if((int)objs[i].x == x && (int)objs[i].y == y){
                        if(i == 0){
                            printf("o");
                        }else{
                            printf("\bo");
                        }
                    }else if(i == 0){
                        printf(" ");
                    }
                }
            }
        }
    }
    printf("t = %3.1lf",t);
    for(int i = 0; i < numobj; i++){
        printf(", objs[%d].x = %7.2lf, objs[%d].y = %7.2lf",i,objs[i].x,i,objs[i].y);
        if(i == (numobj - 1)){
            printf("\n");
        }
    }
}
void my_update_velocities(Object objs[], const size_t numobj, const Condition cond){
    double ax[numobj];
    double ay[numobj];
    double distance = 0;   
    for(int i = 0; i < numobj; i++){
        ax[i] = 0;
        ay[i] = 0;
    }
    for(int i = 0 ; i < numobj; i++){
        objs[i].prev_vy = objs[i].vy;
        objs[i].prev_vx = objs[i].vx;
        for(int j = 0; j < numobj; j++){
            if(i != j){
                distance = sqrt((pow((objs[i].x - objs[j].x),2) + pow((objs[i].y - objs[j].y),2)));
                if(distance >= 1){          //飛びすぎ防止
                    ax[i] += objs[j].m/(pow(distance,3))*(objs[j].x - objs[i].x);
                    ay[i] += objs[j].m/(pow(distance,3))*(objs[j].y - objs[i].y);
                }
            }
        }
         ax[i] *= cond.G;
         ay[i] *= cond.G;
         
         objs[i].vx = objs[i].prev_vx + ax[i]*cond.dt;
         objs[i].vy = objs[i].prev_vy + ay[i]*cond.dt;
    } 
   
}
void my_update_positions(Object objs[], const size_t numobj, const Condition cond){
    for(int i = 0; i < numobj; i++){
        objs[i].prev_y = objs[i].y;
        objs[i].y = objs[i].prev_y + objs[i].prev_vy * cond.dt;
        objs[i].prev_x = objs[i].x;
        objs[i].x = objs[i].prev_x + objs[i].prev_vx * cond.dt;
    }
}
void my_bounce(Object objs[], const size_t numobj, const Condition cond){
    for(int i = 0; i < numobj; i++){
        if((objs[i].y >= cond.height/2) && (objs[i].prev_y < cond.height/2)){
            objs[i].vy = -1 * cond.cor * objs[i].vy;
            objs[i].y = cond.height/2 - (objs[i].y - cond.height/2);
        }
        if((objs[i].y <= -1*cond.height/2) && (objs[i].prev_y > -1*cond.height/2)){
            objs[i].vy = -1 * cond.cor * objs[i].vy;
            objs[i].y = -1 * cond.height/2 - (objs[i].y - -1 * cond.height/2);
        }
        if((objs[i].x >= cond.width/2) && (objs[i].prev_x < cond.width/2)){
            objs[i].vx = -1 * cond.cor * objs[i].vx;
            objs[i].x = cond.width/2 - (objs[i].x - cond.width/2);
        }
        if((objs[i].x <= -1*cond.width/2) && (objs[i].prev_x > -1*cond.width/2)){
            objs[i].vx = -1 * cond.cor * objs[i].vx;
            objs[i].x = -1 * cond.width/2 - (objs[i].x - -1 * cond.width/2);
        }
    }
    for(int i = 0; i < numobj; i++){
        for(int j = 0; j < numobj; j++){
            if(i != j){
                if((objs[i].y == objs[j].y) && (objs[i].x == objs[j].x)){

                }
            }
        }
    }
}



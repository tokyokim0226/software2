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
  
  size_t objnum = 2;
  Object objects[objnum];

  // objects[1] は巨大な物体を画面外に... 地球のようなものを想定
  objects[0] = (Object){ .m = 60.0, .y = -19.9, .vy = 2.0};
  objects[1] = (Object){ .m = 100000.0, .y =  1000.0, .vy = 0.0};

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
                    printf("-+");
                }else{
                    printf("|");
                }
                printf("\n");
            }else if((y == cond.height/2 ) || (y == -1*cond.height/2 - 1)){
                printf("-");
            }else if(x == 0){
                for(int i = 0; i < numobj; i++){
                    if((int)objs[i].y == y){
                        printf("o");
                    }else{
                        printf(" ");
                    }       
                }
            }else{
                printf(" ");
            }
        }
    }
    printf("t = %3.1lf",t);
    for(int i = 0; i < numobj; i++){
        printf(",objs[%d].y = %7.2lf",i,objs[i].y);
        if(i == (numobj - 1)){
            printf("\n");
        }
    }
}
void my_update_velocities(Object objs[], const size_t numobj, const Condition cond){
    double a[numobj];
    for(int i = 0; i < numobj; i++){
        a[i] = 0;
    }
    for(int i = 0 ; i < numobj; i++){
        objs[i].prev_vy = objs[i].vy;
        for(int j = 0; j < numobj; j++){
            if(i != j){
                a[i] += objs[j].m/(pow(fabs(objs[j].y - objs[i].y),3))*(objs[j].y - objs[i].y);
            }
        }
         a[i] *= cond.G;
         
         objs[i].vy = objs[i].prev_vy + a[i]*cond.dt;
    } 
   
}
void my_update_positions(Object objs[], const size_t numobj, const Condition cond){
    for(int i = 0; i < numobj; i++){
        objs[i].prev_y = objs[i].y;
        objs[i].y = objs[i].prev_y + objs[i].prev_vy * cond.dt;
    }
}
void my_bounce(Object objs[], const size_t numobj, const Condition cond){
    //for(int i = 0; i < numobj; i++){
        if((objs[0].y >= cond.height/2 - 1) || (objs[0].y <= -1*cond.height/2)){
            objs[0].vy = -1 * objs[0].prev_vy;
            objs[0].y = objs[0].prev_y;
        }
    //}
}



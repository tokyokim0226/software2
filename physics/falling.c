// どこまでも落ちていく...

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "physics.h" // この中に構造体定義、関数プロトタイプがある

int main(int argc, char **argv)
{
  const Condition cond =
    {
     .width  = 75,
     .height = 40,
     .G = 1.0,
     .dt = 1.0
    };
  
  size_t objnum = 2;
  Object objects[objnum];

  // o[1] は巨大な物体を画面外に... 地球のようなものを想定
  objects[0] = (Object){ .m = 60.0, .y = -20.0, .vy = 0.2};
  objects[1] = (Object){ .m = 100000.0, .y =  1000.0, .vy = 0.0};

  // シミュレーション. ループは整数で回しつつ、実数時間も更新する
  const double stop_time = 400;
  double t = 0;
  for (size_t i = 0 ; t <= stop_time ; i++){
    t = i * cond.dt;
    my_update_velocities(objects, objnum, cond);
    my_update_positions(objects, objnum, cond);

    // 表示の座標系は width/2, height/2 のピクセル位置が原点となるようにする
    my_plot_objects(objects, objnum, t, cond);
    
    usleep(200 * 1000); // 200 x 1000us = 200 ms ずつ停止
    printf("\e[%dA", cond.height+2);// 表示位置を巻き戻す。壁がないのでheight+2行（境界とパラメータ表示分）
  }
  return EXIT_SUCCESS;
}

// 実習: 以下に my_ で始まる関数を実装する
// 最終的には physics.h 内の、こちらが用意したプロトタイプをコメントアウト
void my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond){
    for(int y = -1*cond.height/2; y < cond.height/2 ; y++){
        for(int x = -1*cond.width/2; x < cond.width/2 ; x++){
            if(x == 0){
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
        printf("\n");
    }
    printf("-----\n");
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
        objs[i].y = objs[i].y + objs[i].prev_vy * cond.dt;
    }
}


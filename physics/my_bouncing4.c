#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "physics3.h"

int main(int argc, char **argv)
{
  const Condition cond =
    {
     .width  = 75,
     .height = 40,
     .G = 1.0,
     .dt = 0.05,     //時間幅を狭くした
     .cor = 1       //1に変更した
    };
  
 
  size_t objnum = 3;
  Object objects[objnum];
  
  objects[0] = (Object){ .m = 60.0, .y = -19.9, .vy = 2.0, .x = 0, .vx = 6};
  objects[1] = (Object){ .m = 100000.0, .y =  1000.0, .vy = 0.0, .x = 0, .vx = 0};
  objects[2] = (Object){ .m = 60.0, .y =  -15, .vy = 3.0, .x = 5, .vx = -8};


  Stage stage[5];
  stage[0] = (Stage){.stage_number = 1, .block_number = 2, .block_position_x = {-10,10}, .block_position_y = {-10,10}, .block_height = {5,7}, .block_width = {3,6}};
  stage[1] = (Stage){.stage_number = 1, .block_number = 3, .block_position_x = {-10,10}, .block_position_y = {-10,10}, .block_height = {5,7}, .block_width = {3,6}};
  stage[2] = (Stage){.stage_number = 1, .block_number = 4, .block_position_x = {-10,10}, .block_position_y = {-10,10}, .block_height = {5,7}, .block_width = {3,6}};
  stage[3] = (Stage){.stage_number = 1, .block_number = 5, .block_position_x = {-10,10}, .block_position_y = {-10,10}, .block_height = {5,7}, .block_width = {3,6}};
  stage[4] = (Stage){.stage_number = 1, .block_number = 6, .block_position_x = {-10,10}, .block_position_y = {-10,10}, .block_height = {5,7}, .block_width = {3,6}};
  int selected_stage = 0;

  // シミュレーション. ループは整数で回しつつ、実数時間も更新する
  const double stop_time = 4000;
  double t = 0;
  for (size_t i = 0 ; t <= stop_time ; i++){
    t = i * cond.dt;
    my_update_velocities(objects, objnum, cond);
    my_update_positions(objects, objnum, cond);
    my_bounce(objects, objnum, cond, stage, selected_stage); // 壁があると仮定した場合に壁を跨いでいたら反射させる
    
    // 表示の座標系は width/2, height/2 のピクセル位置が原点となるようにする
    my_plot_objects(objects, objnum, t, cond, stage, selected_stage);
    
    usleep(10 * 1000); // 10 x 100us = 10ms ずつ停止
    printf("\e[%dA", cond.height+3);// 壁とパラメータ表示分で3行
  }
  return EXIT_SUCCESS;
}

// 実習: 以下に my_ で始まる関数を実装する
// 最終的に phisics2.h 内の事前に用意された関数プロトタイプをコメントアウト

void my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond, Stage stage[], int selected_stage){
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
                            printf("\e[30m");
                            printf("\b\e[47m\e[1mO\e[0m");
                        }else{
                            printf("\e[30m");
                            printf("\b\e[47m\e[1mO\e[0m");
                        }
                    }else if(i == 0){
                       
                    }
                }
               for(int j = 0; j < stage[selected_stage].block_number; j++){
                    if((stage[selected_stage].block_position_x[j] <= x) && (stage[selected_stage].block_position_x[j] + stage[selected_stage].block_width[j] >= x)){
                        if((stage[selected_stage].block_position_y[j] <= y) && (stage[selected_stage].block_position_y[j] + stage[selected_stage].block_height[j] >= y)){
                            printf("\b\e[41m \e[0m");
                        }
                    }
                }
            printf("\e[47m \e[0m");            
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
                if(distance > 3){          //飛びすぎ防止
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
void my_bounce(Object objs[], const size_t numobj, const Condition cond, Stage stage[], int selected_stage){
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
        for(int j = 0; j < stage[selected_stage].block_number; j++){
            if(objs[i].prev_y > stage[selected_stage].block_position_y[j] + stage[selected_stage].block_height[j]){
                if((objs[i].x >= stage[selected_stage].block_position_x[j]) && (objs[i].x <= stage[selected_stage].block_position_x[j] + stage[selected_stage].block_width[j]/2)){
                    if(objs[i].y >= stage[selected_stage].block_position_y[j] + stage[selected_stage].block_height[j] -1* (stage[selected_stage].block_height[j]/stage[selected_stage].block_width[j])*(objs[i].x - stage[selected_stage].block_position_x[j])){
                        if(objs[i].y <= stage[selected_stage].block_position_y[j] + stage[selected_stage].block_height[j]){
                            objs[i].vy = -1 * cond.cor * objs[i].vy;                            
                            objs[i].y = stage[selected_stage].block_position_y[j] + stage[selected_stage].block_height[j] + (-1 * objs[i].y + stage[selected_stage].block_position_y[j] + stage[selected_stage].block_height[j]);  
                            objs[i].vx = objs[i].vx;                            
                            objs[i].x = objs[i].x;    
                        }
                    }
                }else if((objs[i].x >= stage[selected_stage].block_position_x[j]+ stage[selected_stage].block_width[j]/2) && (objs[i].x <= stage[selected_stage].block_position_x[j] + stage[selected_stage].block_width[j])){
                    if(objs[i].y >= stage[selected_stage].block_position_y[j] + (stage[selected_stage].block_height[j]/stage[selected_stage].block_width[j])*(objs[i].x - stage[selected_stage].block_position_x[j])){
                        if(objs[i].y <= stage[selected_stage].block_position_y[j] + stage[selected_stage].block_height[j]){
                            objs[i].vy = -1 * cond.cor * objs[i].vy;                            
                            objs[i].y = stage[selected_stage].block_position_y[j] + stage[selected_stage].block_height[j] + (-1 * objs[i].y + stage[selected_stage].block_position_y[j] + stage[selected_stage].block_height[j]);  
                            objs[i].vx = objs[i].vx;                            
                            objs[i].x = objs[i].x;    
                        }
                    }
                }
            }
            if(objs[i].prev_y < stage[selected_stage].block_position_y[j]){
                if((objs[i].x >= stage[selected_stage].block_position_x[j]) && (objs[i].x <= stage[selected_stage].block_position_x[j] + stage[selected_stage].block_width[j]/2)){
                    if(objs[i].y <= stage[selected_stage].block_position_y[j] + (stage[selected_stage].block_height[j]/stage[selected_stage].block_width[j])*(objs[i].x - stage[selected_stage].block_position_x[j])){
                        if(objs[i].y >= stage[selected_stage].block_position_y[j]){
                            objs[i].vy = -1 * cond.cor * objs[i].vy;                            
                            objs[i].y = stage[selected_stage].block_position_y[j] -1* (objs[i].y - stage[selected_stage].block_position_y[j]);  
                            objs[i].vx = objs[i].vx;                            
                            objs[i].x = objs[i].x;    
                        }
                    }
                }else if((objs[i].x >= stage[selected_stage].block_position_x[j]+ stage[selected_stage].block_width[j]/2) && (objs[i].x <= stage[selected_stage].block_position_x[j] + stage[selected_stage].block_width[j])){
                     if(objs[i].y <= stage[selected_stage].block_position_y[j] +stage[selected_stage].block_height[j] -1 * (stage[selected_stage].block_height[j]/stage[selected_stage].block_width[j])*(objs[i].x - stage[selected_stage].block_position_x[j])){
                        if(objs[i].y >= stage[selected_stage].block_position_y[j]){
                            objs[i].vy = -1 * cond.cor * objs[i].vy;                            
                            objs[i].y = stage[selected_stage].block_position_y[j] -1* (objs[i].y - stage[selected_stage].block_position_y[j]); 
                            objs[i].vx = objs[i].vx;                            
                            objs[i].x = objs[i].x;    
                        }
                    }
                }
            }
            if(objs[i].prev_x < stage[selected_stage].block_position_x[j]){
                if((objs[i].y >= stage[selected_stage].block_position_y[j]) && (objs[i].y <= stage[selected_stage].block_position_y[j] + stage[selected_stage].block_height[j]/2)){
                    if(objs[i].x <= stage[selected_stage].block_position_x[j] + (stage[selected_stage].block_width[j]/stage[selected_stage].block_height[j])*(objs[i].y - stage[selected_stage].block_position_y[j])){
                        if(objs[i].x >= stage[selected_stage].block_position_x[j]){
                            objs[i].vy = objs[i].vy;                            
                            objs[i].y = objs[i].y;  
                            objs[i].vx = -1 * cond.cor * objs[i].vx;                            
                            objs[i].x = stage[selected_stage].block_position_x[j] - (objs[i].x - stage[selected_stage].block_position_x[j]);
                        } 
                    }
                }else if((objs[i].y >= stage[selected_stage].block_position_y[j]+ stage[selected_stage].block_height[j]/2) && (objs[i].y <= stage[selected_stage].block_position_y[j] + stage[selected_stage].block_height[j])){
                    if(objs[i].x <= stage[selected_stage].block_position_x[j] -1* (stage[selected_stage].block_width[j]/stage[selected_stage].block_height[j])*(objs[i].y - stage[selected_stage].block_position_y[j] - stage[selected_stage].block_height[j])){
                       if(objs[i].x >= stage[selected_stage].block_position_x[j]){
                            objs[i].vy = objs[i].vy;                            
                            objs[i].y = objs[i].y;  
                            objs[i].vx = -1 * cond.cor * objs[i].vx;                            
                            objs[i].x = stage[selected_stage].block_position_x[j] - (objs[i].x - stage[selected_stage].block_position_x[j]);
                        }
                    }
                }
            }
            if(objs[i].prev_x > stage[selected_stage].block_position_x[j] + stage[selected_stage].block_width[j]){
                if((objs[i].y >= stage[selected_stage].block_position_y[j]) && (objs[i].y <= stage[selected_stage].block_position_y[j] + stage[selected_stage].block_height[j]/2)){
                    if(objs[i].x >= stage[selected_stage].block_position_x[j] -1* (stage[selected_stage].block_width[j]/stage[selected_stage].block_height[j])*(objs[i].y - stage[selected_stage].block_position_y[j] - stage[selected_stage].block_height[j])){
                        if(objs[i].x <= stage[selected_stage].block_position_x[j] + stage[selected_stage].block_width[j]){
                            objs[i].vy = objs[i].vy;                            
                            objs[i].y = objs[i].y;  
                            objs[i].vx = -1 * cond.cor * objs[i].vx;                            
                            objs[i].x = stage[selected_stage].block_position_x[j] + (-1 * objs[i].x + stage[selected_stage].block_position_x[j]);
                        } 
                    }
                }else if((objs[i].y >= stage[selected_stage].block_position_y[j]+ stage[selected_stage].block_height[j]/2) && (objs[i].y <= stage[selected_stage].block_position_y[j] + stage[selected_stage].block_height[j])){
                    if(objs[i].x <= stage[selected_stage].block_position_x[j] + (stage[selected_stage].block_width[j]/stage[selected_stage].block_height[j])*(objs[i].y - stage[selected_stage].block_position_y[j])){
                       if(objs[i].x <= stage[selected_stage].block_position_x[j] + stage[selected_stage].block_width[j]){
                            objs[i].vy = objs[i].vy;                            
                            objs[i].y = objs[i].y;  
                            objs[i].vx = -1 * cond.cor * objs[i].vx;                            
                            objs[i].x = stage[selected_stage].block_position_x[j] + (-1 * objs[i].x + stage[selected_stage].block_position_x[j]);
                        }
                    }
                }
            }
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


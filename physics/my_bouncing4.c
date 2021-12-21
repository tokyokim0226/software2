#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include "physics3.h"

#define BAR_WIDTH 15

int main(int argc, char **argv)
{
  const Condition cond =
    {
     .width  = 75,
     .height = 40,
     .G = 1.0,
     .dt = 0.01,     //時間幅を狭くした
     .cor = 1,     //1に変更した
    };
  
 
  size_t objnum = 3;
  Object objects[objnum];
  

  Stage stage[5];
  stage[0] = (Stage){.block_number = 2, .block_position_x = {-20,5}, .block_position_y = {-10,10}, .block_height = {10,5}, .block_width = {5,10}};
  stage[1] = (Stage){.block_number = 3, .block_position_x = {-20,5,-10}, .block_position_y = {-15,5,10}, .block_height = {10,10,3}, .block_width = {10,8,10}};
  stage[2] = (Stage){.block_number = 4, .block_position_x = {-10,10,5,16}, .block_position_y = {-10,10,3,5}, .block_height = {5,7,4,3}, .block_width = {3,6,5,6}};
  stage[3] = (Stage){.block_number = 7, .block_position_x = {-10,10,5,16,-17,-35,-5}, .block_position_y = {-10,-10,-4,5,-20,5,10}, .block_height = {5,7,4,3,4,10,5}, .block_width = {15,20,5,40,10,20,20}};
  stage[4] = (Stage){.block_number = 5, .block_position_x = {10,-10,15,22,-13}, .block_position_y = {10,10,3,7,-15}, .block_height = {9,12,4,8,6}, .block_width = {3,6,20,6,7}};


  int bar_x = -8;
  init_stage(objects,objnum,cond,stage,bar_x);
  int selected_stage = select_stage(objects,objnum,cond,stage,bar_x);  

  // シミュレーション. ループは整数で回しつつ、実数時間も更新する
  const double stop_time = 4000;
  double t = 0;
  
  char buf;
  for (size_t i = 0 ; t <= stop_time ; i++){
    t = i * cond.dt;
    if (kbhit()) {
        printf("\r      \r");
        buf = getchar();
        if(buf == 'j'){
            bar_x++;
        }else if(buf == 'f'){
            bar_x--;
        }else if(buf == 'q'){
            system("clear\n");
            return 0;
        }
    }
    my_update_velocities(objects, objnum, cond);
    my_update_positions(objects, objnum, cond);
    my_bounce(objects, objnum, cond, stage, selected_stage,bar_x); // 壁があると仮定した場合に壁を跨いでいたら反射させる
    
    // 表示の座標系は width/2, height/2 のピクセル位置が原点となるようにする
    bar_x = my_plot_objects(objects, objnum, t, cond, stage, selected_stage, bar_x);
    my_judge_results(objects,objnum,t,cond,stage,selected_stage,bar_x);
    
    usleep(2 * 100); // 2 x 100us = 0.2ms ずつ停止
    printf("\e[%dA", cond.height+5);// 壁とパラメータ表示分で4行
  }
  return EXIT_SUCCESS;
}

// 実習: 以下に my_ で始まる関数を実装する
// 最終的に phisics2.h 内の事前に用意された関数プロトタイプをコメントアウト

void init_stage(Object objs[], const size_t numobj,const Condition cond, Stage stage[],int bar_x){
  objs[0] = (Object){ .m = 60.0, .y = -19.9, .vy = 2.0, .x = 0, .vx = 6};
  objs[1] = (Object){ .m = 60.0, .y =  -15, .vy = 3.0, .x = 5, .vx = -8};
  objs[2] = (Object){ .m = 100000.0, .y =  1000.0, .vy = 0.0, .x = 0, .vx = 0};
}

int select_stage(Object objs[], const size_t numobj,const Condition cond, Stage stage[],int bar_x){
    int selected_stage = 0;
    char buf;
    while(1){
        my_plot_objects(objs, numobj,0, cond, stage, selected_stage,bar_x);
        printf("\e[%dA\e[K",1);
        printf("selected_stage:%d   select stage(0 ~ 4):\n",selected_stage);
        printf("press j(to increment) or f(to decrement) or y(to start)\n");

        while (1) {
            if (kbhit()) {
                printf("\r    \r");
                buf = getchar();
                if((buf == 'j') && (selected_stage <= 3)){
                    selected_stage++;
                    break;
                }else if((buf == 'f') && (selected_stage >= 1)){
                    selected_stage--;
                    break;
                }else if(buf == 'y'){
                    system("clear\n");
                    return selected_stage;
                }
               
            }
        }
        printf("\e[%dA\e[K", cond.height+5);
    }
}

int my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond, Stage stage[], int selected_stage, int bar_x){
    char buf;
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
            }else if(y == -1 * cond.height/2 - 1){
                printf("-");
            }else if(y == cond.height/2){
                printf(" ");
                if((x >= bar_x) && (x <= bar_x + BAR_WIDTH)){
                    printf("\b=");
                }
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
                    }
                }
               for(int j = 0; j < stage[selected_stage].block_number; j++){
                    if((stage[selected_stage].block_position_x[j] < x) && (stage[selected_stage].block_position_x[j] + stage[selected_stage].block_width[j] > x)){
                        if((stage[selected_stage].block_position_y[j] < y) && (stage[selected_stage].block_position_y[j] + stage[selected_stage].block_height[j] > y)){
                            printf("\b\e[41m \e[0m");
                        }
                    }
                }
            printf("\e[47m \e[0m");            
            }
        }
        if (kbhit()) {
                printf("\r    \r");
                buf = getchar();
                if(buf == 'j'){
                    bar_x++;
                }else if(buf == 'f'){
                    bar_x--;
                }else if(buf == 'q'){
                    system("clear\n");
                    exit(EXIT_SUCCESS);
                }
               
            }
    }
    printf("stage_%d  score = %3.1lf\n",selected_stage,t);
    printf("move right: j , move left: f, quit: q\n");
    return bar_x;
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
void my_bounce(Object objs[], const size_t numobj, const Condition cond, Stage stage[], int selected_stage, int bar_x){
    for(int i = 0; i < numobj; i++){
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
        if((objs[i].y >= cond.height/2) && (objs[i].prev_y < cond.height/2)){
            if((objs[i].x >= bar_x) && (objs[i].x <= bar_x + BAR_WIDTH)){
                objs[i].vy = -1 * cond.cor * objs[i].vy;
                objs[i].y = cond.height/2 - (objs[i].y - cond.height/2);
            }
        }
        for(int j = 0; j < stage[selected_stage].block_number; j++){
            if((objs[i].x >= stage[selected_stage].block_position_x[j]) && (objs[i].x <= stage[selected_stage].block_position_x[j] + stage[selected_stage].block_width[j])){
                if((objs[i].y >= stage[selected_stage].block_position_y[j]) && (objs[i].y <= stage[selected_stage].block_position_y[j] + stage[selected_stage].block_height[j])){
                            
                    //ブロック上に接したとき
                    if((objs[i].prev_x > stage[selected_stage].block_position_x[j]) && (objs[i].prev_x < stage[selected_stage].block_position_x[j] + stage[selected_stage].block_width[j]) && (objs[i].prev_y < stage[selected_stage].block_position_y[j])){
                        objs[i].vy = -1 * cond.cor *  objs[i].vy;
                        objs[i].y = stage[selected_stage].block_position_y[j] - (objs[i].y - stage[selected_stage].block_position_y[j]);
                    }
                    //ブロック下に接したとき
                    else if((objs[i].prev_x > stage[selected_stage].block_position_x[j]) && (objs[i].prev_x < stage[selected_stage].block_position_x[j] + stage[selected_stage].block_width[j]) && (objs[i].prev_y > (stage[selected_stage].block_position_y[j]+stage[selected_stage].block_height[j]))){
                        objs[i].vy = -1 * cond.cor* objs[i].vy;
                        objs[i].y = stage[selected_stage].block_position_y[j] + stage[selected_stage].block_height[j] + (-1 * objs[i].y + stage[selected_stage].block_position_y[j] + stage[selected_stage].block_height[j]);
                    }
                    //ブロック左に接したとき
                    else if((objs[i].prev_y > stage[selected_stage].block_position_y[j]) && (objs[i].prev_y < stage[selected_stage].block_position_y[j] + stage[selected_stage].block_height[j]) && (objs[i].prev_x < stage[selected_stage].block_position_x[j])){
                        objs[i].vx = -1 * cond.cor* objs[i].vx;
                        objs[i].x = stage[selected_stage].block_position_x[j] - (objs[i].x - stage[selected_stage].block_position_x[j]);
                    }
                    //ブロック右に接したとき
                    else if((objs[i].prev_y > stage[selected_stage].block_position_y[j]) && (objs[i].prev_y < stage[selected_stage].block_position_y[j] + stage[selected_stage].block_height[j]) && (objs[i].prev_x > (stage[selected_stage].block_position_x[j]+stage[selected_stage].block_width[j]))){
                        objs[i].vx = -1 * cond.cor* objs[i].vx;
                        objs[i].x = stage[selected_stage].block_position_x[j] + stage[selected_stage].block_width[j]  + (-1*objs[i].x + stage[selected_stage].block_position_x[j] + stage[selected_stage].block_width[j]);
                    }
                }
            }
        }
    }
}

void my_judge_results(Object objs[], const size_t numobj, const double t, const Condition cond, Stage stage[],int num_stage,int bar_x){
    char buf;
    for(int i = 0 ; i  < numobj - 1; i++){ 
        for(int y = -1*cond.height/2 -1; y < cond.height/2 + 1 ; y++){
            for(int x = -1*cond.width/2 - 1 ; x < cond.width/2 + 1 ; x++){
                if(objs[i].y > cond.height/2 + 7){
                    my_plot_objects(objs,numobj,t,cond,stage,num_stage,bar_x);
                    printf("\e[%dA\e[K",1);
                    printf("time you survived was finally %lf\n",t);
                    printf("rechallenge? y or n");
                     while (1) {
                        if (kbhit()) {
                            printf("\r    \r");
                            buf = getchar();
                            if(buf == 'y'){
                                init_stage(objs,numobj,cond,stage,bar_x);
                                select_stage(objs,numobj,cond,stage,bar_x);
                                return;
                            }else if(buf == 'n'){
                                exit(EXIT_SUCCESS);
                            } 
                        }
                    }
                }
            }
        }
    }
}

int kbhit(void)                 // https://hotnews8.net/programming/tricky-code/c-code03 から引用
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sleep()関数を使う
#include <time.h>

void my_init_cells(const int height, const int width, int cell[height][width], FILE* fp){
  int i = 0;
  int data_count = 0;
  if(fp != NULL){
    int tmp1; 
    int tmp2;
    int *x;
    int *y;
    char temp[200];
    double tempnum;
    
    fscanf(fp, "%s %lf", temp, &tempnum);
    while (fscanf(fp, "%d %d", &tmp1, &tmp2) != EOF){
      i++;
    }
    data_count = i; //データの行数
    x = (int*) malloc(sizeof(int) * (data_count));
    y = (int*) malloc(sizeof(int) * (data_count));
    
    fseek(fp,0L,SEEK_SET); 

    fscanf(fp, "%s %lf", temp, &tempnum);
    for (i = 0;i < data_count;i++) {
      fscanf(fp, "%d %d", y + i, x + i);
    }
    for(int j = 0; j < data_count; j++){
      cell[*(x + j)][*(y + j)] = 1;
    }
  }else{
    srand((unsigned int)time(NULL));
    int ran;
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            ran = rand() % 10;
            if(ran == 0){
                cell[y][x] = 1;
            }
        }
    }
  }
};

void my_print_cells(FILE* fp, int gen, const int height, const int width, int cell[height][width]){
  fprintf(fp,"generation = %d\n",gen);
  for(int i = 0; i < height + 2; i++){
    for(int j = 0; j < width + 2; j++){
      if(i == 0){
        if(j == 0){
          fprintf(fp,"+");
        }else if(j == width + 1){
          fprintf(fp,"+\n");
        }else{
          fprintf(fp,"-");
        }
      }else if(i == height + 1){
        if(j == 0){
          fprintf(fp,"+");
        }else if(j == width + 1){
          fprintf(fp,"+\n");
        }else{
          fprintf(fp,"-");
        }
      }else if(j == 0){
        fprintf(fp,"|");
      }else if(j == width + 1){
        fprintf(fp,"|\n");
      }else{
        if(cell[i-1][j-1] == 0){
          fprintf(fp," ");
        }else{
          fprintf(fp,"\e[31m#\e[0m");
        }
      }
    }
  }
  fflush(fp);
};



int my_count_adjacent_cells(int h, int w, const int height, const int width, int (*cell)[width]){
    int count = 0;
    if((cell[h-1][w-1] == 1) & (h-1 >=0) & (w-1 >= 0)){
        count++;
    }
    if((cell[h-1][w] == 1) & (h-1 >=0) ){
        count++;
    }
    if((cell[h-1][w+1] == 1) & (h-1 >=0) & (w+1 < height)){
        count++;
    }
    if((cell[h][w-1] == 1) & (w-1 >= 0)){
        count++;
    }
    if((cell[h][w+1] == 1) & (w+1 < width)){
        count++;
    }
    if((cell[h+1][w-1] == 1) & (h+1 < height) & (w - 1 >= 0)){
        count++;
    }
    if((cell[h+1][w] == 1) & (h+1 < height)){
        count++;
    }
    if((cell[h+1][w+1] == 1) & (h+1 < height) & (w+1 < width)){
        count++;
    }
    return count;
}

void my_update_cells(const int height, const int width, int cell[height][width]){
int tmpcell[height][width];
for(int y = 0; y < height; y++){
    for(int x = 0; x < width; x++){
        tmpcell[y][x] = cell[y][x];
    }
}
int count = 0;
  for(int y = 0 ; y < height ; y++){
    for(int x = 0 ; x < width ; x++){
        count = my_count_adjacent_cells(y,x,height,width,tmpcell);
        if(cell[y][x] == 1){
            if((count == 2) | (count == 3)){
                cell[y][x] = 1;
            }else{
                cell[y][x] = 0;
            }
        }else{
            if(count == 3){
                cell[y][x] = 1;
            }else{
                cell[y][x] = 0;
            }
        }
    }
  }
};


void my_file_cells(int gen,const int height,const int width,int cell[height][width]){           //fileに出力する関数
    if((gen % 100 == 0) & (gen < 10000) & (gen > 0)){
        char filename[20];
        sprintf(filename,"gen%04d.lif",gen);
        FILE *fp = fopen(filename,"w");
        if (fp == NULL) {
            printf("File open failed\n");
            return ;
        }
        fprintf(fp,"#Life 1.06\n");
        for(int y = 0;y < height; y++){
            for(int x = 0; x < width; x++){
                if(cell[y][x] == 1){
                    fprintf(fp,"%d %d\n",x, y);
                }
            }
        }
        fclose(fp);
        
        return;
    }else{
        return ;
    }
};


int main(int argc, char **argv)
{
  FILE *fp = stdout;
  const int height = 40;
  const int width = 70;

  int cell[height][width];
  for(int y = 0 ; y < height ; y++){
    for(int x = 0 ; x < width ; x++){
      cell[y][x] = 0;
    }
  }

  /* ファイルを引数にとるか、ない場合はデフォルトの初期値を使う */
  if ( argc > 2 ) {
    fprintf(stderr, "usage: %s [filename for init]\n", argv[0]);
    return EXIT_FAILURE;
  }
  else if (argc == 2) {
    FILE *lgfile;
    if ( (lgfile = fopen(argv[1],"r")) != NULL ) {
     my_init_cells(height,width,cell,lgfile); // ファイルによる初期化
    }
    else{
      fprintf(stderr,"cannot open file %s\n",argv[1]);
      return EXIT_FAILURE;
    }
    fclose(lgfile);
  }
  else{

    my_init_cells(height, width, cell, NULL); // デフォルトの初期値を使う
  }

  my_print_cells(fp, 0, height, width, cell); // 表示する
  sleep(1); // 1秒休止
  fprintf(fp,"\e[%dA",height+3);//height+3 の分、カーソルを上に戻す(壁2、表示部1)

  /* 世代を進める*/
  for (int gen = 1 ;; gen++) {
    my_update_cells(height, width, cell); // セルを更新
    my_print_cells(fp, gen, height, width, cell);  // 表示する
    my_file_cells(gen,height,width,cell);
    //sleep(1); //1秒休止する
    fprintf(fp,"\e[%dA",height+3);//height+3 の分、カーソルを上に戻す(壁2、表示部1)
  }

  return EXIT_SUCCESS;
}

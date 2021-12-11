#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sleep()関数を使う
#include <time.h>

void my_init_cells(const int height, const int width, int cell[height][width], FILE* fp){
  int data_count = 0;
  if(fp != NULL){
    char temp[200];
    fscanf(fp, "%[^\n]%*1[\n]", temp);
    if(temp[1] == 'L'){
        int tmp1; 
        int tmp2;
        int *x;
        int *y;
        while (fscanf(fp, "%d %d", &tmp1, &tmp2) != EOF){
        data_count++;
        }
       
        x = (int*) malloc(sizeof(int) * (data_count));
        y = (int*) malloc(sizeof(int) * (data_count));
        
        fseek(fp,0L,SEEK_SET); 

        fscanf(fp, "%[^\n]%*1[\n]", temp);
        for (int i = 0;i < data_count;i++) {
        fscanf(fp, "%d %d", x + i, y + i);
        }
        for(int j = 0; j < data_count; j++){
        cell[*(y + j)][*(x + j)] = 1;
        }
    }else{
    int n = 1;
    int obx =0;
    int oby =0;
    char c;
    int x = 0;
    data_count = 0;
    int z = 0;
    int sum_n = 0;
    int dollar_count = 0;

    while(temp[0] == '#'){    
        data_count++;          
        fscanf(fp,"%[^\n]",temp);
        fscanf(fp,"%*1[\n]");
    }
    fseek(fp,0L,SEEK_SET);
    for(int i = 0; i < data_count - 1;i++){
        fscanf(fp,"%[^\n]",temp);
        fscanf(fp,"%*1[\n]");
    }
    fscanf(fp,"%[^1-9]%d%[^1-9]%d%[^\n]",temp,&obx,temp,&oby,temp);
    fscanf(fp,"%*1[\n]");

    while(z != EOF){
        z = fscanf(fp,"%d%c",&n,&c);
        if(z != 2){
            fscanf(fp,"%c",&c);
         }
        sum_n += n;
        if(c == '$'){
            dollar_count++;
            x = 0;
            sum_n = 0;
        }
        if(dollar_count < oby){
            while(x < sum_n){
                if(c == 'o'){
                    cell[dollar_count + 5][x + 1] = 1;
                }
                x++;
            }
        }
        n = 1;
    }
    }
  }else{
    srand((unsigned int)time(NULL));
    int ran = 0;;
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            ran = rand() % 50;
            if(ran == 0 || ran == 10 || ran == 20 || ran == 5 || ran == 15){
                cell[y][x] = 1;
            }else if(ran == 1 || ran == 2 || ran == 3 || ran == 4 || ran == 6 ){
                cell[y][x] = 4;
            }else if(ran == 7){
                cell[y][x] = 5;
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
          if(j > 50){
	        fprintf(fp, "\e[42m \e[0m");
          }else{
            fprintf(fp, "\e[44m \e[0m");
          }
        }else if(cell[i-1][j-1] == 1){
            fprintf(fp,"\e[33m#\e[0m");
        }else if(cell[i-1][j-1] == 4){
            fprintf(fp,"\e[31m#\e[0m");
        }else if(cell[i-1][j-1] == 5){
            fprintf(fp,"\e[30m#\e[0m");
        }
      }
    }
  }
  fflush(fp);
};



int my_count_adjacent_cells_points(int h, int w, const int height, const int width, int tmpcell[height][width]){
    int points = 0;
    for(int y = h - 1; y < h + 2; y ++){
        for(int x = w - 1; x < w + 2 ; x++){
            if((y - 1 < 0) || (y + 1 >= height) || (x - 1 < 0) || (x + 1 >= width) || ((x == w) && (y == h))){
                continue;
            }else{
                points += tmpcell[y][x];
            }
        }
    }
    return points;
}

int my_count_adjacent_cells(int h, int w, const int height, const int width, int tmpcell[height][width]){
    int count = 0;
    for(int y = h - 1; y < h + 2; y ++){
        for(int x = w - 1; x < w + 2 ; x++){
            if((y - 1 < 0) || (y + 1 >= height) || (x - 1 < 0) || (x + 1 >= width) || ((x == w) && (y == h))){
                continue;
            }else{
                if( tmpcell[y][x] >= 1){
                    count++;
                }
            }
        }
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
int points;
  for(int y = 0 ; y < height ; y++){
    for(int x = 0 ; x < width ; x++){
        count = my_count_adjacent_cells(y,x,height,width,tmpcell);
        points = my_count_adjacent_cells_points(y,x,height,width,tmpcell);
        if(tmpcell[y][x] == 1){
            if(x < 50){
                if(points == 1){
                    cell[y][x] = 1;
                }else{
                    cell[y][x] = 0;
                }
            }else if(count >= 3){
                cell[y][x] = 0;
            }
        }else if(tmpcell[y][x] == 4){
            if(x < 50){
                cell[y][x] = 0;
            }else{
                if(points == 2){
                    cell[y][x] = 4;
                }else if(points >= 3){
                    cell[y][x] = 1;
                }else{
                    cell[y][x] = 0;
                }
            }
        }else if(tmpcell[y][x] == 5){
            cell[y][x] = 5;
        }else{
            if(points == 2){
                cell[y][x] = 4;
            }else{
                cell[y][x] = 0;
            }
        }
    }
  }
};


int main(int argc, char **argv)
{
  FILE *fp = stdout;
  const int height = 40;
  const int width = 150;

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
    sleep(1); //1秒休止する
    fprintf(fp,"\e[%dA",height+3);//height+3 の分、カーソルを上に戻す(壁2、表示部1)
  }

  return EXIT_SUCCESS;
}

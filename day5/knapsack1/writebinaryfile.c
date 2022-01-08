#include <stdio.h>
#include <stdlib.h>

int main(int argc, char**argv)
{
    FILE *fp;
    if (argc != 2){
	fprintf(stderr,"usage: %s <txt filename> <binary filename>\n",argv[1]);
	return EXIT_FAILURE;
    }
    int size = 10;

    srand(1);
    double value[10];
    double weight[10];
    for (int i = 0 ; i < 10 ; i++){
        value[i] = 0.1 * (rand() % 200);
        weight[i] = 0.1 * (rand() % 200 + 1);
    }
    // "wb"の'b'はwindows は必須, linux/mac はなくてもよい ("w" でOK)
    if ( (fp = fopen(argv[1],"wb")) == NULL ){
	fprintf(stderr,"%s: cannot open file.\n",argv[1]);
	return EXIT_FAILURE;  
    }
    // 最初にsizeof(size_t) 1個分をサイズ情報として出しておく
    fwrite(&size,sizeof(int),1,fp);
    // dポインタを先頭にからsize個のdoubleデータを出力
    fwrite(value,sizeof(double),size,fp);
    fwrite(weight,sizeof(double),size,fp);
    fclose(fp);
    return EXIT_SUCCESS;
}

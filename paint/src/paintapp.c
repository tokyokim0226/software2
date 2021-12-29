#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h> // for error catch
#include "canvas.h"
#include "command.h"

int main(int argc, char **argv)
{
    //for history recording
    
    const int bufsize = 1000;
    
    // [*]
    History his = (History){ .begin = NULL, .bufsize = bufsize};
    
    int width;
    int height;
    if (argc != 3){
	fprintf(stderr,"usage: %s <width> <height>\n",argv[0]);
	return EXIT_FAILURE;
    } else{
	char *e;
	long w = strtol(argv[1],&e,10);
	if (*e != '\0'){
	    fprintf(stderr, "%s: irregular character found %s\n", argv[1],e);
	    return EXIT_FAILURE;
	}
	long h = strtol(argv[2],&e,10);
	if (*e != '\0'){
	    fprintf(stderr, "%s: irregular character found %s\n", argv[2],e);
	    return EXIT_FAILURE;
	}
	width = (int)w;
	height = (int)h;    
    }
    char pen = '*';
    

    char buf[bufsize];

    Canvas *c = init_canvas(width,height, pen);
    
    printf("\n"); // required especially for windows env

    while(1){
	// [*]
	// hsize はひとまずなし
	// 作る場合はリスト長を調べる関数を作っておく
	print_canvas(c);
	printf("* > ");
	if(fgets(buf, bufsize, stdin) == NULL) break;
	
	const Result r = interpret_command(buf, &his, c);

	if (r == EXIT) break;

	// 返ってきた結果に応じてコマンド結果を表示
	clear_command();
	printf("%s\n",strresult(r));
	// LINEの場合はHistory構造体に入れる
	if (r == LINE) {
	    // [*]
	    push_command(&his,buf);
	}
	
	rewind_screen(2); // command results
	clear_command(); // command itself
	rewind_screen(height+2); // rewind the screen to command input
	
    }
    
    clear_screen();
    free_canvas(c);
    
    return 0;
}


#include <stdio.h>
#include <stdlib.h>

int main(){
    const char *filename = "helloworld.txt";
    FILE *fp;
    if ( (fp = fopen(filename,"rb")) == NULL){
	return EXIT_FAILURE;
    }
    float buf[4];
    
    size_t rsize = fread(buf, sizeof(float), 4, fp); 
    
    printf("%zu read\n", rsize);
    for(int i = 0; i < 4; i++){
        printf("%.200f\n",buf[i]);
    }
    
}
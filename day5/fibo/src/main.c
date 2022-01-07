#include <stdio.h>
#include "fibo.h"

int main(){
    int n = 21;
    long ans = fibo(n);
    printf("%ld\n",ans);
    return 0;
}
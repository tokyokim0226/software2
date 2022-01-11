#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "city.h"

double distance(City a, City b)
{
    const double dx = a.x - b.x;
    const double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

City *load_cities(const char *filename, int *n)
{
    City *city;
    FILE *fp;
    if ((fp=fopen(filename,"rb")) == NULL){
	fprintf(stderr, "%s: cannot open file.\n",filename);
	exit(1);
    }
    fread(n,sizeof(int),1,fp);
    city = (City*)malloc(sizeof(City) * *n);
    for (int i = 0 ; i < *n ; i++){
	fread(&city[i].x, sizeof(int), 1, fp);
	fread(&city[i].y, sizeof(int), 1, fp);
    }
    fclose(fp);
    return city;
}

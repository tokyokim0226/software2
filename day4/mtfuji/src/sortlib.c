#include <stdio.h>
#include <errno.h>
#include <string.h> 
#include <stdlib.h>
#include "func.h"


int comp_alt(const void *p0,const void *p1)
{
  const Data *n0 = (Data *)p0;
  const Data *n1 = (Data *)p1;

  double diff = n0->alt - n1->alt;
  if(diff > 0){
      return 1;
  }else if(diff < 0){
      return -1;
  }else{
      return 0;
  }
}



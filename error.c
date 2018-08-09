#include <stdio.h>
#include "error.h"

int err(char *a){
  printf("%s\n", a);
  return -1;
}

void perr(char *a){
  printf("%s\n", a);
}

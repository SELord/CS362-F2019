/*  noAbortAssert

Steven Lord CS362
11/01/2019
*/

#include <stdio.h>
#include "noAbortAssert.h"
#include <string.h>
#include <stdlib.h>


int noAbortAssert(const void* obj1, const void* obj2, int n){

  int value = memcmp(obj1, obj2, n);

  if (value == 0){
    printf("Call to noAbortAssert:    PASS\n");
  }
  else if(value != 0){
    printf("Call to noAbortAssert:    FAIL\n");
  }

  return 0;

}

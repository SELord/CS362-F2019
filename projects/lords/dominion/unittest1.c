/*****************************************************************************
**    Unit test file for the baron card function in dominion.c
*****************************************************************************/
//#include "dominion.h"
//#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "noAbortAssert.h"
//#include "rngs.h"



int main(){

	int one = 1;
	int two = 2;
	int three = 3;

	noAbortAssert(&one, &two, sizeof(int));

	noAbortAssert(&two, &three, sizeof(int));

	noAbortAssert(&one, &one, sizeof(int));



	return 0;
}

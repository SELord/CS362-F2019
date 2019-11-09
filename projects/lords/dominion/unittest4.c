/*****************************************************************************
**    Unit test file for the tribute card function in dominion.c
*****************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "noAbortAssert.h"
#include "rngs.h"


/*  
    Pre-conditions necessary for Tribute to play:

    First arg = 
    Second arg = 
    Third arg = 
    Fourth arg = 

*/

int checkTribute(int choice1, struct gameState *state, int currentPlayer, int bonus){
	struct gameState control;
	memcpy (&control, state, sizeof(struct gameState));

	int controlBonus = bonus;


	int r = playAmbassador(choice1, state, currentPlayer, &bonus);
    int *ptrR = &r;




    int zero = 0;
    int * ptrZero = &zero;

    printf("Check for return 0\n");
    noAbortAssert(ptrR, ptrZero, sizeof(int));

    printf("Check for bonus increase\n");
    noAbortAssert(&controlBonus, &bonus, sizeof(int));

    printf("Check that discardCount has been changed\n");
    noAbortAssert(&control.discardCount[currentPlayer], &state->discardCount[currentPlayer], sizeof(int));

    printf("Check discarded card\n");
    noAbortAssert(&control.discard[currentPlayer][ control.discardCount[currentPlayer] ], &state->discard[currentPlayer][state->discardCount[currentPlayer]], sizeof(int));

    printf("Check that handCount has been changed\n");
    noAbortAssert(&control.handCount[currentPlayer], &state->handCount[currentPlayer], sizeof(int));


	return 0;
}


int main(){
	int choice1;
    int numPlayers = 2;
    int p = 0;
    int bonus = 0;
    int seed = 1000;
	struct gameState G, testState;

    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};

    //initialize the game state
    initializeGame(numPlayers, k, seed, &G);


    printf("\n_-_-_-_-_-_-_-_-_-_-_-_-_- playBaron Test Suite -_-_-_-_-_-_-_-_-_-_-_-_-_\n\n");

    printf("\n\n*****    TEST 1: choice1 == 1, estate in hand    *****\n");

    choice1 = 1;

    memcpy(&testState, &G, sizeof(struct gameState));

    //testState.hand[0][0] = baron;
    testState.hand[0][0] = estate;

    checkAmbassador(choice1, &testState, p, bonus);





    printf("\n\n*****    TEST 2: choice1 == 1, estate in hand, check for bad iterator    *****\n");

    choice1 = 1;

    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = baron;
    testState.hand[0][1] = estate;
    testState.hand[0][2] = copper;
    testState.hand[0][3] = copper;
    testState.hand[0][4] = copper;

    checkBaron(choice1, &testState, p, bonus);





    printf("\n\n*****    TEST 3: choice1 == 1, estate NOT in hand    *****\n");

    choice1 = 1;

    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = baron;
    testState.hand[0][1] = copper;
    testState.hand[0][2] = copper;
    testState.hand[0][3] = copper;
    testState.hand[0][4] = copper;

    checkBaron(choice1, &testState, p, bonus);





    printf("\n\n*****    TEST 4: choice1 == 0    *****\n");

    choice1 = 0;

    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = baron;
    testState.hand[0][1] = estate;

    checkBaron(choice1, &testState, p, bonus);

	



	return 0;
}

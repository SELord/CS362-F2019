/*****************************************************************************
**    Unit test file for the mine card function in dominion.c
*****************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "noAbortAssert.h"
#include "rngs.h"


/*  
    Pre-conditions necessary for Mine to play:

    (int choice1, int choice2, int handPos, struct gameState *state, int currentPlayer)

    First arg = choice1 : which treasure card to trash
    Second arg = choice2 : which treasure card to gain 
    Third arg = handPos of mine card
    Fourth arg = game state
    Fifth arg = current player

*/



int main(){
	int handPos, j, r, i, choice1, choice2;
    int zero = 0;
    int negOne = -1;
    int *ptrR = &r;
    int numPlayers = 2;
    int p = 0;
    int seed = 1000;
    struct gameState G, testState, control;

    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, tribute, great_hall};

    //initialize the game state
    initializeGame(numPlayers, k, seed, &G);


    printf("\n_-_-_-_-_-_-_-_-_-_-_-_-_- playMine Test Suite -_-_-_-_-_-_-_-_-_-_-_-_-_\n\n");

    printf("\n\n*****    TEST 1: choice1 is not a treasure    *****\n");


    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = mine;
    testState.hand[0][1] = estate;
    testState.hand[0][2] = estate;
    testState.hand[0][3] = copper;
    testState.hand[0][4] = copper;
    handPos = 0;
    choice1 = 1;  //choice1 is an estate
    choice2 = copper;

    memcpy(&control, &testState, sizeof(struct gameState));


    r = playMine(choice1, choice2, handPos, &testState, p);


    printf("Check for return code -1\n");
    noAbortAssert(ptrR, &negOne, sizeof(int));






    printf("\n\n*****    TEST 2: choice1 == treasure, choice2 != treasure    *****\n");


    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = mine;
    testState.hand[0][1] = estate;
    testState.hand[0][2] = estate;
    testState.hand[0][3] = copper;
    testState.hand[0][4] = copper;
    handPos = 0;
    choice1 = 3;  //choice1 is a treasure
    choice2 = estate;  //choice2 is NOT a treasure

    memcpy(&control, &testState, sizeof(struct gameState));


    r = playMine(choice1, choice2, handPos, &testState, p);



    printf("Check for return code -1\n");
    noAbortAssert(ptrR, &negOne, sizeof(int));





    printf("\n\n*****    TEST 3: choice1 == treasure, choice2 == treasure costing more than 3 more    *****\n");


    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = mine;
    testState.hand[0][1] = estate;
    testState.hand[0][2] = estate;
    testState.hand[0][3] = copper;
    testState.hand[0][4] = copper;
    handPos = 0;
    choice1 = 3;  //choice1 is a treasure
    choice2 = gold;  //choice2 is a treasure costing more than allowed

    memcpy(&control, &testState, sizeof(struct gameState));


    r = playMine(choice1, choice2, handPos, &testState, p);




    printf("Check for return code -1\n");
    noAbortAssert(ptrR, &negOne, sizeof(int));






    printf("\n\n*****    TEST 4: Positive test case: choice1 == treasure, choice2 == treasure costing exactly 3 more    *****\n");


    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = mine;
    testState.hand[0][1] = estate;
    testState.hand[0][2] = estate;
    testState.hand[0][3] = copper;
    testState.hand[0][4] = copper;
    handPos = 0;
    choice1 = 3;  //choice1 is a treasure
    choice2 = silver;  //choice2 is a treasure costing more than allowed

    memcpy(&control, &testState, sizeof(struct gameState));


    r = playMine(choice1, choice2, handPos, &testState, p);

    j = control.hand[p][choice1];

    //gain new treasure card to hand
    control.hand[p][control.handCount[p]] = choice2;
    control.handCount[p]++;


    //discard mine card from deck
    control.discard[p][control.discardCount[p]] = control.hand[p][handPos];
    control.discardCount[p]++;  //increment the count of cards in players discard pile
    control.hand[p][handPos] = -1;
    control.hand[p][handPos] = control.hand[p][ (control.handCount[p] - 1)];
    control.hand[p][control.handCount[p] - 1] = -1;
    control.handCount[p]--;


    //trash the choice1 treasure card
    for (i = 0; i < control.handCount[p]; i++)
    {
        if (control.hand[p][i] == j)
        {
            //trash card
            control.hand[p][i] = -1;
            control.hand[p][i] = control.hand[p][ (control.handCount[p] - 1)];
            control.hand[p][control.handCount[p] - 1] = -1;
            control.handCount[p]--;

            break;
        }
    }



    printf("Check for return code 0\n");
    noAbortAssert(ptrR, &zero, sizeof(int));


    printf("Check that treasure was added to hand\n");
    noAbortAssert(&control.hand[p][i], &testState.discard[p][i], sizeof(int));


    printf("Check that handCount is correct\n");
    noAbortAssert(&control.handCount[p], &testState.handCount[p], sizeof(int));


    printf("Check that mine was discarded\n");
    noAbortAssert(&control.discard[p][ control.discardCount[p] ], &testState.discard[p][testState.discardCount[p]], sizeof(int));


	return 0;
}

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
    (int handPos, struct gameState *state, int currentPlayer, int nextPlayer, int *tributeRevealedCards, int *bonus)
    First arg = the hand position of the tribute card played
    Second arg = the game state
    Third arg = the current player
    Fourth arg = the next player
    Fifth arg = pointer to ints of the revealed cards
    sixth arg = pointer to bonus
*/



int main(){
    int j, r;
    int *ptrR = &r;
    int numPlayers = 2;
    int p = 0;
    int nextP = p + 1;
    int seed = 1000;
	struct gameState G, testState, control;

    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, tribute, great_hall};

    //initialize the game state
    initializeGame(numPlayers, k, seed, &G);


    printf("\n_-_-_-_-_-_-_-_-_-_-_-_-_- playTribute Test Suite -_-_-_-_-_-_-_-_-_-_-_-_-_\n\n");

    printf("\n\n*****    TEST 1: nextPlayer doesn't have any cards in discard or deck    *****\n");



    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = tribute;

    testState.deckCount[nextP] = 0;
    testState.discardCount[nextP] = 0;

    memcpy(&control, &testState, sizeof(struct gameState));


    tributeHandler(state, currentPlayer, nextPlayer);




    printf("Check that the Tribute card was discarded after being played\n");
    noAbortAssert(&control.hand[0][0], &testState.discard[p][testState.discardCount[p]], sizeof(int));





    printf("\n\n*****    TEST 2: nextPlayer has only 1 card in deck and discard and it is a treasure   *****\n");


    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = tribute;

    //zero out next players deck
    for (j = 0; j < 10; j++){
        testState.deck[nextP][j] = -1;
        
    }
    testState.deckCount[nextP] = 0;

    //Set nextPlayer's deck to 1 card
    testState.deck[nextP][testState.deckCount[nextP]] = copper;
    testState.deckCount[nextP]++;


    memcpy(&control, &testState, sizeof(struct gameState));


    tributeHandler(state, currentPlayer, nextPlayer);

    control.coins = 2;

    //Discard played Tribute card
    control.discard[p][control.discardCount[p]] = control.hand[p][handPos];
    control.discardCount[p]++;  //increment the count of cards in players discard pile
    control.hand[p][handPos] = -1;
    control.hand[p][handPos] = control.hand[p][ (control.handCount[p] - 1)];
    control.hand[p][control.handCount[p] - 1] = -1;
    control.handCount[p]--;



    printf("Check for coin increase\n");
    noAbortAssert(&control.coins, &testState.coins, sizeof(int));

    printf("Check discarded card\n");
    noAbortAssert(&control.discard[p][ control.discardCount[p] ], &testState.discard[p][testState.discardCount[p]], sizeof(int));






    printf("\n\n*****    TEST 3: nextPlayer has only 1 card in deck and discard and it is a victory card   *****\n");


    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = tribute;

    //zero out next players deck
    for (j = 0; j < 10; j++){
        testState.deck[nextP][j] = -1;
        
    }
    testState.deckCount[nextP] = 0;

    //Set nextPlayer's deck to 1 card
    testState.deck[nextP][testState.deckCount[nextP]] = province;
    testState.deckCount[nextP]++;


    memcpy(&control, &testState, sizeof(struct gameState));


    tributeHandler(state, currentPlayer, nextPlayer);

    control.coins = 0;


    //Discard played Tribute card
    control.discard[p][control.discardCount[p]] = control.hand[p][handPos];
    control.discardCount[p]++;  //increment the count of cards in players discard pile
    control.hand[p][handPos] = -1;
    control.hand[p][handPos] = control.hand[p][ (control.handCount[p] - 1)];
    control.hand[p][control.handCount[p] - 1] = -1;
    control.handCount[p]--;

    control.handCount[p] += 2;



    printf("Check that coins have not increased\n");
    noAbortAssert(&control.coins, &testState.coins, sizeof(int));

    printf("Check that handCount has two more\n");
    noAbortAssert(&control.handCount[p], &testState.handCount[p], sizeof(int));

    printf("Check discarded card\n");
    noAbortAssert(&control.discard[p][ control.discardCount[p] ], &testState.discard[p][testState.discardCount[p]], sizeof(int));






    printf("\n\n*****    TEST 4: nextPlayer has only 1 card in deck and discard and it is an action card   *****\n");


    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = tribute;

    //zero out next players deck
    for (j = 0; j < 10; j++){
        testState.deck[nextP][j] = -1;
        
    }
    testState.deckCount[nextP] = 0;

    //Set nextPlayer's deck to 1 card
    testState.deck[nextP][testState.deckCount[nextP]] = mine;
    testState.deckCount[nextP]++;


    memcpy(&control, &testState, sizeof(struct gameState));


    tributeHandler(state, currentPlayer, nextPlayer);



    //Discard played Tribute card
    control.discard[p][control.discardCount[p]] = control.hand[p][handPos];
    control.discardCount[p]++;  //increment the count of cards in players discard pile
    control.hand[p][handPos] = -1;
    control.hand[p][handPos] = control.hand[p][ (control.handCount[p] - 1)];
    control.hand[p][control.handCount[p] - 1] = -1;
    control.handCount[p]--;

    control.numActions += 2;



    printf("Check that numActions has increased\n");
    noAbortAssert(&control.numActions, &testState.numActions, sizeof(int));

    printf("Check discarded card\n");
    noAbortAssert(&control.discard[p][ control.discardCount[p] ], &testState.discard[p][testState.discardCount[p]], sizeof(int));






    printf("\n\n*****    TEST 5: positive test case: nextPlayer has two different cards in deck/discard: treasure/victory    *****\n");


    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = tribute;

    //zero out next players deck
    for (j = 0; j < 10; j++){
        testState.deck[nextP][j] = -1;
        
    }
    testState.deckCount[nextP] = 0;

    //Set nextPlayer's deck to 2 cards
    testState.deck[nextP][testState.deckCount[nextP]] = copper;
    testState.deckCount[nextP]++;
    testState.deck[nextP][testState.deckCount[nextP]] = province;
    testState.deckCount[nextP]++;


    memcpy(&control, &testState, sizeof(struct gameState));


    tributeHandler(state, currentPlayer, nextPlayer);


    control.coins = 2;

    //Discard played Tribute card
    control.discard[p][control.discardCount[p]] = control.hand[p][handPos];
    control.discardCount[p]++;  //increment the count of cards in players discard pile
    control.hand[p][handPos] = -1;
    control.hand[p][handPos] = control.hand[p][ (control.handCount[p] - 1)];
    control.hand[p][control.handCount[p] - 1] = -1;
    control.handCount[p]--;

    control.handCount[p] += 2;



    printf("Check for coin increase\n");
    noAbortAssert(&control.coins, &testState.coins, sizeof(int));

    printf("Check that handCount has two more\n");
    noAbortAssert(&control.handCount[p], &testState.handCount[p], sizeof(int));

    printf("Check discarded card\n");
    noAbortAssert(&control.discard[p][ control.discardCount[p] ], &testState.discard[p][testState.discardCount[p]], sizeof(int));








    printf("\n\n*****    TEST 6: positive test case: nextPlayer has two different cards in deck/discard: victory/action    *****\n");


    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = tribute;

    //zero out next players deck
    for (j = 0; j < 10; j++){
        testState.deck[nextP][j] = -1;
        
    }
    testState.deckCount[nextP] = 0;

    //Set nextPlayer's deck to 2 cards
    testState.deck[nextP][testState.deckCount[nextP]] = province;
    testState.deckCount[nextP]++;
    testState.deck[nextP][testState.deckCount[nextP]] = mine;
    testState.deckCount[nextP]++;


    memcpy(&control, &testState, sizeof(struct gameState));


    tributeHandler(state, currentPlayer, nextPlayer);


    //Discard played Tribute card
    control.discard[p][control.discardCount[p]] = control.hand[p][handPos];
    control.discardCount[p]++;  //increment the count of cards in players discard pile
    control.hand[p][handPos] = -1;
    control.hand[p][handPos] = control.hand[p][ (control.handCount[p] - 1)];
    control.hand[p][control.handCount[p] - 1] = -1;
    control.handCount[p]--;

    control.handCount[p] += 2;
    control.numActions += 2;
    control.playedCardCount = 0;



    printf("Check that coins have not increased\n");
    noAbortAssert(&control.coins, &testState.coins, sizeof(int));

    printf("Check that handCount has two more\n");
    noAbortAssert(&control.handCount[p], &testState.handCount[p], sizeof(int));

    printf("Check that numActions has increased\n");
    noAbortAssert(&control.numActions, &testState.numActions, sizeof(int));

    printf("Check discarded card\n");
    noAbortAssert(&control.discard[p][ control.discardCount[p] ], &testState.discard[p][testState.discardCount[p]], sizeof(int));

    printf("Check that playedCards has not been incrememented\n");
    noAbortAssert(&control.playedCardCount, &testState.playedCardCount, sizeof(int));




	return 0;
}

/*****************************************************************************
**    Unit test file for the baron card function in dominion.c
*****************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "noAbortAssert.h"
#include "rngs.h"


/*  
    Pre-conditions necessary for Baron to play:
    First arg = an integer where >0 means player will discard a card and <0, player will not discard
    Second arg = the current game state
    Third arg = int representing the current player
    Fourth arg = int representing any bonus
    gameState numBuys is set to correct amount for the start of this turn
    The hand of the current player is filled for this turn
    handCount is setup
    Player's discard is set for this turn
    discardCount is set for this turn
    supplyCount for the Baron card must be set to an appropriate int
    Deck for the player must be set up
    deckCount for the player must be set
*/

int checkBaron(int choice1, struct gameState *state, int currentPlayer){
	struct gameState control;
	memcpy (&control, state, sizeof(struct gameState));


	baronHandler(state, choice1, currentPlayer);

    control.numBuys++;


    if(choice1 >0){
    	int estateFlag = 0;
    	for(int x = 0; x < control.handCount[currentPlayer]; x++){
    		if(control.hand[currentPlayer][x] == estate){
    			control.coins += 4;
    			control.discard[currentPlayer][control.discardCount[currentPlayer]] = control.hand[currentPlayer][x];
                control.discardCount[currentPlayer]++;  //increment the count of cards in players discard pile
    
			    control.hand[currentPlayer][x] = -1;
		        control.hand[currentPlayer][x] = control.hand[currentPlayer][ (control.handCount[currentPlayer] - 1)];
		        control.hand[currentPlayer][control.handCount[currentPlayer] - 1] = -1;
		        control.handCount[currentPlayer]--;

		        estateFlag = 1;

		        break;
    		}
    	}

    	if(estateFlag == 0){
    		//gain estate to discard pile
    		control.discard[currentPlayer][ control.discardCount[currentPlayer] ] = estate;
	        control.discardCount[currentPlayer]++;
		    control.supplyCount[estate]--;
    	}
    }
    else{
    	//gain estate to discard pile
		control.discard[currentPlayer][ control.discardCount[currentPlayer] ] = estate;
        control.discardCount[currentPlayer]++;
	    control.supplyCount[estate]--;
    }


    printf("Check for coin increase\n");
    noAbortAssert(control.coins, &state->coins, sizeof(int));
    
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

    checkBaron(choice1, &testState, p);





    printf("\n\n*****    TEST 2: choice1 == 1, estate in hand, check for bad iterator    *****\n");

    choice1 = 1;

    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = baron;
    testState.hand[0][1] = estate;
    testState.hand[0][2] = copper;
    testState.hand[0][3] = copper;
    testState.hand[0][4] = copper;

    checkBaron(choice1, &testState, p);





    printf("\n\n*****    TEST 3: choice1 == 1, estate NOT in hand    *****\n");

    choice1 = 1;

    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = baron;
    testState.hand[0][1] = copper;
    testState.hand[0][2] = copper;
    testState.hand[0][3] = copper;
    testState.hand[0][4] = copper;

    checkBaron(choice1, &testState, p);





    printf("\n\n*****    TEST 4: choice1 == 0    *****\n");

    choice1 = 0;

    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = baron;
    testState.hand[0][1] = estate;

    checkBaron(choice1, &testState, p);

	



	return 0;
}
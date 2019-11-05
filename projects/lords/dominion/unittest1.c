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

int checkBaron(int choice1, struct gameState *state, int currentPlayer, int bonus){
	struct gameState control;
	memcpy (&control, state, sizeof(struct gameState));

	int controlBonus = bonus;


	int r = playBaron(choice1, state, currentPlayer, &bonus);


    control.numBuys++;

    if(choice1 >0){
    	int estateFlag = 0;
    	for(int x = 0; x < control.handCount[currentPlayer]; x++){
    		if(control.hand[currentPlayer][x] == estate){
    			controlBonus += 4;
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


    noAbortAssert(&r, 0, sizeof(int));
    noAbortAssert(&controlBonus, &bonus, sizeof(int));
    noAbortAssert(&control.discardCount[currentPlayer], &state->discardCount[currentPlayer], sizeof(int));
    noAbortAssert(&control.handCount[currentPlayer], &state->handCount[currentPlayer], sizeof(int));


	return 0;
}


int main(){
	int choice1, p, bonus;
	struct gameState G;

    p = 0;
    choice1 = 1;
    bonus = 0;
    G.numBuys = 1;
    G.hand[0][0] = copper;
    G.hand[0][1] = baron;
    G.hand[0][2] = copper;
    G.hand[0][3] = estate;
    G.hand[0][4] = copper;

    G.handCount[0] = 5;

    G.discardCount[0] = 0;

    G.supplyCount[baron] = 10;

    G.deck[0][0] = silver;
    G.deck[0][1] = copper;

    G.deckCount[0] = 2;
    

    printf ("Testing playBaron.\n");

    checkBaron(choice1, &G, p, bonus);

    printf ("****  TEST COMPLETE  ****\n");

	



	return 0;
}

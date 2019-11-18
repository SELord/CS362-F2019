#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "noAbortAssert.h"
#include <time.h>




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



int checkTribute(int choice1, int choice2, struct gameState *state, int currentPlayer, int handPos, int bonus){
	struct gameState control;
	memcpy (&control, state, sizeof(struct gameState));

	int controlBonus = bonus;


	int r = playTribute(choice1, choice2, state, currentPlayer, handPos, &bonus);
    int *ptrR = &r;


    


    int zero = 0;
    int * ptrZero = &zero;

    printf("Check for return value of 0\n");
    noAbortAssert(ptrR, ptrZero, sizeof(int));

    printf("Check for bonus increase\n");
    noAbortAssert(&controlBonus, &bonus, sizeof(int));


    int i;
    for(i = 0; i < control.numPlayers; i++){
        printf("Check player #%d handCount\n", i);
        noAbortAssert(&control.handCount[i], &state->handCount[i], sizeof(int));

        printf("Check player #%d deckCount\n", i);
        noAbortAssert(&control.deckCount[i], &state->deckCount[i], sizeof(int));

        printf("Check player #%d discardCount\n", i);
        noAbortAssert(&control.discardCount[i], &state->discardCount[i], sizeof(int));
    }


	return 0;
}






int main(){
    int i, j, n, currentPlayer, choice1, choice2, handPos, bonus, numPlayers, randomHandCount, randomDeckCount, randomDiscardCount, randomHandPos;

    int masterCardList[27] = {curse, estate, duchy, province, copper, silver, gold, adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall, minion, steward, tribute, ambassador, cutpurse, embargo, outpost, salvager, sea_hag, treasure_map};


    struct gameState G;

    printf("\n_-_-_-_-_-_-_-_-_-_-_-_-_- playTribute Random Testing -_-_-_-_-_-_-_-_-_-_-_-_-_\n\n");

    srand(time(NULL));

    for (n = 0; n < 100; n++) {
        numPlayers = (rand() % (MAX_PLAYERS-1))+2;     //Random number of players from 2 to 4
        currentPlayer = (rand() % (numPlayers - 1));    //random number from 0 to 3, representing 1 of 4 players
        choice1 = (rand() % 2);    //random either 0 or 1
        choice2 = (rand() % 2);    //random either 0 or 1
        bonus = rand() % 100;
        randomDeckCount = (rand() % (MAX_DECK+1));
        randomHandCount = (rand() % (MAX_HAND+1));
        randomDiscardCount = randomDeckCount - (rand() % ((MAX_DECK/2)+1));
        randomHandPos = (rand() % randomHandCount+1);

        //random game state
        for (i = 0; i < sizeof(struct gameState); i++) {
            ((char*)&G)[i] = rand() % 250;
        }

        //set whoseTurn
        G.whoseTurn = currentPlayer;

        //set numActions
        G.numActions = rand() % 100;

        //set number of players
        G.numPlayers = numPlayers;


        //set numBuys
        G.numBuys = rand() % 500;

        //set all players deck
        for (i = 0; i < numPlayers; i++)
        {
	        G.deckCount[i] = 0;
	        for (j = 0; j < randomDeckCount; j++)
	        {
	            G.deck[i][j] = masterCardList[(rand() % 27)];
	            G.deckCount[i]++;
	        }
        }


        //setup all player hands
        for (i = 0; i < numPlayers; i++)
        {
	        G.handCount[i] = 0;
	        for (j = 0; j < randomHandCount; j++)
	        {
	            G.hand[i][j] = masterCardList[(rand() % 27)];
	            G.handCount[i]++;
	        }
	    }

        //set a specific handPos with minion just for the currentPlayer
        G.hand[currentPlayer][randomHandPos] = minion;
        handPos = randomHandPos; 


        //set player discard
        for (i = 0; i < numPlayers; i++)
        {
	        G.discardCount[i] = 0;
	        for (j = 0; j < randomDiscardCount; j++)
	        {
	            G.discard[i][j] = masterCardList[(rand() % 27)];
	            G.discardCount[i]++;
	        }
	    }


        checkTribute(choice1, choice2, &G, currentPlayer, handPos, bonus);
    }

    printf("\n_-_-_-_-_-_-_-_-_-_-_-_-_- End Of playTribute Testing -_-_-_-_-_-_-_-_-_-_-_-_-_\n\n");



    return 0;
}
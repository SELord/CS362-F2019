#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "noAbortAssert.h"
#include <time.h>

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
    noAbortAssert(&control.coins, &state->coins, sizeof(int));

    printf("Check that discardCount has been changed\n");
    noAbortAssert(&control.discardCount[currentPlayer], &state->discardCount[currentPlayer], sizeof(int));

    printf("Check discarded card\n");
    noAbortAssert(&control.discard[currentPlayer][ control.discardCount[currentPlayer] ], &state->discard[currentPlayer][state->discardCount[currentPlayer]], sizeof(int));

    printf("Check that handCount has been changed\n");
    noAbortAssert(&control.handCount[currentPlayer], &state->handCount[currentPlayer], sizeof(int));


    return 0;
}







int main(){
    int i, j, n, currentPlayer, choice1, bonus, numPlayers, randomHandCount, randomDeckCount, randomDiscardCount;

    int masterCardList[27] = {curse, estate, duchy, province, copper, silver, gold, adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall, minion, steward, tribute, ambassador, cutpurse, embargo, outpost, salvager, sea_hag, treasure_map};


    struct gameState G;

    printf("\n_-_-_-_-_-_-_-_-_-_-_-_-_- playBaron Random Testing -_-_-_-_-_-_-_-_-_-_-_-_-_\n\n");

    srand(time(NULL));

    for (n = 0; n < 10000; n++) {
        numPlayers = (rand() % 3)+2;     //Random number of players from 2 to 4
        currentPlayer = (rand() % (numPlayers - 1));    //random number from 0 to 3, representing 1 of 4 players
        choice1 = (rand() % 2);    //random either 0 or 1
        bonus = rand();
        randomDeckCount = (rand() % (MAX_DECK+1));
        randomHandCount = (rand() % (MAX_HAND+1));
        randomDiscardCount = (rand() % (MAX_DECK+1));

        //random game state
        for (i = 0; i < sizeof(struct gameState); i++) {
            ((char*)&G)[i] = rand() % 256;
        }

        //set numBuys
        G.numBuys = rand();

        //set coin count
        G.coins = rand();

        //set player deck
        G.deckCount[currentPlayer] = 0;
        for (j = 0; j < randomDeckCount; j++)
        {
            G.deck[currentPlayer][j] = masterCardList[(rand() % 27)];
            G.deckCount[currentPlayer]++;
        }


        //setup player hand
        G.handCount[currentPlayer] = 0;
        for (j = 0; j < randomHandCount; j++)
        {
            G.hand[currentPlayer][j] = masterCardList[(rand() % 27)];
            G.handCount[currentPlayer]++;
        }

        //set player discard
        G.discardCount[currentPlayer] = 0;
        for (j = 0; j < randomDiscardCount; j++)
        {
            G.discard[currentPlayer][j] = masterCardList[(rand() % 27)];
            G.discardCount[currentPlayer]++;
        }

        G.supplyCount[estate] = rand()%(MAX_DECK+1);


        checkBaron(choice1, &G, currentPlayer, bonus);
    }

    printf("\n_-_-_-_-_-_-_-_-_-_-_-_-_- End Of playBaron Testing -_-_-_-_-_-_-_-_-_-_-_-_-_\n\n");



    return 0;
}

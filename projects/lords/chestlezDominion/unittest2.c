/*****************************************************************************
**    Unit test file for the minion card function in dominion.c
*****************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "noAbortAssert.h"
#include "rngs.h"


/*  
    Pre-conditions necessary for Minion to play:
    (int choice1, int choice2, struct gameState *state, int currentPlayer, int handPos, int *bonus)
    First arg = is the choice of whether the player wants 2 coins or not. 1 = get 2 coins. 0 = Don't get 2 coins
    Second arg = is the choice of whether the player wants to discard hand and get 4 new cards. 1 = discard, 0 = don't discard
    Third arg = is the gameState struct
    Fourth arg = is the current player. The one playing the minion card
    Fifth arg = the hand position of the minion card being played
    Sixth arg = pointer to bonus to capture the 2 coins if that is selected
   
*/

int checkMinion(int choice1, int choice2, struct gameState *state, int currentPlayer, int handPos){
	struct gameState control;
	memcpy (&control, state, sizeof(struct gameState));


    minionHandler(state, choice1, choice2, currentPlayer, handPos);


    if(choice1 == 1){
        //get two coins and discard minion since it has been played
        control.coins += 2;

        control.discard[currentPlayer][control.discardCount[currentPlayer]] = control.hand[currentPlayer][handPos];
        control.discardCount[currentPlayer]++;  //increment the count of cards in players discard pile

        control.hand[currentPlayer][handPos] = -1;
        control.hand[currentPlayer][handPos] = control.hand[currentPlayer][ (control.handCount[currentPlayer] - 1)];
        control.hand[currentPlayer][control.handCount[currentPlayer] - 1] = -1;
        control.handCount[currentPlayer]--;
    }
    else if (choice2 == 1){
        while (control.handCount[currentPlayer] > 0){
            //discard each card
            control.discard[currentPlayer][control.discardCount[currentPlayer]] = control.hand[currentPlayer][0];
            control.discardCount[currentPlayer]++;  //increment the count of cards in players discard pile
            control.hand[currentPlayer][0] = -1;
            control.hand[currentPlayer][0] = control.hand[currentPlayer][ (control.handCount[currentPlayer] - 1)];
            control.hand[currentPlayer][control.handCount[currentPlayer] - 1] = -1;
            control.handCount[currentPlayer]--;
        }
        //draw 4 cards for current player
        for(int x = 0; x < 4; x++){
            //assume there are enough cards in player's deck
            control.hand[currentPlayer][control.handCount[currentPlayer]] = control.deck[currentPlayer][control.deckCount[currentPlayer] - 1];//Add card to end of hand
            control.deckCount[currentPlayer]--;  //Decrement deck count
            control.handCount[currentPlayer]++;  //Increment hand count
        }
        //other players discard hand and redraw if hand size > 4
        for (int otherPlayer = 1; otherPlayer < control.numPlayers; otherPlayer++)
        {
            //otherPlayer = 0 is current player. skip current play in this portion since we already did it above
            if ( control.handCount[otherPlayer] > 4 )
            {
                //discard hand
                while( control.handCount[otherPlayer] > 0 )
                {
                    control.discard[otherPlayer][control.discardCount[otherPlayer]] = control.hand[otherPlayer][0];
                    control.discardCount[otherPlayer]++;  //increment the count of cards in players discard pile
                    control.hand[otherPlayer][0] = -1;
                    control.hand[otherPlayer][0] = control.hand[otherPlayer][ (control.handCount[otherPlayer] - 1)];
                    control.hand[otherPlayer][control.handCount[otherPlayer] - 1] = -1;
                    control.handCount[otherPlayer]--;
                }

                //draw 4
                for (int j = 0; j < 4; j++)
                {
                    //assume there are enough cards in player's deck
                    control.hand[otherPlayer][control.handCount[otherPlayer]] = control.deck[otherPlayer][control.deckCount[otherPlayer] - 1];//Add card to end of hand
                    control.deckCount[otherPlayer]--;  //Decrement deck count
                    control.handCount[otherPlayer]++;  //Increment hand count
                }
            }
        }
    }



    printf("Check for coin increase\n");
    noAbortAssert(&control.coins, &state->coins, sizeof(int));


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
	int choice1, choice2, handPos, x;
    int numPlayers = 3;
    int p = 0;
    int seed = 1000;
	struct gameState G, testState;

    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, minion, great_hall};

    //initialize the game state
    initializeGame(numPlayers, k, seed, &G);


    printf("\n_-_-_-_-_-_-_-_-_-_-_-_-_- playMinion Test Suite -_-_-_-_-_-_-_-_-_-_-_-_-_\n\n");

    printf("\n\n*****    TEST 1: choice1 == 1    *****\n");

    choice1 = 1;
    choice2 = 0;

    memcpy(&testState, &G, sizeof(struct gameState));



    testState.hand[0][0] = minion;
    testState.hand[0][1] = estate;
    testState.hand[0][2] = copper;
    testState.hand[0][3] = copper;
    testState.hand[0][4] = copper;

    handPos = 0;

    for(x = 1; x < numPlayers; x++){
        testState.hand[x][0] = estate;
        testState.hand[x][1] = estate;
        testState.hand[x][2] = copper;
        testState.hand[x][3] = copper;
        testState.hand[x][4] = copper;
        testState.handCount[x] = 5;
    }



    checkMinion(choice1, choice2, &testState, p, handPos);





    printf("\n\n*****    TEST 2: choice1 == 0, choice2 == 1    *****\n");
    //going to need to check the following:
    //    1) current player handCount
    //    2) current player deckCount
    //    3) other player's handCount
    //    4) other player's deckCount
    //    5) discardCount 

    choice1 = 0;
    choice2 = 1;

    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = minion;
    testState.hand[0][1] = estate;
    testState.hand[0][2] = copper;
    testState.hand[0][3] = copper;
    testState.hand[0][4] = copper;

    handPos = 0;

    for(x = 1; x < numPlayers; x++){
        testState.hand[x][0] = estate;
        testState.hand[x][1] = estate;
        testState.hand[x][2] = copper;
        testState.hand[x][3] = copper;
        testState.hand[x][4] = copper;
        testState.handCount[x] = 5;
    }

    checkMinion(choice1, choice2, &testState, p, handPos);





    printf("\n\n*****    TEST 3: choice1 == 0, choice2 == 0    *****\n");

    choice1 = 0;
    choice2 = 0;

    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = minion;
    testState.hand[0][1] = estate;
    testState.hand[0][2] = copper;
    testState.hand[0][3] = copper;
    testState.hand[0][4] = copper;

    handPos = 0;

    for(x = 1; x < numPlayers; x++){
        testState.hand[x][0] = estate;
        testState.hand[x][1] = estate;
        testState.hand[x][2] = copper;
        testState.hand[x][3] = copper;
        testState.hand[x][4] = copper;
        testState.handCount[x] = 5;
    }

    checkMinion(choice1, choice2, &testState, p, handPos);



	return 0;
}

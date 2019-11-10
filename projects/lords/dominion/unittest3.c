/*****************************************************************************
**    Unit test file for the ambassador card function in dominion.c
*****************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "noAbortAssert.h"
#include "rngs.h"


/*  

    Ambassador Card text: 
    Reveal a card from your hand. Return up to 2 copies of it from your hand to the Supply.
    Then each other player gains a copy of it.

    (int choice1, int choice2, struct gameState *state, int currentPlayer, int handPos)

    Pre-conditions necessary for Ambassador to play:

    First arg = choice1 denotes the position in the players hand of the card that they want to discard
    Second arg = choice2 denotes the quantity of cards to discard
    Third arg = the game state
    Fourth arg = currentPlayer
    Fifth arg = the hand position of the Ambassador card

*/

int checkAmbassador(int choice1, int choice2, struct gameState *state, int currentPlayer, int handPos){
	struct gameState control;
	memcpy (&control, state, sizeof(struct gameState));


	int r = playAmbassador(choice1, choice2, state, currentPlayer, handPos);
    int *ptrR = &r;

    int retVal = 0;
    int * ptrRetVal = &retVal;

    //start of control code

    int i;
    int j = 0;

    if (choice2 > 2 || choice2 < 0 || choice1 == handPos)   //choice2 must be between 0 and 2 and choice1 can't be Ambassador card
    {
        retVal = -1;
    }
    for (i = 0; i < control.handCount[currentPlayer]; i++)   //for each card in hand
    {
        if (i != handPos && control.hand[currentPlayer][i] == control.hand[currentPlayer][choice1])  //check it matches choice1 card, but is not current handPos
        {
            j++;
        }
    }
    if (j < choice2)  //Check that we actually have enough cards to remove
    {
        retVal = -1;
    }
    //increase supply count for choosen card by amount being discarded
    control.supplyCount[control.hand[currentPlayer][choice1]] += choice2;

    //each other player gains a copy of revealed card to their discard pile
    for (i = 0; i < control.numPlayers; i++)
    {
        if (i != currentPlayer) //check not current player
        {
            //check if supply pile is empty (0) or card is not used in game (-1)
            if ( control.supplyCount[choice1] > 0 )
            {
                control.discard[i][ control.discardCount[i] ] = choice1;
                control.discardCount[i]++;
                control.supplyCount[choice1]--;
            }
        }
    }

    //Discard played Ambassador card
    control.discard[currentPlayer][control.discardCount[currentPlayer]] = control.hand[currentPlayer][handPos];
    control.discardCount[currentPlayer]++;  //increment the count of cards in players discard pile
    control.hand[currentPlayer][handPos] = -1;
    control.hand[currentPlayer][handPos] = control.hand[currentPlayer][ (control.handCount[currentPlayer] - 1)];
    control.hand[currentPlayer][control.handCount[currentPlayer] - 1] = -1;
    control.handCount[currentPlayer]--;

    //trash copies of cards returned to supply to get them out of current player's hand/deck/discard
    for (j = 0; j < choice2; j++)
    {
        for (i = 0; i < control.handCount[currentPlayer]; i++)
        {
            if (control.hand[currentPlayer][i] == control.hand[currentPlayer][choice1])
            {
                //set played card to -1
                control.hand[currentPlayer][handPos] = -1;

                //remove card from player's hand
                if ( handPos == (control.handCount[currentPlayer] - 1) )     //last card in hand array is played
                {
                    //reduce number of cards in hand
                    control.handCount[currentPlayer]--;
                }
                else if ( control.handCount[currentPlayer] == 1 ) //only one card in hand
                {
                    //reduce number of cards in hand
                    control.handCount[currentPlayer]--;
                }
                else
                {
                    //replace discarded card with last card in hand
                    control.hand[currentPlayer][handPos] = control.hand[currentPlayer][ (control.handCount[currentPlayer] - 1)];
                    //set last card to -1
                    control.hand[currentPlayer][control.handCount[currentPlayer] - 1] = -1;
                    //reduce number of cards in hand
                    control.handCount[currentPlayer]--;
                }

                break;
            }
        }
    }


    printf("Check for return 0 or return -1\n");
    noAbortAssert(ptrR, ptrRetVal, sizeof(int));

    for(i = 0; i < control.handCount[currentPlayer]; i++){
        printf("Check current player's hand at position [%d]\n", i);
        noAbortAssert(&control.hand[currentPlayer][i], &state->hand[currentPlayer][i], sizeof(int));
    }

    for(i = 0; i < control.numPlayers; i++){
        printf("Check player #%d last card in discard pile\n", i);
        noAbortAssert(&control.discard[i][ control.discardCount[i] ], &state->discard[i][ control.discardCount[i] ], sizeof(int));
    }


	return 0;
}


int main(){
	int choice1, choice2, handPos;
    int numPlayers = 4;
    int p = 0;
    int seed = 1000;
	struct gameState G, testState;

    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, ambassador, great_hall};

    //initialize the game state
    initializeGame(numPlayers, k, seed, &G);


    printf("\n_-_-_-_-_-_-_-_-_-_-_-_-_- playAmbassador Test Suite -_-_-_-_-_-_-_-_-_-_-_-_-_\n\n");

    printf("\n\n*****    TEST 1: choice2 == 3    *****\n");
    //Check to make sure function exits with return code -1 since player is not allowed to return more than 2 cards to the supply

    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = ambassador;
    testState.hand[0][1] = estate;
    testState.hand[0][2] = estate;
    testState.hand[0][3] = copper;
    testState.hand[0][4] = copper;

    choice1 = 1;
    choice2 = 3;
    handPos = 0;

    checkAmbassador(choice1, choice2, &testState, p, handPos);




    printf("\n\n*****    TEST 2: choice1 == position of Ambassador    *****\n");
    //Player should not be allowed to return the played Ambassador to the supply

    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = ambassador;
    testState.hand[0][1] = estate;
    testState.hand[0][2] = estate;
    testState.hand[0][3] = copper;
    testState.hand[0][4] = copper;

    choice1 = 0;
    choice2 = 1;
    handPos = 0;

    checkAmbassador(choice1, choice2, &testState, p, handPos);





    printf("\n\n*****    TEST 3: choice2 is more than the card we have    *****\n");


    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = ambassador;
    testState.hand[0][1] = estate;
    testState.hand[0][2] = copper;
    testState.hand[0][3] = copper;
    testState.hand[0][4] = copper;

    choice1 = 1;
    choice2 = 2;
    handPos = 0;

    checkAmbassador(choice1, choice2, &testState, p, handPos);



    printf("\n\n*****    TEST 4: positive test case    *****\n");


    memcpy(&testState, &G, sizeof(struct gameState));

    testState.hand[0][0] = ambassador;
    testState.hand[0][1] = estate;
    testState.hand[0][2] = estate;
    testState.hand[0][3] = copper;
    testState.hand[0][4] = copper;

    choice1 = 1;
    choice2 = 2;
    handPos = 0;

    checkAmbassador(choice1, choice2, &testState, p, handPos);



	return 0;
}

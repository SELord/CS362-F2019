#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "noAbortAssert.h"
#include <time.h>
#include "rngs.h"
#include <math.h>




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

int comparator(const void* a, const void* b) {
    if (*(int*)a > *(int*)b)
        return 1;
    if (*(int*)a < *(int*)b)
        return -1;
    return 0;
}


int checkTribute(int handPos, struct gameState *state, int currentPlayer, int nextPlayer, int *tributeRevealedCards){
	struct gameState control;
	memcpy (&control, state, sizeof(struct gameState));

    int controlTribReveal[2];
    memcpy(&controlTribReveal, tributeRevealedCards, sizeof(int));
    //controlTribReveal[0] = *tributeRevealedCards[0];
    //controlTribReveal[1] = *tributeRevealedCards[1];


	tributeHandler(state, currentPlayer, nextPlayer);


    int i, j;
    int count;
    int deckCounter;
    int newDeck[MAX_DECK];
    int newDeckPos = 0;
    int card;
    //check if player has any cards in their deck and/or discard pile
    if((control.discardCount[nextPlayer] + control.deckCount[nextPlayer]) > 0)
    {
        for(i=0; i<2; i++)
        {

            if (control.deckCount[nextPlayer] <= 0) { //Deck is empty

                //Step 1 Shuffle the discard pile back into a deck
                //Move discard to deck
                for (i = 0; i < control.discardCount[nextPlayer]; i++) {
                    control.deck[nextPlayer][i] = control.discard[nextPlayer][i];
                    control.discard[nextPlayer][i] = -1;
                }

                control.deckCount[nextPlayer] = control.discardCount[nextPlayer];
                control.discardCount[nextPlayer] = 0;//Reset discard

                //Shufffle the deck
                if (control.deckCount[nextPlayer] > 0)
                {
                    qsort ((void*)(control.deck[nextPlayer]), control.deckCount[nextPlayer], sizeof(int), comparator);
                    /* SORT CARDS IN DECK TO ENSURE DETERMINISM! */

                    while (control.deckCount[nextPlayer] > 0) {
                        card = floor(Random() * control.deckCount[nextPlayer]);
                        newDeck[newDeckPos] = control.deck[nextPlayer][card];
                        newDeckPos++;
                        for (i = card; i < control.deckCount[nextPlayer]-1; i++) {
                            control.deck[nextPlayer][i] = control.deck[nextPlayer][i+1];
                        }
                        control.deckCount[nextPlayer]--;
                    }
                    for (i = 0; i < newDeckPos; i++) {
                        control.deck[nextPlayer][i] = newDeck[i];
                        control.deckCount[nextPlayer]++;
                    }

                    //Step 2 Draw Card
                    count = control.handCount[nextPlayer];//Get player's hand count


                    deckCounter = control.deckCount[nextPlayer];//Create a holder for the deck count

                    control.hand[nextPlayer][count] = control.deck[nextPlayer][deckCounter - 1];//Add card to end of hand
                    control.deckCount[nextPlayer]--;  //Decrement deck count
                    control.handCount[nextPlayer]++;  //Increment hand count
                }
            }
            else {
                count = control.handCount[nextPlayer];//Get current hand count for nextPlayer
                deckCounter = control.deckCount[nextPlayer];//Create holder for the deck count
                control.hand[nextPlayer][count] = control.deck[nextPlayer][deckCounter - 1];//Add card to the hand
                control.deckCount[nextPlayer]--;
                control.handCount[nextPlayer]++;//Increment hand count
            }
            


            controlTribReveal[i] = control.hand[nextPlayer][control.handCount[nextPlayer]-1];  //assign tribute element with card that was temporarily placed in hand

            control.discard[nextPlayer][control.discardCount[nextPlayer]] = control.hand[nextPlayer][control.handCount[nextPlayer]-1];
            control.discardCount[nextPlayer]++;  //increment the count of cards in players discard pile
            control.hand[nextPlayer][control.handCount[nextPlayer] - 1] = -1;
            control.handCount[nextPlayer]--;
        }

        if (controlTribReveal[0] == controlTribReveal[1]) { //Check for duplicate card and only gain rewards once
            if (controlTribReveal[0] == copper || controlTribReveal[0] == silver || controlTribReveal[0] == gold) 
            { //Treasure cards
                control.coins += 2;
            }
            else if (controlTribReveal[0] == estate || controlTribReveal[0] == duchy || controlTribReveal[0] == province || controlTribReveal[0] == gardens || controlTribReveal[0] == great_hall) 
            { //Victory Card Found

                //draw 2 cards
                for (i = 0; i < 2; i++)
                {


                    if (control.deckCount[currentPlayer] <= 0) { //Deck is empty

                        //Step 1 Shuffle the discard pile back into a deck
                        //Move discard to deck
                        for (i = 0; i < control.discardCount[currentPlayer]; i++) {
                            control.deck[currentPlayer][i] = control.discard[currentPlayer][i];
                            control.discard[currentPlayer][i] = -1;
                        }

                        control.deckCount[currentPlayer] = control.discardCount[currentPlayer];
                        control.discardCount[currentPlayer] = 0;//Reset discard

                        //Shufffle the deck
                        if (control.deckCount[currentPlayer] > 0)
                        {
                            qsort ((void*)(control.deck[currentPlayer]), control.deckCount[currentPlayer], sizeof(int), comparator);
                            /* SORT CARDS IN DECK TO ENSURE DETERMINISM! */

                            while (control.deckCount[currentPlayer] > 0) {
                                card = floor(Random() * control.deckCount[currentPlayer]);
                                newDeck[newDeckPos] = control.deck[currentPlayer][card];
                                newDeckPos++;
                                for (i = card; i < control.deckCount[currentPlayer]-1; i++) {
                                    control.deck[currentPlayer][i] = control.deck[currentPlayer][i+1];
                                }
                                control.deckCount[currentPlayer]--;
                            }
                            for (i = 0; i < newDeckPos; i++) {
                                control.deck[currentPlayer][i] = newDeck[i];
                                control.deckCount[currentPlayer]++;
                            }

                            //Step 2 Draw Card
                            count = control.handCount[currentPlayer];//Get player's hand count


                            deckCounter = control.deckCount[currentPlayer];//Create a holder for the deck count

                            control.hand[currentPlayer][count] = control.deck[currentPlayer][deckCounter - 1];//Add card to end of hand
                            control.deckCount[currentPlayer]--;  //Decrement deck count
                            control.handCount[currentPlayer]++;  //Increment hand count
                        }
                    }
                    else {
                        count = control.handCount[currentPlayer];//Get current hand count for currentPlayer
                        deckCounter = control.deckCount[currentPlayer];//Create holder for the deck count
                        control.hand[currentPlayer][count] = control.deck[currentPlayer][deckCounter - 1];//Add card to the hand
                        control.deckCount[currentPlayer]--;
                        control.handCount[currentPlayer]++;//Increment hand count
                    }
                }
            }
            else {
                //Action Card
                control.numActions += 2;
            }
        }
        else if(controlTribReveal[0] != controlTribReveal[1]) //tribute cards are not duplicate so gain rewards twice
        {
            for (j = 0; j < 2; j ++) {
                if (controlTribReveal[j] == copper || controlTribReveal[j] == silver || controlTribReveal[j] == gold) 
                { //Treasure cards
                    control.coins += 2;
                }
                else if (controlTribReveal[j] == estate || controlTribReveal[j] == duchy || controlTribReveal[j] == province || controlTribReveal[j] == gardens || controlTribReveal[j] == great_hall) 
                { //Victory Card Found
                    //draw 2 cards
                    for (i = 0; i < 2; i++)
                    {


                        if (control.deckCount[currentPlayer] <= 0) { //Deck is empty

                            //Step 1 Shuffle the discard pile back into a deck
                            //Move discard to deck
                            for (i = 0; i < control.discardCount[currentPlayer]; i++) {
                                control.deck[currentPlayer][i] = control.discard[currentPlayer][i];
                                control.discard[currentPlayer][i] = -1;
                            }

                            control.deckCount[currentPlayer] = control.discardCount[currentPlayer];
                            control.discardCount[currentPlayer] = 0;//Reset discard

                            //Shufffle the deck
                            if (control.deckCount[currentPlayer] > 0)
                            {
                                qsort ((void*)(control.deck[currentPlayer]), control.deckCount[currentPlayer], sizeof(int), comparator);
                                /* SORT CARDS IN DECK TO ENSURE DETERMINISM! */

                                while (control.deckCount[currentPlayer] > 0) {
                                    card = floor(Random() * control.deckCount[currentPlayer]);
                                    newDeck[newDeckPos] = control.deck[currentPlayer][card];
                                    newDeckPos++;
                                    for (i = card; i < control.deckCount[currentPlayer]-1; i++) {
                                        control.deck[currentPlayer][i] = control.deck[currentPlayer][i+1];
                                    }
                                    control.deckCount[currentPlayer]--;
                                }
                                for (i = 0; i < newDeckPos; i++) {
                                    control.deck[currentPlayer][i] = newDeck[i];
                                    control.deckCount[currentPlayer]++;
                                }

                                //Step 2 Draw Card
                                count = control.handCount[currentPlayer];//Get player's hand count


                                deckCounter = control.deckCount[currentPlayer];//Create a holder for the deck count

                                control.hand[currentPlayer][count] = control.deck[currentPlayer][deckCounter - 1];//Add card to end of hand
                                control.deckCount[currentPlayer]--;  //Decrement deck count
                                control.handCount[currentPlayer]++;  //Increment hand count
                            }
                        }

                        else {
                            count = control.handCount[currentPlayer];//Get current hand count for currentPlayer
                            deckCounter = control.deckCount[currentPlayer];//Create holder for the deck count
                            control.hand[currentPlayer][count] = control.deck[currentPlayer][deckCounter - 1];//Add card to the hand
                            control.deckCount[currentPlayer]--;
                            control.handCount[currentPlayer]++;//Increment hand count
                        }
                    }
                }
                else { 
                    //Action Card
                    control.numActions += 2;
                }
            }
        }
    }
    



    //discard tribute card
    control.discard[currentPlayer][control.discardCount[currentPlayer]] = control.hand[currentPlayer][handPos];
    control.discardCount[currentPlayer]++;  //increment the count of cards in players discard pile
    control.hand[currentPlayer][handPos] = -1;
    control.hand[currentPlayer][handPos] = control.hand[currentPlayer][ (control.handCount[currentPlayer] - 1)];
    control.hand[currentPlayer][control.handCount[currentPlayer] - 1] = -1;
    control.handCount[currentPlayer]--;

    
    











    printf("Check for coin increase\n");
    noAbortAssert(&control.coins, &state->coins, sizeof(int));

    printf("Check discarded card\n");
    noAbortAssert(&control.discard[currentPlayer][ control.discardCount[currentPlayer] ], &state->discard[currentPlayer][state->discardCount[currentPlayer]], sizeof(int));


    printf("Check handCount\n");
    noAbortAssert(&control.handCount[currentPlayer], &state->handCount[currentPlayer], sizeof(int));

    printf("Check numActions\n");
    noAbortAssert(&control.numActions, &state->numActions, sizeof(int));

    printf("Check that playedCards has not been incrememented\n");
    noAbortAssert(&control.playedCardCount, &state->playedCardCount, sizeof(int));

	return 0;
}






int main(){
    int i, j, n, handPos, currentPlayer, nextPlayer, numPlayers, randomHandCount, randomDeckCount, randomDiscardCount, randomHandPos;
    int tributeRevealedCards[2] = {-1, -1};
    int masterCardList[27] = {curse, estate, duchy, province, copper, silver, gold, adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall, minion, steward, tribute, ambassador, cutpurse, embargo, outpost, salvager, sea_hag, treasure_map};


    struct gameState G;

    printf("\n_-_-_-_-_-_-_-_-_-_-_-_-_- playTribute Random Testing -_-_-_-_-_-_-_-_-_-_-_-_-_\n\n");

    srand(time(NULL));

    for (n = 0; n < 10000; n++) {
        numPlayers = (rand() % (MAX_PLAYERS-1))+2;     //Random number of players from 2 to 4
        currentPlayer = (rand() % (numPlayers - 1));    //random number from 0 to 3, representing 1 of 4 players
        if(currentPlayer == 3){   //If current player is at the max allowed, loop to begining for next player
            nextPlayer = 0;
        }
        else{
            nextPlayer = currentPlayer + 1;
        }


        randomDeckCount = (rand() % (MAX_DECK+1));
        randomHandCount = (rand() % (MAX_HAND+1))+1;
        randomDiscardCount = (rand() % ((MAX_DECK/5)+1));
        randomHandPos = (rand() % randomHandCount);


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
        //G.numBuys = rand() % 500;
      
        //set coins
        G.coins = rand() % 4;

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
        G.hand[currentPlayer][randomHandPos] = tribute;
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


        //set playedCards and playedCardCount
        G.playedCardCount = 0;
        for (i = 0; i < randomDiscardCount; i++)
        {
            G.playedCards[i] = masterCardList[(rand() % 27)];
            G.playedCardCount++;
        }


        checkTribute(handPos, &G, currentPlayer, nextPlayer, tributeRevealedCards);
    }

    printf("\n_-_-_-_-_-_-_-_-_-_-_-_-_- End Of playTribute Testing -_-_-_-_-_-_-_-_-_-_-_-_-_\n\n");

    return 0;
}

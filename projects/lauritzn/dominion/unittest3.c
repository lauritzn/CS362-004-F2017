/*
 * File: unittest3.c
 * Author: Nina Lauritzen
 * Date: 10/18/2017
 * Description: unit tests for the gainCard() function
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define NUM_CARDS 27 

/***********************************************************************************
 * Function: asserttrue
 * Description: displays a brief success message if statement evaluates to true
 *              and a failure message if statement evaluates to false 
 **********************************************************************************/
void asserttrue(int statement)
{
	if(statement)
	{
		printf("Test passed\n");
	}
	else
	{
		printf("Test failed!\n");
	}
	return;
}; 

/*************************************************************************************
 * Function: countCardNum
 * Description: takes a card number and a card pile/hand array, counts and returns
 *              the number of times that card (number) appears 
 *************************************************************************************/

int countCardNum(int numToCount, int array[], int arraySize)
{
	int i;
	int count=0;
	
	for (i = 0; i<arraySize; i++)
	{
		if (array[i]== numToCount)
		{
			count++;
		}
	}

	return count;
}

/**************************************************************************************
 * Function: addedToPile
 * Description: takes a "before" array and an "after" array and returns the number of
 *	        a card added to the "after" array.  If a card was not correctly added,
 *              returns -1.  Correctness means one and only one card added.
 **************************************************************************************/
  
int addedToPile (int pileBefore[], int pileCountBefore, int pileAfter[], int pileCountAfter)
{
	// Create arrays for counting the number of each type of card before and after and initialize to zero

	int countPileBefore[NUM_CARDS]; // array stores count of cards of that type in hand
	
	int i;
	for (i = 0; i<NUM_CARDS; i++)
	{
  		 countPileBefore[i] = 0;
	} 

	int countPileAfter[NUM_CARDS];
	for (i = 0; i<NUM_CARDS; i++)
	{
		countPileAfter[i] = 0;
	}

	// Variable to flag hand count as correct (or incorrect)
			
	int cardAdded = -1; // holds number of card that was added to hand from stateBefore to stateAfter			
	int found = 0; // tracks number of added cards found

	// Update the count in the array for any cards in the player's hand before and after and compare
	for (i = 0; i<NUM_CARDS; i++)
	{
		//call function to count that card in the hand and store that count in the array
		countPileAfter[i] = countCardNum(i, pileAfter, pileCountAfter);
		countPileBefore[i] = countCardNum(i, pileBefore, pileCountBefore);

		if (countPileBefore[i] == countPileAfter[i]-1)
		{
			cardAdded = i;
			found++;
		}
		else if (countPileBefore[i] != countPileAfter[i])
		{
			return -1; // meaning some card was lost from the hand, so there is a bug 
		}	
	}

	if (found !=1) // if no added card has been found, or too many have been found
	{
		return -1;
	}
	
	return cardAdded;
}


/*************************************************************************************
 * Function: checkPileUnchanged
 * Description: takes a pile before and a pile after along with their counts, returns
 *				true if and only if the two piles are identical	
 *************************************************************************************/ 
  
int checkPileUnchanged(int pile1[], int pile1Count,int pile2[], int pile2Count)
{
	int unchanged = 1; 	
	int i;
      
	//make sure count is the same for both
	if (pile1Count != pile2Count)
	{
		unchanged = 0;
	}

	//make sure the pile is unchanged
	
	int cardCount1;
	int cardCount2;
	
	for (i = 0; i<NUM_CARDS;i++)
	{
		cardCount1 = countCardNum(i, pile1, pile1Count);
		cardCount2 = countCardNum(i, pile2, pile2Count);
		if (cardCount1 != cardCount2)
		{
			unchanged = 0;
		}
	}

	return unchanged;
}

/************************************************************************************
 * Function: checkAllUnchanged
 * Description: takes two gameState pointers and a player number and returns true
 *              if and only if that player's hand, discard, and deck piles are 
 *              identical in both gameStates 
 ***********************************************************************************/

int checkAllUnchanged(struct gameState* state1, struct gameState* state2, int player)
{
	int unchanged; // 1 indicates all unchanged, 0 means changed

	int discardUnchanged;
	int handUnchanged;
	int deckUnchanged;

	discardUnchanged = checkPileUnchanged(state1->discard[player], state1->discardCount[player], state2->discard[player], state2->discardCount[player]);
	handUnchanged = checkPileUnchanged(state1->hand[player], state1->handCount[player], state2->hand[player], state2->handCount[player]);
	deckUnchanged = checkPileUnchanged(state1->deck[player], state1->deckCount[player], state2->deck[player], state2->deckCount[player]);

	unchanged = (discardUnchanged & handUnchanged & deckUnchanged); // all must be true for unchanged to be true
	return unchanged;
}

void checkGainCard(int playerNum, int supplyPos)
{
	/*
     * Case 1: Card is gained by deck
     */

	// Set up the game
	
	int randomSeed = 222109;
	int players = MAX_PLAYERS;
	
	struct gameState state1;
	int initializeResult;
	int supply1[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

	initializeResult = initializeGame(players, supply1, randomSeed, &state1);

	// Make copy of original state
	struct gameState origState1;
	origState1 = state1;
	
	// Variables
	
	int returnedByFunction;
	int addedToDeck;
	int discardUnchanged;
	int handUnchanged;
 
	// Call gainCard for deck (flag = 1)
	
	returnedByFunction = gainCard(supplyPos, &state1, 1, playerNum);	
	
	// Need to check that supply count is less!	
	
	printf("gainCard: deck flagged, test whether the supply count has been decremented: ");

	int supplyCountBefore = origState1.supplyCount[supplyPos];
	int supplyCountAfter = state1.supplyCount[supplyPos];

	asserttrue(supplyCountAfter == supplyCountBefore-1);

	printf("gainCard: deck flagged, test for card added to deck: ");

	addedToDeck = addedToPile(origState1.deck[playerNum], origState1.deckCount[playerNum], state1.deck[playerNum], state1.deckCount[playerNum]);
	asserttrue(addedToDeck != -1);  // -1 indicates not correctly added to deck

	// check that the discard pile is unchanged
	
	printf("gainCard: deck flagged, test for discard pile unchanged: ");
	discardUnchanged = checkPileUnchanged(origState1.discard[playerNum], origState1.discardCount[playerNum], state1.discard[playerNum], state1.discardCount[playerNum]);
	asserttrue(discardUnchanged);	

	// check that the hand is unchanged

	printf("gainCard: deck flagged, testing for hand unchanged: ");
	handUnchanged = checkPileUnchanged(origState1.hand[playerNum], origState1.handCount[playerNum], state1.hand[playerNum], state1.handCount[playerNum]);	
	asserttrue(handUnchanged);

	/*
 	 * Case #2: Card is gained by hand
 	 */
	 
	// Set up the game
	
	randomSeed = 222109;
	players = MAX_PLAYERS;
	
	struct gameState state2;
	int supply2[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

	initializeResult = initializeGame(players, supply2, randomSeed, &state2);

	// Make copy of original state
	struct gameState origState2;
	origState2 = state2;

	// Call gainCard for hand (flag = 2)

	returnedByFunction = gainCard(supplyPos, &state2, 2, playerNum);	
	
	// Check that supply count is less	
	
	printf("gainCard: hand flagged, test whether supply count has been decremented: ");

	supplyCountBefore = origState2.supplyCount[supplyPos];
	supplyCountAfter = state2.supplyCount[supplyPos];

	asserttrue(supplyCountAfter == supplyCountBefore-1);

	printf("gainCard: hand flagged, test whether card was added to the hand: ");

	int addedToHand;
	addedToHand = addedToPile(origState2.hand[playerNum], origState2.handCount[playerNum], state2.hand[playerNum], state2.handCount[playerNum]);
	asserttrue(addedToHand != -1);  // -1 indicates not correctly added to hand

	// check that the discard pile is unchanged
	
	printf("gainCard: hand flagged, test for discard pile unchanged: ");
	discardUnchanged = checkPileUnchanged(origState2.discard[playerNum], origState2.discardCount[playerNum], state2.discard[playerNum], state2.discardCount[playerNum]);
	asserttrue(discardUnchanged);	

	// check that the deck is unchanged

	printf("gainCard: hand flagged, test for deck unchanged: ");
	int deckUnchanged;
	deckUnchanged = checkPileUnchanged(origState2.deck[playerNum], origState2.deckCount[playerNum], state2.deck[playerNum], state2.deckCount[playerNum]);	
	asserttrue(deckUnchanged);

	/*
 	 * Case #3: Card is gained by discard pile
 	 */  
	 
	// Set up the game
	
	randomSeed = 222109;
	players = MAX_PLAYERS;
	
	struct gameState state3;
	int supply3[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

	// Minimum information to initialize game is number of players, list of supply cards, random seed, and the gameState struct
	initializeResult = initializeGame(players, supply3, randomSeed, &state3);

	// Make copy of original state
	struct gameState origState3;
	origState3 = state3;

	// Call gainCard for discard (flag = 0)

	returnedByFunction = gainCard(supplyPos, &state3, 0, playerNum);	
	
	// Need to check that supply count is less!	
	
	printf("gainCard: discard flagged, testing whether the supply count decremented: ");

	supplyCountBefore = origState3.supplyCount[supplyPos];
	supplyCountAfter = state3.supplyCount[supplyPos];

	asserttrue(supplyCountAfter == supplyCountBefore-1);

	//int addedToPile (int pileBefore[], int pileCountBefore, int pileAfter[], int pileCountAfter)
	
	printf("gainCard: discard flagged, test for card added to the discard: ");

	int addedToDiscard;
	addedToDiscard = addedToPile(origState3.discard[playerNum], origState3.discardCount[playerNum], state3.discard[playerNum], state3.discardCount[playerNum]);
	asserttrue(addedToDiscard != -1);  // -1 indicates not correctly added to deck

	//checkPileUnchanged(int pile1[], int pile1Count,int pile2[], int pile2Count);
	
	// check that the hand is unchanged
	
	printf("gainCard: discard flagged, test for hand unchanged: ");
	handUnchanged = checkPileUnchanged(origState3.hand[playerNum], origState3.handCount[playerNum], state3.hand[playerNum], state3.handCount[playerNum]);
	asserttrue(handUnchanged);	

	// check that the deck is unchanged

	printf("gainCard: discard flagged, test for deck unchanged: ");
	deckUnchanged = checkPileUnchanged(origState2.deck[playerNum], origState2.deckCount[playerNum], state2.deck[playerNum], state2.deckCount[playerNum]);	
	asserttrue(deckUnchanged);

	/*
 	 * Case #4: Supply pile is empty 
	 */
	
	randomSeed = 222109;
	players = MAX_PLAYERS;
	
	struct gameState state4;
	int supply4[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

	// Minimum information to initialize game is number of players, list of supply cards, random seed, and the gameState struct
	initializeResult = initializeGame(players, supply4, randomSeed, &state4);
	
	// Empty the relevant supply pile	

	state4.supplyCount[supplyPos] = 0;

	// Make copy of original state
	struct gameState origState4;
	origState4 = state4;

	// Variable to hold value returned by gainCard and check that other piles are unchanged 
	
	int pilesUnchanged;
	int flag;

	for (flag=0; flag < 4; flag++) // loops once for each flag, discard, hand, and deck 
	{	
		printf("gainCard: supply empty, test for -1 return value: ");
		returnedByFunction = gainCard(supplyPos, &state4, flag, playerNum);	
		asserttrue(returnedByFunction == -1); 

		printf("gainCard: supply empty, test for all piles unchanged: ");
		pilesUnchanged = checkAllUnchanged(&origState4, &state4, playerNum);
		asserttrue(pilesUnchanged);
	}

	/*
 	 * Case #5: Gained card is not in the game (not in play)
 	 */   

	 // Set up the game
	
	randomSeed = 222109;
	players = MAX_PLAYERS;
	
	struct gameState state5;
	int supply5[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

	initializeResult = initializeGame(players, supply5, randomSeed, &state5);
	
	// Remove the gained card type from play (if in play)
	state5.supplyCount[supplyPos] = -1;
	
	// Make copy of original state
	struct gameState origState5;
	origState5 = state5;

	for (flag=0; flag<4; flag++) // loops once for each flag (discard, hand, deck)
	{
		printf("gainCard: supply card not in play, test for -1 return value: ");
		returnedByFunction = gainCard(supplyPos, &state5, flag, playerNum);	
		asserttrue(returnedByFunction == -1); 
		printf("gainCard: supply card not in play, test for all piles unchanged: ");
		pilesUnchanged = checkAllUnchanged(&origState5, &state5, playerNum);
		asserttrue(pilesUnchanged);
	}
 	
	return;

}  


int main()
{
	// Check all cases for first player: Player 0
	checkGainCard(0, 0);

	return 0;
}
 

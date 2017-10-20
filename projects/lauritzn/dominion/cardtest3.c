/******************************************************************************************
 * File: cardtest3.c
 * Author: Nina Lauritzen
 * Date: 11/18/2017
 * Description: test of the village card 
 ******************************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>

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

/***********************************************************************************
 *  Function: removedFromHand
 *  Description: Checks whether a given card has been correctly removed from the
 *               player's hand. Returns 1 if correct, 0 if removal is not correct  
 ***********************************************************************************/ 

int removedFromHand (struct gameState *stateBefore, struct gameState *stateAfter, int player, int cardRemoved)
{
	// Create arrays for counting the number of each type of card before and after and initialize to zero

	int countHandBefore[NUM_CARDS]; // array stores count of cards of that type in hand
	
	int i;
	for (i = 0; i<NUM_CARDS; i++)
	{
  		 countHandBefore[i] = 0;
	} 

	int countHandAfter[NUM_CARDS];
	for (i = 0; i<NUM_CARDS; i++)
	{
		countHandAfter[i] = 0;
	}

	// Variable to flag hand count as correct (or incorrect)
			
	int handCorrect = 1; // default is true (correct)
			

	// Update the count in the array for any cards in the player's hand before and after and compare
	for (i = 0; i<NUM_CARDS; i++)
	{
		//call function to count that card in the hand and store that count in the array
		countHandAfter[i] = countCardNum(i, stateAfter->hand[player], stateAfter->handCount[player]);
		countHandBefore[i] = countCardNum(i, stateBefore->hand[player], stateBefore->handCount[player]);

		// If the card being counted is the discarded card, the count in the new hand should be one less
		if (i == cardRemoved)
		{
			if (countHandBefore[i] != (countHandAfter[i]+1))
			{					
				handCorrect = 0; //meaning hand incorrect
				printf("count of removed card is not less!\n");
			}
		}

		// Otherwise the counts should be equal, since no other cards were added or discarded 
	
		else
		{
			if (countHandBefore[i] != countHandAfter[i])
			{
				handCorrect = 0; // meaning hand incorrect
				printf("count of non-removed cards has changed!\n");	
			}
		}
		
	}  
	
	return handCorrect;
}

/*********************************************************************************************************
 * Function: checkAddToDiscard
 * Description: Takes two gameState parameters, a player, and a card and checks to make sure that 
 *              discarded card ends up in the discard pile 
 *******************************************************************************************************/ 

int checkAddToDiscard(struct gameState *stateBefore, struct gameState *stateAfter, int player, int cardAdded)
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
			
	int discardAdded = 1; // default is true (correct)
			
	// Update the count in the array for any cards in the player's hand before and after and compare
	for (i = 0; i<NUM_CARDS; i++)
	{
		//call function to count that card in the hand and store that count in the array
		countPileAfter[i] = countCardNum(i, stateAfter->discard[player], stateAfter->discardCount[player]);
		countPileBefore[i] = countCardNum(i, stateBefore->discard[player], stateBefore->discardCount[player]);

		// If the card being counted is the discarded card, the count in the new hand should be one less
		if (i == cardAdded)
		{
			if (countPileBefore[i] >= countPileAfter[i])
			{					
				discardAdded = 0; //meaning no added card of the type discarded  
			}
		}

		// Otherwise the counts should be equal, since no other cards were added or discarded 
		else
		{
			if (countPileBefore[i] != countPileAfter[i])
			{
				discardAdded = 0; // meaning discard incorrect
			}
		}
		
	}  
	
	return discardAdded;
}

/**********************************************************************************************
 *  Function: removedFromDeck
 *  Description:  Returns -1 if a card has not been correctly removed from the deck, otherwise 
 *  		  returns the number of the card removed. 
 *  		  Correct means 1 and only 1 card removed.
 *********************************************************************************************/ 

int removedFromDeck(struct gameState *stateBefore, struct gameState *stateAfter, int player)
{
	// Create arrays for counting the number of each type of card before and after and initialize to zero

	int countDeckBefore[NUM_CARDS]; // array stores count of cards of that type in hand
	
	int i;
	for (i = 0; i<NUM_CARDS; i++)
	{
  		 countDeckBefore[i] = 0;
	} 

	int countDeckAfter[NUM_CARDS];
	for (i = 0; i<NUM_CARDS; i++)
	{
		countDeckAfter[i] = 0;
	}

	// Variable to hold card removed (or flag as incorrect)
				
	int cardRemoved = -1; // holds number of card that was removed from deck from stateBefore to stateAfter			
	int found = 0; // tracks number of removed cards found, should be 1 if 1 card correctly removed

	// Update the count in the array for any cards in the player's hand before and after and compare
	for (i = 0; i<NUM_CARDS; i++)
	{
		//call function to count that card in the hand and store that count in the array
		countDeckAfter[i] = countCardNum(i, stateAfter->deck[player], stateAfter->deckCount[player]);
		countDeckBefore[i] = countCardNum(i, stateBefore->deck[player], stateBefore->deckCount[player]);
		
		if (countDeckBefore[i] == countDeckAfter[i]+1)
		{
			cardRemoved = i;
			found++;
		}
		else if (countDeckBefore[i] != countDeckAfter[i])
		{	
			return -1; // meaning some card was added rather than removed, bug found  
		}
	}
	
	if (found != 1) // if no removed card has been found, or too many have been found
	{
		return -1; // meaning hand invalid
	}				

	return cardRemoved;
}

/***************************************************************************************
 * Function: testVillageCard
 * Description: Runs tests for the village card function, for a particular position
 *              of the village card in the hand  
 **************************************************************************************/ 

void testVillageCard(int villagePos)
{
	//Set up the game state
	int randomSeed = 100;
	int players = MAX_PLAYERS;
	int player = 0;
	
	struct gameState state;
	int initializeResult;
	int supply[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

	// Minimum information to initialize game is number of players, list of supply cards, random seed, and the gameState struct
	initializeResult = initializeGame(players, supply, randomSeed, &state);

	// Make the card in villagePos a village card
	state.hand[player][villagePos] = village;	
 
	// Make copy of original state
	struct gameState origState;
	origState = state;	
	
	// Play the village card
	cardEffect(village, 0, 0, 0, &state, villagePos, 0); // value 0 for choices and bonus since N/A  


	// Test whether the deck loses exactly one card
	int cardRemoved = removedFromDeck(&origState, &state, player);
	printf("village card, position %i: test exactly one card removed from deck: ", villagePos);
	asserttrue(cardRemoved != -1);
	
	// Test that the count of the card removed from the deck is now one greater in the player's hand, or equal if village was drawn
	if (cardRemoved !=-1) // assuming a card was successfully removed
	{
		
		int countBefore = countCardNum(cardRemoved, origState.hand[player], origState.handCount[player]);
		int countAfter = countCardNum(cardRemoved, state.hand[player], state.handCount[player]);
		if(cardRemoved == village)
		{
			printf("village card, position %i: test card removed from deck was added to hand: ", villagePos);
			asserttrue(countBefore == countAfter);
		}	
		else
		{
			printf("village card, position %i: test card removed from deck was added to hand: ", villagePos);
			asserttrue(countAfter = countBefore+1);
		}	
		
	}	
			
	// Test whether the player gains two actions
	
	printf("village card, position %i: test action count increased by 2: ", villagePos);
	asserttrue(state.numActions == origState.numActions+2);
	
	// Test whether the village card is removed from the player's hand: whether village card count in hand is one less (if village card was not drawn)
	
	int villageCountBefore = countCardNum(village, origState.hand[player], origState.handCount[player]);
	int villageCountAfter = countCardNum(village, state.hand[player], state.handCount[player]);

	printf("village card, position %i: test count of village card in player hand correct: ", villagePos);
	if(cardRemoved != village)
	{
		asserttrue(villageCountAfter == villageCountBefore-1);
	}	
	else
	{
		asserttrue(villageCountAfter == villageCountBefore);
	}	
	
	// Test whether the village card is discarded
	
	printf("village card, position %i: test village card added to discard pile: ", villagePos);
	int villageDiscarded = checkAddToDiscard(&origState, &state, player, village);
	asserttrue(villageDiscarded);
	
	return;	
} 

int main()
{
	// Test case where card is in position 0
	testVillageCard(0);
	
	// Test case where card is in position 2
	testVillageCard(2);
	
	// Test case where card is in position 4
	testVillageCard(4);
	
	return 0;
}

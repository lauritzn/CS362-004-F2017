/**********************************************************************************
 * File: cardtest4.c
 * Author: Nina Lauritzen
 * Date: 10/19/2017
 * Description: unit tests for the embargo card function
 *********************************************************************************/ 

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

/*********************************************************************************
 * Function: embargoTest
 * Description: performs tests of end conditions in game state after embargo 
 *              card is played
 *********************************************************************************/
void embargoTest(struct gameState* before, struct gameState* after, int player, int supplyPos, int embargoHandPos)
 {
	
	// Check that two coins are added
	printf("embargo card: test for 2 coins added: ");
	asserttrue(after->coins = before->coins+2);
	
	// Check that an embargo token has been added
	printf("embargo card: test for embargo token added: ");
	asserttrue(after->embargoTokens[supplyPos] = before->embargoTokens[supplyPos]+1);	
	
	// Check that the embargo card has been removed from the player's hand
	int embargoBefore = countCardNum(embargo, before->hand[player], before->handCount[player]);
	int embargoAfter = countCardNum(embargo, after->hand[player], after->handCount[player]);
	
	printf("embargo card: test for embargo card removed from player's hand: "); 
	asserttrue(embargoAfter == embargoBefore-1);
	
	return;
	 
 } 
 
 /*********************************************************************************
   * Function: startTests
   * Description: sets up the game with embargo card in a particular position in 
   *              player's hand, calls cardEffect() for the embargo card,
   *              and calls the function to test the resulting game state for
   *              correctness   
   ********************************************************************************/
   
 void startTests(int embargoHandPos, int player)
 {
		// Set up the game
		
		int randomSeed = 1000;
		int players = 2;
	
		struct gameState state;
		int initializeResult;
		int supply[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

		initializeResult = initializeGame(players, supply, randomSeed, &state);
		
		// Place the embargo card in the appropriate hand position
		state.hand[player][embargoHandPos] = embargo;
		
		// Make a copy of the game state
		struct gameState origState;
		origState = state;	
		
		// Position for the embargo token
		int supplyPos = smithy;  // this supply card is in play
		
		// call the embargo card function
		cardEffect(embargo, supplyPos, 0, 0, &state, embargoHandPos, 0); // first choice is supply position
	
		printf("Tests for embargo card in hand position %i:\n", embargoHandPos);
		embargoTest(&origState, &state, player, supplyPos, embargoHandPos);
 	 
		return;
 }
 
 int main()
 {
	 startTests(0, 0); // embargo card in first position in hand, player 0
	 startTests(2, 0); // embargo card in middle position in hand, player 0
	 startTests(4, 0); // embargo card in end position in hand, player 0
	 
	 printf("Tests for playing embargo card when chosen supply not in play:\n");

		// Set up the game
		int randomSeed = 1000;
		int players = 2;
		int embargoHandPos = 2;  // position of the embargo card in the player's hand
		int player = 0; // player 0	

		struct gameState state;
		int initializeResult;
		int supply[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

		initializeResult = initializeGame(players, supply, randomSeed, &state);
		
		// Place the embargo card in the appropriate hand position
		state.hand[player][embargoHandPos] = embargo;
		
		// Make a copy of the game state
		struct gameState origState;
		origState = state;	
		
		// Position for the embargo token
		int supplyPos = minion; // minion is not in play!
		
		// call the embargo card function
		int returnValue = cardEffect(embargo, supplyPos, 0, 0, &state, embargoHandPos, 0); // first choice is supply position
	 
		printf("embargoCard: test for -1 return value when targeted card not in play: ");
		asserttrue(returnValue == -1);
		
		printf("embargoCard: test for no change in coins when targeted card not in play: ");
		asserttrue(origState.coins == state.coins);
		
	printf("Tests for playing embargo card on an empty pile:\n");

		// Set up the game
		randomSeed = 1000;
		players = 2;
		embargoHandPos = 2;
		
		struct gameState state2;
		int supply2[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

		initializeResult = initializeGame(players, supply2, randomSeed, &state2);
		
		// Place the embargo card in the appropriate hand position
		state2.hand[player][embargoHandPos] = embargo;
		
		state2.supplyCount[adventurer] = 0; //empty the supply pile for the adventurer card, none left 
		
		// Make a copy of the game state
		struct gameState origState2;
		origState2 = state2;	
		
		// Position for the embargo token
		supplyPos = adventurer; 
		
		// call the embargo card function
		returnValue = cardEffect(embargo, supplyPos, 0, 0, &state2, embargoHandPos, 0); // first choice is supply position
	 
		printf("embargoCard: test for -1 return value when targeted card pile empty: ");
		asserttrue(returnValue == -1);
		
		printf("embargoCard: test for no change in coins when targeted card pile empty: ");
		asserttrue(origState2.coins == state2.coins);
		
		return 0;
 }
  
  
  

/********************************************************************
 * File: cardtest2.c
 * Author: Nina Lauritzen
 * Date: 10/18/2017 
 * Description: unit tests for the adventurer card function
 ********************************************************************/

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
 * Function: adventurerTest
 * Description: plays the adventurer card and performs tests of end conditions 
 *              in game state after adventurer card is played
 *********************************************************************************/
void adventurerTest(struct gameState* before, struct gameState* after, int player, int adventurerPos)
 {
	 // call the adventurer card function
	  cardEffect(adventurer, 0, 0, 0, after, adventurerPos, 0); // value 0 for choices and bonus since N/A 
	
	 // check that two cards are added to the player's hand, minus adventurer, for 1 total
	 
	 int handCountBefore = before->handCount[player];
	 int handCountAfter = after->handCount[player];
	 printf("adventurer card: test for 1 card added to player's hand (2 treasure - adventurer): "); 
	 asserttrue(handCountAfter == handCountBefore + 1);	
	
	 // count treasure added to the player's hand
	 int i; 
	 int oldTreasure = countCardNum(gold, before->hand[player], before->handCount[player]);
	 oldTreasure += countCardNum(copper, before->hand[player], before->handCount[player]);
	 oldTreasure += countCardNum(silver, before->hand[player], before->handCount[player]);
	
	 int newTreasure = countCardNum(gold, after->hand[player], after->handCount[player]);
	 newTreasure += countCardNum(copper, after->hand[player], after->handCount[player]);
 	 newTreasure += countCardNum(silver, after->hand[player], after->handCount[player]);

	 printf("adventurer card: test for two treasure added to the player's hand: "); 	
   	 asserttrue(newTreasure - oldTreasure == 2); 
	 
	 // check that no non-treasure card is added to the player's hand
	 int onlyTreasureAdded = 1; // default is true
	 int countHandBefore[NUM_CARDS];
	 int countHandAfter[NUM_CARDS];
	 
	 for (i = 0; i<NUM_CARDS; i++)
	{
		//call function to count that card in the hand and store that count in the array
		countHandAfter[i] = countCardNum(i, after->hand[player], after->handCount[player]);
		countHandBefore[i] = countCardNum(i, before->hand[player], before->handCount[player]);
		
		if (countHandBefore[i] != countHandAfter[i])
		{
			if (i != gold && i != silver && i != copper)
			{
				onlyTreasureAdded = 0;
			}	
		}
	}
	printf("adventurer card: test for only treasure added to hand: ");
	asserttrue(onlyTreasureAdded);
	
	// Check that count of cards in discard + deck = original deck count - 1 (cards added to hand)
	
	printf("adventurer card: test for combined count of discard and deck correct: ");
	asserttrue(after->discardCount[player]+after->deckCount[player] == before->deckCount[player]-1);
	
	// Check that the adventurer card has been removed from the players hand
	// Count adventurer card before
	int adventurerBefore = countCardNum(adventurer, before->hand[player], before->handCount[player]);
	int adventurerAfter = countCardNum(adventurer, after->hand[player], after->handCount[player]);
	// Count adventurer card after
	printf("adventurer card: test for count of adventurer card in hand being one less: "); 
	asserttrue(adventurerAfter == adventurerBefore-1);
	
	return;
	 
 } 
 
 
/**************************************************************************************
 *  Function: adventurerTop
 *  Description: sets up a gameState with three identical coins at the top of the deck
 *               and calls the function to test the adventurer card function     
 **************************************************************************************/

 void adventurerTop(int coin)
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

	// Set up the player's deck with coins at the top

	state.deck[player][0] = council_room;
	state.deck[player][1] = embargo;
	state.deck[player][2] = gardens;
	state.deck[player][3] = sea_hag;
	state.deck[player][4] = coin;
	state.deck[player][5] = coin;
	state.deck[player][6] = coin;
	state.deckCount[player] = 7;
		
	// Place adventurer card in player's hand, position 0

	int adventurerPos = 0;	
	state.hand[player][adventurerPos] = adventurer;
		
	// Make copy of original state
	struct gameState origState;
	origState = state;	
	 
	// Test the result
	adventurerTest(&origState, &state, player, adventurerPos);
	
	return;
 }
 
void adventurerBottom(int coin)
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

	// Set up the player's deck with coins at the top

	state.deck[player][0] = coin;
	state.deck[player][1] = coin;
	state.deck[player][2] = coin;
	state.deck[player][3] = sea_hag;
	state.deck[player][4] = remodel;
	state.deck[player][5] = smithy;
	state.deck[player][6] = steward;
	state.deckCount[player] = 7;
		
	// Place adventurer card in player's hand, position 0

	int adventurerPos = 0;	
	state.hand[player][adventurerPos] = adventurer;
	
	
	// Make copy of original state
	struct gameState origState;
	origState = state;	
	 
	// Test the result
	adventurerTest(&origState, &state, player, adventurerPos);
	
	return;
}

int main()
{
	// Test case with mixed coins in middle
    printf("\nCASE #1 for adventurer: mixed coins in the middle of the deck\n");
	
	//Set up the game state
	int randomSeed = 100;
	int players = MAX_PLAYERS;
	int player = 0;
	
	struct gameState state;
	int initializeResult;
	int supply[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

	// Minimum information to initialize game is number of players, list of supply cards, random seed, and the gameState struct
	initializeResult = initializeGame(players, supply, randomSeed, &state);

	// Set up the player's deck with mixed coins in the middle
	
	state.deck[player][0] = smithy;
	state.deck[player][1] = sea_hag;
	state.deck[player][2] = copper;
	state.deck[player][3] = silver;
	state.deck[player][4] = gold;
	state.deck[player][5] = smithy;
	state.deck[player][6] = village;
	state.deckCount[player] = 7;
		
	// Place adventurer card in player's hand, position 0

	int adventurerPos = 0;
	state.hand[player][adventurerPos] = adventurer;	
	
	// Make copy of original state
	struct gameState origState;
	origState = state;	
	
	adventurerTest(&origState, &state, player, adventurerPos);
	
	// Test cases with all coins on top
	printf("\nCASE #2: gold coins on top of deck:\n");
	adventurerTop(gold);
	printf("\nCASE #3: copper coins on top of deck:\n");
	adventurerTop(copper);
	printf("\nCASE #4: silver coins on top of deck:\n");
	adventurerTop(silver);
	
	// Test cases with all coins at bottom
	printf("\nCASE #5: gold coins at bottom of deck:\n");
	adventurerBottom(gold);
	printf("\nCASE #6: copper coins at bottom of deck:\n");
	adventurerBottom(copper);
	printf("\nCASE #7: silver coins at bottom of deck:\n");
	adventurerBottom(silver);
	
	return 0;
}



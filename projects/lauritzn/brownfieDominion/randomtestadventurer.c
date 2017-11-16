/***************************************************************************
 * Filename: randomtestadventurer.c
 * Author: Nina Lauritzen
 * Date: 11/2/3017
 * Description: Random tests for the adventurer card function in 
 *		dominion.c 
 **************************************************************************/ 

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>

#define NUM_CARDS 27

// Struct to track coverage of edge cases
struct edge_counts {
	int deckMin;
	int discardMin;
	int handMin;
	int deckMax;
	int discardMax;
	int handMax;
	int noTreasure;
	int test1Fail;
	int test2Fail;
	int test3Fail;
	int test4Fail;
};

int asserttrue(int statement);
int countCardNum(int numToCount, int array[], int arraySize);
void randomHand(struct gameState* state, int player, int handSize, int supply[]);
void randomDeck(struct gameState* state, int player, int deckSize, int supply[]);
void randomDiscard(struct gameState* state, int player, int discardSize, int supply[]);
int randomAdventurerTest(int count, int seed, struct edge_counts* edges);

int main()
{
	int seed;
	seed = time(NULL); 
	srand(seed);
	
	// Print the seed so that tests can be reproduced
	printf("Random Adventurer Card Tests, Seed = %i\n\n", seed);

	// Set up the struct to track edge cases
	struct edge_counts edgeCases;
	edgeCases.deckMin = 0;
	edgeCases.discardMin = 0;  
	edgeCases.handMin = 0;   
	edgeCases.deckMax = 0;   
	edgeCases.discardMax = 0;   
	edgeCases.handMax =0;  
	edgeCases.noTreasure = 0;	
	edgeCases.test1Fail = 0;
	edgeCases.test2Fail = 0;
	edgeCases.test3Fail = 0;
	edgeCases.test4Fail = 0;
	
	int i;
	int count = 0;	
	
	for (i = 0; i<10000; i++)
	{
		count++;
		randomAdventurerTest(count, seed, &edgeCases);			
	}	

	// Print the counts of the edge cases hit:
	printf("\nTESTS COMPLETE\n");
	printf("Number of times deck was empty: %i\n", edgeCases.deckMin);
	printf("Number of times deck was at max size: %i\n", edgeCases.deckMax);
	printf("Number of times discard was empty: %i\n", edgeCases.discardMin);
	printf("Number of times discard was at max size: %i\n", edgeCases.discardMax);
	printf("Number of times hand was at minimum: %i\n", edgeCases.handMin);
	printf("Number of times hand was at max size: %i\n", edgeCases.handMax);
	printf("Number of times no treasure there to be drawn: %i\n", edgeCases.noTreasure);	
	printf("Number of times test 1 failed: %i\n", edgeCases.test1Fail);
	printf("Number of times test 2 failed: %i\n", edgeCases.test2Fail);
	printf("Number of times test 3 failed: %i\n", edgeCases.test3Fail);
	printf("Number of times test 4 failed: %i\n", edgeCases.test4Fail); 

	return 0;
}

/**********************************************************************************************
 * Function: randomAdventurerTest
 * Description: creates a gameState with a random player number, deck, discard, and hand.
 * Calls the function to play the smithy card.  Checks the game state afterwards to test
 * whether various requirements have been met 
 *********************************************************************************************/ 

int randomAdventurerTest(int count, int seed, struct edge_counts* edgeCases)
{
	// Set up a random game state
	
	struct gameState state;
	int supply[10];
	int cardNum;
	int i;
	int j;
	int duplicate = 0;
	int discardCopper = 0;
	int discardSilver = 0;
	int discardGold = 0;
	int deckCopper = 0;
	int deckSilver = 0;
	int deckGold = 0;

		
	for (i = 0; i<10; i++)
	{
		duplicate = 0;
		do
		{
			supply[i] = rand()%NUM_CARDS;  // number between 0 and NUM_CARDS=1
		}
		while (supply[i] < 6); // repeat if card chosen is treasure, curse, or victory card
		
		// Guarantee that supply piles are unique (no two with the same card)
		for (j=0; j<i; j++)
		{
				if (supply[j] == supply[i])
				{
					duplicate = 1;
				}		
		}
		if(duplicate)
		{
			i--; // decrement i so that next iteration will try again
		}		
		
	}		

	int initializeResult;
	int players = MAX_PLAYERS;	

	// Minimum information to initialize game is number of players, list of supply cards, random seed, and the gameState struct
	initializeResult = initializeGame(players, supply, seed, &state);

	// Get a random player
	int player;
	player = rand()%MAX_PLAYERS;
	
	// make it that player's turn
	state.whoseTurn = player;
	
	// Get a random size for the discard, deck, and hand	
	int discardSize, deckSize, handSize;
	
	deckSize = rand()%(MAX_DECK+1);
	
	// Track edge cases
	if (deckSize == MAX_DECK)
	{
		edgeCases->deckMax++;
	}
	else if (deckSize == 0)
	{
		edgeCases->deckMin++;
	}
	
	discardSize = rand()%(MAX_DECK+1);
	
	// Track edge cases
	if (discardSize == MAX_DECK)
	{
		edgeCases->discardMax++;
	}
	else if (discardSize == 0)
	{
		edgeCases->discardMin++;
	}


	handSize = rand()%MAX_HAND;
	handSize += 1; // hand size cannot be 0
	
	// Track edge cases
	if (handSize == MAX_DECK)
	{
		edgeCases->handMax++;
	}
	else if (handSize == 1) // minimum hand is one card, adventurer
	{
		edgeCases->handMin++;
	}
	
	// Create a random hand for the player
	randomHand(&state, player, handSize, supply);
		
	// Place the adventurer card in a random position in the player's hand
	int handPos;
	handPos = rand()%handSize;
	state.hand[player][handPos] = adventurer;
	
	// Create a random deck for the player	
	randomDeck(&state, player, deckSize, supply);

	// Create a random discard for the player
	randomDiscard(&state, player, discardSize, supply);
	
	// Make a copy of the game state
	struct gameState origState;
	origState = state;		
	
	// Count treasure cards in the discard
	int discardTreasure = 0;
	discardGold = countCardNum(gold, origState.discard[player], origState.discardCount[player]);
	discardCopper = countCardNum(copper, origState.discard[player], origState.discardCount[player]);
	discardSilver = countCardNum(silver, origState.discard[player], origState.discardCount[player]);
	discardTreasure += discardGold;
	discardTreasure += discardCopper;
	discardTreasure += discardSilver;
	
	// Count treasure cards in the deck
	int deckTreasure = 0;
	deckGold = countCardNum(gold, origState.deck[player], origState.deckCount[player]);
	deckCopper = countCardNum(copper, origState.deck[player], origState.deckCount[player]);
	deckSilver = countCardNum(silver, origState.deck[player], origState.deckCount[player]);
	deckTreasure += deckGold;
	deckTreasure += deckCopper;
	deckTreasure += deckSilver;
	
	// Track edge cases where there is no treasure to be drawn
	if (deckTreasure + discardTreasure == 0)
	{
		edgeCases->noTreasure++;
	}	
		
	// call the adventurer card function
	cardEffect(adventurer, 0, 0, 0, &state, handPos, 0); // value 0 for choices and bonus since N/A 
	
	// count treasure added to the player's hand
	int oldTreasure = countCardNum(gold, origState.hand[player], origState.handCount[player]);
	oldTreasure += countCardNum(copper, origState.hand[player], origState.handCount[player]);
	oldTreasure += countCardNum(silver, origState.hand[player], origState.handCount[player]);
	
	int newTreasure = countCardNum(gold, state.hand[player], state.handCount[player]);
	newTreasure += countCardNum(copper, state.hand[player], state.handCount[player]);
	newTreasure += countCardNum(silver, state.hand[player], state.handCount[player]);
	
	// Get the count of the player's hand before and after	
	int handCountBefore = origState.handCount[player];
	int handCountAfter = state.handCount[player];
	
	// Variable to hold test result
	int testPassed;
	
	// If there is more than one treasure card available to be drawn 
	 if (deckTreasure + discardTreasure > 1)
	 {	 
		// Check that two cards are added to the player's hand, minus adventurer, for 1 total added
		testPassed = asserttrue(handCountAfter == handCountBefore + 1);	
		if (!testPassed)
		{
			edgeCases->test1Fail++;
			printf("Test iteration number %i, seed = %i: ", count, seed);
			printf("1. number of cards added to the players hand is incorrect\n");
			printf("Expected %i cards, counted %i cards, ", handCountBefore+1, handCountAfter);
			printf("starting count of deck was %i, ", origState.deckCount[player]); 
			printf("starting count of discard was %i, ", origState.discardCount[player]); 
			printf("starting count of hand was %i\n", origState.handCount[player]);
			printf("Number of gold in combined starting deck and discard: %i\n", deckGold + discardGold);
			printf("Number of silver in combined starting deck and discard: %i\n", deckSilver + discardSilver);
			printf("Number of copper in combined starting deck and discard: %i\n\n", deckCopper + discardCopper);
		}		
	
		// Check that the count of treasure in the player's hand is increased by 2	
		testPassed = asserttrue(newTreasure - oldTreasure == 2); 
		if (!testPassed)
		{
			edgeCases->test2Fail++;
			printf("Test iteration number %i, seed = %i: ", count, seed);
			printf("2. change in count of treasure in player hand is incorrect\n");
			printf("Expected %i treasure, counted %i treasure, ", oldTreasure + 2, newTreasure);
			printf("starting count of deck was %i, ", origState.deckCount[player]); 
			printf("starting count of discard was %i, ", origState.discardCount[player]); 
			printf("starting count of hand was %i\n", origState.handCount[player]);
			printf("Number of gold in combined starting deck and discard: %i\n", deckGold + discardGold);
			printf("Number of silver in combined starting deck and discard: %i\n", deckSilver + discardSilver);
			printf("Number of copper in combined starting deck and discard: %i\n\n", deckCopper + discardCopper);
		}
	 }
	 
	 // If there is one treasure card to be drawn
	 else if (deckTreasure + discardTreasure == 1)
	 {
		// check that one card is added to the hand, minus adventurer, for 0 total 
		testPassed = asserttrue(handCountAfter == handCountBefore);	
		if (!testPassed)
		{
			edgeCases->test1Fail++;
			printf("Test iteration number %i, seed = %i: ", count, seed);
			printf("1. number of cards added to the players hand is incorrect\n");
			printf("Expected %i cards, counted %i cards, ", handCountBefore, handCountAfter);
			printf("starting count of deck was %i, ", origState.deckCount[player]); 
			printf("starting count of discard was %i, ", origState.discardCount[player]); 
			printf("starting count of hand was %i\n", origState.handCount[player]);
			printf("Number of gold in combined starting deck and discard: %i\n", deckGold + discardGold);
			printf("Number of silver in combined starting deck and discard: %i\n", deckSilver + discardSilver);
			printf("Number of copper in combined starting deck and discard: %i\n\n", deckCopper + discardCopper);
		}		
	
		// check that count of treasure is one greater	
		testPassed = asserttrue(newTreasure - oldTreasure == 1); 
		if (!testPassed)
		{
			edgeCases->test2Fail++;
			printf("Test iteration number %i, seed = %i: ", count, seed);
			printf("2. change in count of treasure in player hand is incorrect\n");
			printf("Expected %i treasure, counted %i treasure, ", oldTreasure + 1, newTreasure);
			printf("starting count of deck was %i, ", origState.deckCount[player]); 
			printf("starting count of discard was %i, ", origState.discardCount[player]); 
			printf("starting count of hand was %i\n", origState.handCount[player]);
			printf("Number of gold in combined starting deck and discard: %i\n", deckGold + discardGold);
			printf("Number of silver in combined starting deck and discard: %i\n", deckSilver + discardSilver);
			printf("Number of copper in combined starting deck and discard: %i\n\n", deckCopper + discardCopper);
		}
	 }
	 
	// Else the count is zero
	else
	 {
		// check that the count of the hand is one less (since adventurer discarded) 
		testPassed = asserttrue(handCountAfter == handCountBefore - 1);	
		if (!testPassed)
		{
			edgeCases->test1Fail++;
			printf("Test iteration number %i, seed = %i: ", count, seed);
			printf("1. number of cards added to the players hand is incorrect\n");
			printf("Expected %i cards, counted %i cards, ", handCountBefore - 1, handCountAfter);
			printf("starting count of deck was %i, ", origState.deckCount[player]); 
			printf("starting count of discard was %i, ", origState.discardCount[player]); 
			printf("starting count of hand was %i\n", origState.handCount[player]);
			printf("Number of gold in combined starting deck and discard: %i\n", deckGold + discardGold);
			printf("Number of silver in combined starting deck and discard: %i\n", deckSilver + discardSilver);
			printf("Number of copper in combined starting deck and discard: %i\n\n", deckCopper + discardCopper);
		}		
	
		// check the count of treasure is unchanged	
		testPassed = asserttrue(newTreasure - oldTreasure == 1); 
		if (!testPassed)
		{
			edgeCases->test2Fail++;
			printf("Test iteration number %i, seed = %i: ", count, seed);
			printf("2. change in count of treasure in player hand is incorrect\n");
			printf("Expected %i treasure, counted %i treasure, ", oldTreasure, newTreasure);
			printf("starting count of deck was %i, ", origState.deckCount[player]); 
			printf("starting count of discard was %i, ", origState.discardCount[player]); 
			printf("starting count of hand was %i\n", origState.handCount[player]);
			printf("Number of gold in combined starting deck and discard: %i\n", deckGold + discardGold);
			printf("Number of silver in combined starting deck and discard: %i\n", deckSilver + discardSilver);
			printf("Number of copper in combined starting deck and discard: %i\n\n", deckCopper + discardCopper);
		}	 
	 }	 
	
	// Check that no non-treasure card is added to the player's hand
	int onlyTreasureAdded = 1; // default is true
	int countHandBefore[NUM_CARDS];
	int countHandAfter[NUM_CARDS];
	
	 for (i = 0; i<NUM_CARDS; i++)
	 {
		//call function to count that card in the hand and store that count
		countHandAfter[i] = countCardNum(i, state.hand[player], state.handCount[player]);
		countHandBefore[i] = countCardNum(i, origState.hand[player], origState.handCount[player]);
		
		if (countHandBefore[i] < countHandAfter[i]) // meaning was added to the hand
		{
			if (i != gold && i != silver && i != copper)
			{
				onlyTreasureAdded = 0;
			}	
		}
	}
	
	testPassed = asserttrue(onlyTreasureAdded);
	if (!testPassed)
		{
			edgeCases->test3Fail++;
			printf("Test iteration number %i, seed = %i: ", count, seed);
			printf("3. some non-treasure cards added to player hand\n");
			printf("starting count of deck was %i, ", origState.deckCount[player]); 
			printf("starting count of discard was %i, ", origState.discardCount[player]); 
			printf("starting count of hand was %i\n", origState.handCount[player]);
			printf("Number of gold in combined starting deck and discard: %i\n", deckGold + discardGold);
			printf("Number of silver in combined starting deck and discard: %i\n", deckSilver + discardSilver);
			printf("Number of copper in combined starting deck and discard: %i\n\n", deckCopper + discardCopper);
		}	 
		
	// Check that the adventurer card has been removed from the players hand
	// Count adventurer card before
	int adventurerBefore = countCardNum(adventurer, origState.hand[player], origState.handCount[player]);

	// Count adventurer card after
	int adventurerAfter = countCardNum(adventurer, state.hand[player], state.handCount[player]);
	
	// Adventurer should not be added to the hand, only removed
	testPassed = asserttrue(adventurerAfter == adventurerBefore-1);
	if (!testPassed)
		{
			edgeCases->test4Fail++;	
			printf("Test iteration number %i, seed = %i: ", count, seed);
			printf("4. adventurer card not removed from player hand\n");
			printf("Expected %i adventurers, counted %i adventurers, ", adventurerBefore-1, adventurerAfter);
			printf("starting count of deck was %i, ", origState.deckCount[player]); 
			printf("starting count of discard was %i, ", origState.discardCount[player]); 
			printf("starting count of hand was %i\n", origState.handCount[player]);
			printf("Number of gold in combined starting deck and discard: %i\n", deckGold + discardGold);
			printf("Number of silver in combined starting deck and discard: %i\n", deckSilver + discardSilver);
			printf("Number of copper in combined starting deck and discard: %i\n\n", deckCopper + discardCopper);
		}	 
	return 0;
}

/***********************************************************************************
 * Function: asserttrue
 * Description: displays a brief failure message if statement evaluates to false. 
 *				Returns	0 (false) if test failed and 1 (true) if test passed 
 **********************************************************************************/
int asserttrue(int statement)
{
	if(statement)
	{
		return 1;
	}
	else
	{
		printf("Test failed: ");
	}
	return 0;
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
 
 /*******************************************************************************
 * Function: randomDeck()
 * Description: gives a specified player a random deck of the specified size,
 *				containing cards from the supply array provided, as well as
 *				treasure, victory, and curse cards
 ******************************************************************************/
void randomDeck(struct gameState* state, int player, int deckSize, int supply[])
{
	int i;
	int randChoice;
	
	// set the deck count to the correct value
	state->deckCount[player] = deckSize;
	
	// fill the deck with random cards
	for (i=0; i<deckSize; i++)
	{
		randChoice = rand()%16; // generates a number between 0 and 15, cards 0-5 and then the 10 supply piles
		
		// a random treasure, victory, or curse card
		if (randChoice < 6)
		{
			state->deck[player][i] = randChoice;
		}
		
		// a random card from the 10 in the supply
		else
		{	
			state->deck[player][i] = supply[randChoice-6];
		}	
	}	
	
	return;	
}	

/*************************************************************************************** 
 *  Function: randomDiscard()
 *  Description: gives a specified player a random discard of the specified size,
 *				containing cards from the supply array provided, as well as
 *				treasure, victory, and curse cards
 ***************************************************************************************/
void randomDiscard(struct gameState* state, int player, int discardSize, int supply[])
{
	int i;
	int randChoice;
	
	// set the hand count to the correct value
	state->discardCount[player] = discardSize;
	
	// fill the hand with random cards
	for (i=0; i<discardSize; i++)
	{
		randChoice = rand()%16; // generates a number between 0 and 15, cards 0-5 and then the 10 supply piles
		
		// a random treasure, victory, or curse card
		if (randChoice < 6)
		{
			state->discard[player][i] = randChoice;
		}
		
		// a random card from the 10 in the supply
		else
		{	
			state->discard[player][i] = supply[randChoice-6];
		}	
	}	
	
	return;
}	

/*******************************************************************************************
 * Function: randomHand()
 * Description: gives a specified player a random hand of the specified size,
 *				containing cards from the supply array provided, as well as
 *				treasure, victory, and curse cards
 *******************************************************************************************/
void randomHand(struct gameState* state, int player, int handSize, int supply[])
{
	int i;
	int randChoice;
	
	// set the hand count to the correct value
	state->handCount[player] = handSize;
	
	// fill the hand with random cards
	for (i=0; i<handSize; i++)
	{
		randChoice = rand()%16; // generates a number between 0 and 15, cards 0-5 and then the 10 supply piles
		
		// a random treasure, victory, or curse card
		if (randChoice < 6)
		{
			state->hand[player][i] = randChoice;
		}
		
		// a random card from the 10 in the supply
		else
		{	
			state->hand[player][i] = supply[randChoice-6];
		}	
	}	
	
	return;
}	
 
 



	

/***************************************************************************
 * Filename: randomtestcard2.c
 * Author: Nina Lauritzen
 * Date: 11/2/3017
 * Description: Random tests for the village card function in 
 *				dominion.c 
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
};

int asserttrue(int statement);
int countCardNum(int numToCount, int array[], int arraySize);
int removedFromHand (struct gameState *stateBefore, struct gameState *stateAfter, int player, int cardRemoved);
int checkAddToDiscard(struct gameState *stateBefore, struct gameState *stateAfter, int player, int cardAdded);
int removedFromDeck(struct gameState *stateBefore, struct gameState *stateAfter, int player);
void randomHand(struct gameState* state, int player, int handSize, int supply[]);
void randomDeck(struct gameState* state, int player, int deckSize, int supply[]);
void randomDiscard(struct gameState* state, int player, int discardSize, int supply[]);
void randomVillageTest(int seed, int count, struct edge_counts* edges);

int main()
{
	int seed;
	seed = time(NULL); 
	srand(seed);
	
	// Print the seed so that tests can be reproduced
	printf("Random Village Card Tests, Seed = %i\n\n", seed);
	
	// Set up the struct to track edge cases
	struct edge_counts edgeCases;
	edgeCases.deckMin = 0;
	edgeCases.discardMin = 0;  
	edgeCases.handMin = 0;   
	edgeCases.deckMax = 0;   
	edgeCases.discardMax = 0;   
	edgeCases.handMax =0;
  	
	int count = 0; // tracks the iteration of the test
	int i;

	for (i = 0; i<2000; i++)
	{
		count++;	
		randomVillageTest(seed, count, &edgeCases);		
	}	
		
	// Print the counts of the edge cases hit:
	printf("\nTESTS OF VILLAGE CARD COMPLETE\n");
	printf("Number of times deck was empty: %i\n", edgeCases.deckMin);
	printf("Number of times deck was at max size: %i\n", edgeCases.deckMax);
	printf("Number of times discard was empty: %i\n", edgeCases.discardMin);
	printf("Number of times discard was at max size: %i\n", edgeCases.discardMax);
	printf("Number of times hand was at minimum: %i\n", edgeCases.handMin);
	printf("Number of times hand was at max size: %i\n", edgeCases.handMax); 	
	
	return 0;
}	

/***************************************************************************************
 * Function: randomVillageTest
 * Description: creates a gameState with a random player number, deck, discard, and hand.
 * Calls the function to play the vilage card.  Checks the game state afterwards to test
 * whether various requirements have been met  
 **************************************************************************************/ 

 void randomVillageTest(int seed, int count, struct edge_counts* edgeCases)
{
	
	// Set up a random game state
	
	struct gameState state;
	int supply[10];
	int cardNum;
	int i;
	int j;
	int duplicate = 0;
	
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
	player = rand()%(MAX_PLAYERS);
		
	// make it that player's turn
	state.whoseTurn = player;
	
	// Get a random size for the discard, deck, and hand	
	int discardSize, deckSize, handSize;
	
	deckSize = rand()%(MAX_DECK+1);
	if (deckSize == MAX_DECK)
	{
		edgeCases->deckMax++;  // track coverage of maximum
	}
	else if (deckSize == 0)
	{
		edgeCases->deckMin++; // track coverage of minimum
	}
	
	discardSize = rand()%(MAX_DECK+1);
	if(discardSize == MAX_DECK)
	{
		edgeCases->discardMax++;
	}	
	else if (discardSize == 0)
	{
		edgeCases->discardMin++;
	}
	
	handSize = rand()%MAX_HAND;
	handSize += 1;  // hand must have at least 1 card, the village card
	
	if(handSize == MAX_HAND)
	{
		edgeCases->handMax++;
	}
	else if (handSize == 1)
	{
		edgeCases->handMin++;
	}
 	
	// Create a random hand for the player
	randomHand(&state, player, handSize, supply);
	
	// Place the smithy card in a random position in the player's hand
	int handPos;
	handPos = rand()%handSize;
	state.hand[player][handPos] = village;
	
	// Create a random deck for the player	
	randomDeck(&state, player, deckSize, supply);

	// Create a random discard for the player
	randomDiscard(&state, player, discardSize, supply);
 
	// Make copy of original state
	struct gameState origState;
	origState = state;	
	
	// Play the village card
	cardEffect(village, 0, 0, 0, &state, handPos, 0); // value 0 for choices and bonus since N/A  

	int testPassed = 0;
	
	/*
	 * If the deck and discard did not both start off empty,
	 * test whether the deck or deck+discard loses exactly one card
	 */
	
	int cardRemoved = -1; // indicates no card, or card incorrectly removed
 
	if(origState.deckCount[player]!=0 || origState.discardCount[player]!=0)
	{	
		cardRemoved = removedFromDeck(&origState, &state, player);
	
		testPassed = asserttrue(cardRemoved != -1);
		if (!testPassed)
		{
			printf("Test iteration number %i, seed = %i: ", count, seed);
			printf("one and only one card was not removed from the deck\n");
			printf("Starting count of deck was %i, ", origState.deckCount[player]); 
			printf("starting count of discard was %i, ", origState.discardCount[player]); 
			printf("starting count of hand was %i\n", origState.handCount[player]);
			printf("Position of village card in player's hand was %i\n\n", handPos);
		}
	
		// Test that the count of the card removed from the deck is now one greater in the player's hand, or equal if village was drawn
		if (cardRemoved !=-1) // assuming a card was successfully removed
		{
			int countBefore = countCardNum(cardRemoved, origState.hand[player], origState.handCount[player]);
			int countAfter = countCardNum(cardRemoved, state.hand[player], state.handCount[player]);
			if(cardRemoved == village)
			{
				testPassed = asserttrue(countBefore == countAfter);
				if (!testPassed)
				{
					printf("Test iteration number %i, seed = %i: ", count, seed);
					printf("card removed from deck was not added to hand\n");

					printf("the card removed was card %i\n", cardRemoved);
					printf("the count of the card removed in the hand before was %i\n", countBefore);
					printf("the count of the card removed in the hand after was %i\n", countAfter);

					printf("Starting count of deck was %i, ", origState.deckCount[player]); 
					printf("starting count of discard was %i, ", origState.discardCount[player]); 
					printf("starting count of hand was %i\n", origState.handCount[player]);
	
					printf("Ending count of deck was %i, ", state.deckCount[player]);
					printf("Ending count of discard was %i, ", state.discardCount[player]);
					printf("Ending count of hand was %i\n", state.handCount[player]);

					printf("Position of village card in player's hand was %i\n\n", handPos);
				}			
			}	
			else
			{
				testPassed = asserttrue(countAfter == countBefore+1);
				if (!testPassed)
				{
					printf("Test iteration number %i, seed = %i: ", count, seed);
					printf("card removed from deck was not added to hand\n");

					printf("the card removed was card %i\n", cardRemoved);
					printf("the count of the card removed in the hand before was %i\n", countBefore);
					printf("the count of the card removed in the hand after was %i\n", countAfter);

					printf("Starting count of deck was %i, ", origState.deckCount[player]); 
					printf("starting count of discard was %i, ", origState.discardCount[player]); 
					printf("starting count of hand was %i\n", origState.handCount[player]);
	
					printf("Ending count of deck was %i, ", state.deckCount[player]);
					printf("Ending count of discard was %i, ", state.discardCount[player]);
					printf("Ending count of hand was %i\n", state.handCount[player]);


					printf("Position of village card in player's hand was %i\n\n", handPos);


				}					
			}	
		}
	}	
			
	// Test whether the player gains two actions
	
	testPassed = asserttrue(state.numActions == origState.numActions+2);
	if (!testPassed)
	{
		printf("Test iteration number %i, seed = %i: ", count, seed);
		printf("number of actions is incorrect\n");
		printf("Starting count of deck was %i, ", origState.deckCount[player]); 
		printf("starting count of discard was %i, ", origState.discardCount[player]); 
		printf("starting count of hand was %i\n", origState.handCount[player]);
		printf("Position of village card in player's hand was %i\n\n", handPos);
	}	
	
	// Test whether the village card is removed from the player's hand: whether village card count in hand is one less (if village card was not drawn)
	
	int villageCountBefore = countCardNum(village, origState.hand[player], origState.handCount[player]);
	int villageCountAfter = countCardNum(village, state.hand[player], state.handCount[player]);

	if(cardRemoved != village)
	{
		testPassed = asserttrue(villageCountAfter == villageCountBefore-1);
	}	
	else
	{
		testPassed = asserttrue(villageCountAfter == villageCountBefore);
	}	
	if (!testPassed)
	{
		printf("Test iteration number %i, seed = %i: ", count, seed);
		printf("count of village card in player's hand is incorrect\n");
		printf("Starting count of deck was %i, ", origState.deckCount[player]); 
		printf("starting count of discard was %i, ", origState.discardCount[player]); 
		printf("starting count of hand was %i\n", origState.handCount[player]);
		printf("Position of village card in player's hand was %i\n\n", handPos);
	}	
	
	// Test whether the village card is discarded
	int villageDiscarded = checkAddToDiscard(&origState, &state, player, village);
	testPassed = asserttrue(villageDiscarded);
	if (!testPassed)
	{
		printf("Test iteration number %i, seed = %i: ", count, seed);
		printf("village card not in discard pile\n");
		printf("Starting count of deck was %i, ", origState.deckCount[player]); 
		printf("starting count of discard was %i, ", origState.discardCount[player]); 
		printf("starting count of hand was %i\n", origState.handCount[player]);
		printf("Position of village card in player's hand was %i\n\n", handPos);
	}	
	
	return;	
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
			}
		}

		// Otherwise the counts should be equal, since no other cards were added or discarded 
	
		else
		{
			if (countHandBefore[i] != countHandAfter[i])
			{
				handCorrect = 0; // meaning hand incorrect
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
	
	int countDiscardBefore[NUM_CARDS]; // for case where discard is shuffled in
	for (i= 0; i<NUM_CARDS; i++)
	{
		countDiscardBefore[i] = 0;
	}
	
	// Variable to hold card removed (or flag as incorrect)
				
	int cardRemoved = -1; // holds number of card that was removed from deck from stateBefore to stateAfter			
	int found = 0; // tracks number of removed cards found, should be 1 if 1 card correctly removed

	// Update the count in the array for any cards in the player's deck before and after and compare
	for (i = 0; i<NUM_CARDS; i++)
	{
		//call function to count that card in the hand and store that count in the array
		countDeckAfter[i] = countCardNum(i, stateAfter->deck[player], stateAfter->deckCount[player]);
		countDeckBefore[i] = countCardNum(i, stateBefore->deck[player], stateBefore->deckCount[player]);
		
		// Adjust for the case where the discard pile was shuffled in
		if (stateBefore->discardCount[player] > 0 && stateAfter->discardCount[player] < 2) // means discard shuffled into deck
		{
			countDiscardBefore[i] = countCardNum(i, stateBefore->discard[player], stateBefore->discardCount[player]);
			countDeckBefore[i] += countDiscardBefore[i];
		}
			
		// If the count has decreased by 1, that card was removed
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

/***************************************************************************
 * Filename: randomtestcard1.c
 * Author: Nina Lauritzen
 * Date: 11/2/3017
 * Description: Random tests for the smithy card function in 
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
};

int asserttrue(int statement);
int countCardNum(int numToCount, int array[], int arraySize);
int numFromDeckToHand(struct gameState *stateBefore, struct gameState *stateAfter, int player);
int removedFromHand (struct gameState *stateBefore, struct gameState *stateAfter, int player);
int checkAddToDiscard(struct gameState *stateBefore, struct gameState *stateAfter, int player, int cardAdded);
void randomHand(struct gameState* state, int player, int handSize, int supply[]);
void randomDeck(struct gameState* state, int player, int deckSize, int supply[]);
void randomDiscard(struct gameState* state, int player, int discardSize, int supply[]);
int randomSmithyTest(int count, int seed, struct edge_counts* edges);

int main()
{
	// Seed the random number generator
	int seed;
	seed = time(NULL); 
	srand(seed);
	
	// Print the seed so that tests can be reproduced
	printf("Random Smithy Card Tests, Seed = %i\n\n", seed);
	int i;
	int count = 0;	
	
	// Set up the struct to track edge cases
	struct edge_counts edgeCases;
	edgeCases.deckMin = 0;
	edgeCases.discardMin = 0;  
	edgeCases.handMin = 0;   
	edgeCases.deckMax = 0;   
	edgeCases.discardMax = 0;   
	edgeCases.handMax =0;   

	// Run 2000 random tests of the smithy card function	
	for (i = 0; i<2000; i++)
	{
		count++;
		randomSmithyTest(count, seed, &edgeCases);			
	}	

	// Print the counts of the edge cases hit:
	printf("\nTESTS COMPLETE\n");
	printf("Number of times deck was empty: %i\n", edgeCases.deckMin);
	printf("Number of times deck was at max size: %i\n", edgeCases.deckMax);
	printf("Number of times discard was empty: %i\n", edgeCases.discardMin);
	printf("Number of times discard was at max size: %i\n", edgeCases.discardMax);
	printf("Number of times hand was at minimum: %i\n", edgeCases.handMin);
	printf("Number of times hand was at max size: %i\n", edgeCases.handMax); 
		
	return 0;
}	

/**********************************************************************************************
 * Function: randomSmithyTest
 * Description: creates a gameState with a random player number, deck, discard, and hand.
 * Calls the function to play the smithy card.  Checks the game state afterwards to test
 * whether various requirements have been met 
 *********************************************************************************************/ 
 
int randomSmithyTest(int count, int seed, struct edge_counts* edgeCases)
{

	// Set up a random game state
	
	struct gameState state;
	int supply[10];
	int cardNum;
	int i;
	int j;
	int duplicate = 0;	// flag for randomly selecting the same supply card twice
	
	for (i = 0; i<10; i++)
	{
		// Choose 10 random supply card piles
		duplicate = 0;
		do
		{
			supply[i] = rand()%NUM_CARDS;  // number between 0 and NUM_CARDS=1
		}
		while (supply[i] < 6); // redo if card chosen is treasure, curse, or victory card
		
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
	int players = MAX_PLAYERS;	// random player will be any of the possible players

	// Create and initialize a game state
	initializeResult = initializeGame(players, supply, seed, &state);

	/*
	 *  Randomize the key elements of the game state: player, hand size and contents,
	 *  deck size and cotnents, discard size and contents, position of the smithy card
	 *  in the hand 
         */
	 
	// Get a random player
	int player;
	player = rand()%MAX_PLAYERS;
	
	// Make it that player's turn
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
	
	handSize = rand()%MAX_HAND + 1; // Hand cannot be size zero because must contain at least smithy
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
	state.hand[player][handPos] = smithy;
	
	// Create a random deck for the player	
	randomDeck(&state, player, deckSize, supply);

	// Create a random discard for the player
	randomDiscard(&state, player, discardSize, supply);

	int cardsExpected; // number of cards expected to be added to hand by smithy
	
	/* 
	 * Figure out how many cards should be added to the player's hand
	 * This will be 3 if the combined deck and discard is 3 or greater
	 * 2 if the combined deck and discard is 2
	 * 1 if the combined deck and discard is 1
	 * 0 if the combined deck and discard is 0-5
	 */
	if (deckSize + discardSize > 2)
	{
		cardsExpected = 3;
	}	
	else
	{
		cardsExpected = deckSize + discardSize;
	}	
			
	// Make a copy of the game state
	struct gameState origState;
	origState = state;		
	
	// Play the smithy card
	cardEffect(smithy, 0, 0, 0, &state, handPos, 0);
	
	// Check that 3 cards are removed from the deck and also added to hand
		
	// Call the function to check how many cards were drawn
	int numCardsDrawn;
	numCardsDrawn = numFromDeckToHand(&origState, &state, player);
			
	int testPassed; // holds value returned by asserttrue()
	
	testPassed = asserttrue(numCardsDrawn == cardsExpected);
	if (!testPassed)
	{
		printf("Test iteration number %i, seed = %i: ", count, seed);
		printf("number of cards removed from deck, added to the hand is incorrect\n");
		printf("Expected %i cards, counted %i cards, ", cardsExpected, numCardsDrawn);
		printf("starting count of deck was %i, ", origState.deckCount[player]); 
		printf("starting count of discard was %i, ", origState.discardCount[player]); 
		printf("starting count of hand was %i\n\n", origState.handCount[player]);
	}		
	
	// Check that the hand is the correct number greater
	int expectedHandDiff; // expected difference between the old hand count and the new hand count
		
	expectedHandDiff = cardsExpected - 1; // because smithy discarded		 
			
	testPassed = asserttrue(state.handCount[player] == origState.handCount[player]+expectedHandDiff); 
	if (!testPassed)
	{
		printf("Test iteration number %i, seed = %i: ", count, seed);
		printf("hand count is not the correct number greater\n");
		printf("Expected %i cards in hand, ", origState.handCount[player]+expectedHandDiff);
		printf("counted %i cards in hand, ", state.handCount[player]);
		printf("starting count of deck was %i, ", origState.deckCount[player]); 
		printf("starting count of discard was %i, ", origState.discardCount[player]); 
		printf("starting count of hand was %i\n\n", origState.handCount[player]);
	}		
	
	// Check whether the discard pile was shuffled in; this affects the final count of the deck
	int discardAdded = 0;
	int expectedCount = 0;
	
	if ((state.discardCount[player] < 2) && (origState.discardCount[player] > 0))
	{
		discardAdded = 1;
	}
		
	/*
 	 * If the discard pile is shuffled in, the count of the current deck should equal
	 * the combined count of the starting deck and discard minus the number of cards
	 * expected to be drawn
	 */
	 
	if (discardAdded)
	{
		expectedCount = (origState.deckCount[player] + origState.discardCount[player]) - cardsExpected;
	}	
	
	/*
	 *  If the discard pile is not shuffled in, the count of the current deck should
	 *  equal the count of the starting deck minus the number of cards expected
	 *  to be drawn 
	 */
	else
	{	 
		expectedCount = origState.deckCount[player]-cardsExpected;
	}
	
	// Test whether the count of the deck is correct
	testPassed = asserttrue(state.deckCount[player] == expectedCount);
	
	if (!testPassed)
	{
		printf("Test iteration number %i, seed = %i: ", count, seed);
		printf("deck count is not the correct number less\n");
		printf("Expected %i cards in deck, ", expectedCount);
		printf("counted %i cards in deck, ", state.deckCount[player]);
		printf("starting count of deck was %i, ", origState.deckCount[player]); 
		printf("starting count of discard was %i, ", origState.discardCount[player]); 
		printf("starting count of hand was %i\n\n", origState.handCount[player]);
	}	
	

	// Check that the smithy card was added to the discard pile
	int smithyInDiscard =  checkAddToDiscard(&origState, &state, player, smithy);
	testPassed = asserttrue(smithyInDiscard);
	if (!testPassed)
	{
		printf("Test iteration number %i, seed = %i: ", count, seed);
		printf("smithy card was not added to discard\n");
		printf("Starting count of deck was %i, ", origState.deckCount[player]); 
		printf("starting count of discard was %i, ", origState.discardCount[player]); 
		printf("starting count of hand was %i\n\n", origState.handCount[player]);
	}		
	
	// Check that the smithy card was removed from the player's hand
	//int removedFromHand (struct gameState *stateBefore, struct gameState *stateAfter, int player, int cardRemoved)
	int smithyRemoved = removedFromHand(&origState, &state, player);
	testPassed = asserttrue(smithyRemoved);
	if (!testPassed)
	{
		printf("Test iteration number %i, seed = %i: ", count, seed);
		printf("smithy card was not removed from hand\n");
		printf("Starting count of deck was %i, ", origState.deckCount[player]); 
		printf("starting count of discard was %i, ", origState.discardCount[player]); 
		printf("starting count of hand was %i\n\n", origState.handCount[player]);
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
	
	// Loop through the pile/hand of cards and count the number of the given card
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
 * Function: numFromDeckToHand
 * Description:  To be called after smithy card is played.
 *				 Counts and returns the count of the number of cards that have 
 *               been removed from the deck and moved to the hand, or returns -1 
 *               if the count is invalid. 
 *				 Counts one additional card as being in the hand (smithy), to 
 *				 adjust for the fact that smithy was once in the hand but will have
 *				 been discarded
 ***********************************************************************************/

int numFromDeckToHand(struct gameState *stateBefore, struct gameState *stateAfter, int player)
{

	printf(" ");

	int deckSizeBefore = stateBefore->deckCount[player];
	int deckCtBefore[deckSizeBefore];

	int deckSizeAfter = stateAfter->deckCount[player];
	int deckCtAfter[deckSizeAfter];

	int handSizeBefore = stateBefore->handCount[player];
	int handCtBefore[handSizeBefore];

	int handSizeAfter = stateAfter->handCount[player];
	int handCtAfter[handSizeAfter];

	int i;

	// If the discard was shuffled in, flag this
	int discardAdded = 0;
	
	if ((stateAfter->discardCount[player] < 2) && (stateBefore->discardCount[player] > 0))
	{
		discardAdded = 1;
	}

	int inDiscard = 0; // will hold count of a card in discard if applicable
	
	// initialize all counts to 0
	 
	for (i = 0; i<handSizeBefore; i++ )
	{
		handCtBefore[i]=0;
	}

	for (i = 0; i<handSizeAfter; i++)
	{
		handCtAfter[i]=0;
	}

	for (i = 0; i<deckSizeBefore; i++)
	{
		deckCtBefore[i]=0;
	}

	for (i = 0; i<deckSizeAfter; i++)
	{
		deckCtAfter[i]=0;
	}	

	int deckDiff;
	int handDiff;
	int found = 0; // counter of cards found that have been added to hand and removed from deck

	//for each card in the card count, count it in the deck before and after
	for (i=0; i<NUM_CARDS; i++)
	{
		deckCtBefore[i] = countCardNum(i, stateBefore->deck[player], deckSizeBefore);
		
		if (discardAdded)
		{
			inDiscard = countCardNum(i, stateBefore->discard[player], stateBefore->discardCount[player]);
			deckCtBefore[i] += inDiscard;
		}	
		
		deckCtAfter[i] = countCardNum(i, stateAfter->deck[player], deckSizeAfter);

		deckDiff = deckCtBefore[i]-deckCtAfter[i];
		
		//if the count is 1 or more less, count it in the hand
		if(deckDiff > 0)
		{
			handCtBefore[i] = countCardNum(i, stateBefore->hand[player], handSizeBefore);
			handCtAfter[i] = countCardNum(i, stateAfter->hand[player], handSizeAfter);

			/*
			 * Smithy card will have been discarded, so add one to handCountAfter
			 * This covers the case where smithy is one of the cards drawn
			 */
  			
			if(i==smithy) 
			{
				handCtAfter[i]++;
			}

			handDiff = handCtAfter[i]-handCtBefore[i];

			if(handDiff == deckDiff)
			{
				found+=deckDiff;
			}
			
			else
 			{
				return -1; // difference in deck must match difference in hand
			}
		}		
		else if(deckDiff != 0)
		{
			return -1; // no cards should be added to the deck by smithy		
		} 
	}
		
	return found;
}

/**********************************************************************
 * Function: smithyDrawn()
 * Description: Checks whether the smithy card was one of the cards
 *              drawn by the player because of the smithy card.
 *		a) If there is one or no cards in the discard pile 
 *		and the discard	started off non-empty, discard 
 *		pile was shuffled in.
 *		(b) Otherwise the discard started off empty and could
 *		not have been shuffled in, or the discard was non-empty
 *		but not shuffled in.  
 *		In case (a), checks the count before of combined 
 *		smithy cards in the discard and deck.
 *		If this number is more than the cards currently in the 
 *		deck, smithy was drawn(by that number).  Otherwise
 *		smithy was not drawn.  
 *		In case (b), checks the count before and after of
 *		smithy cards in the deck. 
 *		Returns the number of smithy cards drawn
 ***********************************************************************/

 int smithyDrawn(struct gameState *stateBefore, struct gameState *stateAfter, int player)
 {
	 int countDrawn = 0; // default is no smithy cards drawn
	 //if the discard pile size is 0 or 1 and the starting discard size was not zero
	 //meaning the discard pile was shuffled into the player's hand
	 if (stateAfter->discardCount[player] < 2 && stateBefore->discardCount[player] > 0)
	 {		
		//get the count of smithy cards in the deck before
		int smithyDeckBefore = countCardNum(smithy, stateBefore->deck[player], stateBefore->deckCount[player]);
		//get the count of smithy cards in the discard before
		int smithyDiscardBefore = countCardNum(smithy, stateBefore->discard[player], stateBefore->discardCount[player]);
		//add them together
		int totalBefore = smithyDeckBefore + smithyDiscardBefore;
		//get the count of smithy cards in the deck after
		int totalAfter = countCardNum(smithy, stateAfter->deck[player], stateAfter->deckCount[player]);
		// if the count of smithy cards in the deck after is less than or equal to the combined before
		if (totalBefore >= totalAfter) // meaning something removed from the deck
		{
			// then the countDrawn is equal to the difference
			countDrawn = totalBefore - totalAfter; // will be zero if no change
		}
		// else there is an error, countDrawn = -1
		else
		{
			countDrawn =-1;
		}
	 }	
	//else either the discard pile is >1 or the starting discard size was zero
	else
	{		
		// in which case count the smithy cards in the deck before
		int totalBefore = countCardNum(smithy, stateBefore->deck[player], stateBefore->deckCount[player]);
		// count the smithy cards in the deck after
		int totalAfter = countCardNum(smithy, stateAfter->deck[player], stateAfter->deckCount[player]);
		// if the count after is less than or equal to the count before
		if (totalBefore >= totalAfter)
		{
			countDrawn = totalBefore-totalAfter;
		}	
		else
		{	
			// else there is an error, countDrawn = -1
			countDrawn = -1;
		}
	}		
	return countDrawn;	 
 }
 
/***********************************************************************
 *  Function: removedFromHand
 *  Description: Checks whether smithy card has been removed
 *               from players hand; multiple cards may be removed
 *               but no cards added.  Returns -1 when count of  
 *               removed card indicates error, for instance,
 *               when the count is higher after its removal.
 *				 Accounts for the fact that smithy may have been
 *				 one of the cards drawn.  Smithy count in hand
 *				 should be one less than combined count of smithy 
 *				 in hand before and smithy drawn 
 ***********************************************************************/ 

 int removedFromHand (struct gameState *stateBefore, struct gameState *stateAfter, int player)
{
	// Variable to flag hand count as correct (or incorrect)
	int handCorrect = 1; // default is true (correct)
	int cardRemoved = smithy;
	
	int expectedSmithy = 0;
	int smithyHandBefore = countCardNum(smithy, stateBefore->hand[player], stateBefore->handCount[player]);
	expectedSmithy += smithyHandBefore;
	int drawnSmithy = smithyDrawn(stateBefore, stateAfter, player);
	if (drawnSmithy == -1)
	{
		handCorrect = 0; // there is an error
	}	
	else
	{	
		expectedSmithy += drawnSmithy;
		expectedSmithy--; // because smithy should be removed from hand; this will be zero if started with only played card	
		int smithyCount = countCardNum(smithy, stateAfter->hand[player], stateAfter->handCount[player]);
		if (smithyCount != expectedSmithy)
		{
			handCorrect = 0;
		}	
	}	
	
	return handCorrect;
}

/************************************************************************
 * Function: checkAddToDiscard
 * Description: Takes two gameState parameters, a player, and a card and
 *              checks to make sure that discarded card ends up in the
 *              discard pile 
 ************************************************************************/ 

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
				discardAdded = 0; // meaning hand incorrect
			}
		}
		
	}  
	
	return discardAdded;
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


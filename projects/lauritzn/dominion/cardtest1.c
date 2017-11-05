/*****************************************************************************************
 * File: cardtest1.c
 * Author: Nina Lauritzen
 * Date: 10/18/2017
 * Description: This file contains unit tests for the function that plays the smithy card
 *****************************************************************************************/

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

	printf("Checking cards from deck to hand");
	int deckSizeBefore = stateBefore->deckCount[player];
	int deckCtBefore[deckSizeBefore];

	int deckSizeAfter = stateAfter->deckCount[player];
	int deckCtAfter[deckSizeAfter];

	int handSizeBefore = stateBefore->handCount[player];
	int handCtBefore[handSizeBefore];

	int handSizeAfter = stateAfter->handCount[player];
	int handCtAfter[handSizeAfter];

	int i;

	// initialize all counts to 0
	 
	for (i = 0; i<deckSizeBefore; i++ )
	{
		deckCtAfter[i]=0;
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
		deckCtBefore[i]=0;
	}	

	int deckDiff;
	int handDiff;
	int found = 0; // counter of cards found that have been added to hand and removed from deck

	//for each card in the card count, count it in the deck before and after
	for (i=0; i<NUM_CARDS; i++)
	{
		deckCtBefore[i] = countCardNum(i, stateBefore->deck[player], deckSizeBefore);
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

/*****************************************************************
 *  Function: removedFromHand
 *  Description: Checks whether a specific card has been removed
 *               from players hand; multiple cards may be removed
 *               but no cards added.  Returns -1 when count of  
 *               removed card indicates error, for instance,
 *               when the count is higher after its removal 
 *****************************************************************/ 

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

		// If the card being counted is the discarded card, the count in the new hand should be at least one less
		// countHandBefore should be greater than countHandAfter; it cannot be less than or equal
		if (i == cardRemoved)
		{
			if (countHandBefore[i] <= countHandAfter[i])
			{					
				handCorrect = 0; //meaning hand incorrect
			}
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



int main()
{
        /*
 	*  CASE #1: Case where more than 3 cards in deck
 	*
 	*/ 
		int randomSeed = 100;
		int players = 2;
		int i;
		int playerNum = 0;  // concerned with player 0, first player 
		
		struct gameState state1;
		int initializeResult;
		int supply1[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

		initializeResult = initializeGame(players, supply1, randomSeed, &state1);
		// Make sure the turn belongs to the player we're interested in 
		state1.whoseTurn = playerNum;			
 
		// Place the smithy card in first position in the player's hand
		state1.hand[playerNum][0] = smithy;			
			
		// Make a copy of the game state
		struct gameState origState1;
		origState1 = state1;		
		
		// play the smithy card
		cardEffect(smithy, 0, 0, 0, &state1, 0, 0);

		// Check that 3 cards are removed from the deck and also added to hand
		
		int numCardsDrawn;
		numCardsDrawn = numFromDeckToHand(&origState1, &state1, playerNum);

		printf("smithy card: deck count > 3, test number of cards removed from deck, added to the hand = 3: ");
		asserttrue(numCardsDrawn == 3);
		
		// Check that the hand count is 2 greater (only 2 because smithy discarded)
		
		printf("smithy card: deck count > 3, test that hand count is 2 greater (3 - smithy): ");
		asserttrue(state1.handCount[playerNum] == origState1.handCount[playerNum]+2); 

		// Check that the deck count is 3 less
		printf("smithy card: deck count > 3, test that the deck count is 3 less: ");
		asserttrue(state1.deckCount[playerNum] == origState1.deckCount[playerNum]-3);

		// Check that the smithy card was added to the discard pile
		printf("smithy card: deck count > 3, test that the smithy card was added to the discard pile: ");
		int smithyInDiscard =  checkAddToDiscard(&origState1, &state1, playerNum, smithy);
		asserttrue(smithyInDiscard);

		// Check that the smithy card was removed from the player's hand
		//int removedFromHand (struct gameState *stateBefore, struct gameState *stateAfter, int player, int cardRemoved)
		printf("smithy card: deck count > 3, test that smithy card emoved from player's hand: ");
		int smithyRemoved = removedFromHand(&origState1, &state1, playerNum, smithy);
		asserttrue(smithyRemoved);

	/*
         * CASE #2: 2 cards in deck, with the discard pile empty (so cannot add more cards)
         */
		randomSeed = 100;
		players = 2;
		playerNum = 0;  // concerned with player 0, first player 
		
		struct gameState state2;
		int supply2[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

		// Minimum information to initialize game is number of players, list of supply cards, random seed, and the gameState struct
		initializeResult = initializeGame(players, supply2, randomSeed, &state2);

		// Make sure the turn belongs to the player we're interested in 
		state2.whoseTurn = playerNum;			

		// Place the smithy card in first position in the player's hand
		state2.hand[playerNum][0] = smithy;			

		// Change the deck so that it contains only the first two cards
		int deckCount = state2.deckCount[playerNum];
		for (i=deckCount-1; i>1; i--) // i is the index of the card in the deck array
		{
			state2.deck[playerNum][i] = -1;
			state2.deckCount[playerNum]--;
		}

		// Make a copy of the game state
			struct gameState origState2;
			origState2 = state2;		

		// play the smithy card
		cardEffect(smithy, 0, 0, 0, &state2, 0, 0);
	
		numCardsDrawn = numFromDeckToHand(&origState2, &state2, playerNum);

		//printf("Number of cards drawn is counted as %i", numCardsDrawn);  
		
		printf("smithy card: deck count = 2, test that number cards removed from, added to hand = 2: ");
		asserttrue(numCardsDrawn == 2);
		
		// Check that the hand count is 2 greater (only 2 because smithy discarded)
		
		printf("smithy card: deck count = 2, test that the hand count is 1 greater (2 - smithy): ");
		asserttrue(state2.handCount[playerNum] == origState2.handCount[playerNum]+1); 

		// Check that the deck count is 2 less
		printf("smithy card: deck count = 2, test that the deck count is 2 less: ");
		asserttrue(state2.deckCount[playerNum] == origState2.deckCount[playerNum]-2);

		// Check that the smithy card was added to the discard pile
		printf("smithy card: deck count = 2, test that smithy card added to the discard pile: ");
		smithyInDiscard =  checkAddToDiscard(&origState2, &state2, playerNum, smithy);
		asserttrue(smithyInDiscard);

		// Check that the smithy card was removed from the player's hand
		//int removedFromHand (struct gameState *stateBefore, struct gameState *stateAfter, int player, int cardRemoved)
		printf("smithy card: deck count = 2, test that smithy card removed from player's hand: ");
		smithyRemoved = removedFromHand(&origState2, &state2, playerNum, smithy);
		asserttrue(smithyRemoved);


	/*
 	 * CASE 3: Exactly 3 cards in deck 
 	 */	

		randomSeed = 100;
		players = 2;
		playerNum = 0;  // concerned with player 0, first player 
		
		struct gameState state3;
		int supply3[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

		initializeResult = initializeGame(players, supply3, randomSeed, &state3);

		// Make sure the turn belongs to the player we're interested in 
		state3.whoseTurn = playerNum;			

		// Place the smithy card in first position in the player's hand
		state3.hand[playerNum][0] = smithy;			

		// Change the deck so that it contains only the first three cards
		deckCount = state3.deckCount[playerNum];
		for (i=deckCount-1; i>2; i--) // i is the index of the card in the deck array
		{
			state3.deck[playerNum][i] = -1;
			state3.deckCount[playerNum]--;
		}

		// Make a copy of the game state
		struct gameState origState3;
		origState3 = state3;		

		// play the smithy card
		cardEffect(smithy, 0, 0, 0, &state3, 0, 0);
	
		numCardsDrawn = numFromDeckToHand(&origState3, &state3, playerNum);

		//printf("Number of cards drawn is counted as %i", numCardsDrawn);  
		
		printf("smithy card: deck count = 3, Test that the number of cards removed from deck, added to hand = 3: ");
		asserttrue(numCardsDrawn == 3);
		
		// Check that the hand count is 2 greater (only 2 because smithy discarded)
		
		printf("smithy card: deck count = 3, test that the hand count is 2 greater (3 - smithy): ");
		asserttrue(state3.handCount[playerNum] == origState3.handCount[playerNum]+2); 

		// Check that the deck count is 2 less
		printf("smithy card: deck count = 3, test that the deck count is 3 less: ");
		asserttrue(state3.deckCount[playerNum] == origState3.deckCount[playerNum]-3);

		// Check that the smithy card was added to the discard pile
		// 
		printf("smithy card: deck count = 3, test that the smithy card was added to the discard pile: ");
		smithyInDiscard =  checkAddToDiscard(&origState3, &state3, playerNum, smithy);
		asserttrue(smithyInDiscard);

		// Check that the smithy card was removed from the player's hand
		//int removedFromHand (struct gameState *stateBefore, struct gameState *stateAfter, int player, int cardRemoved)
		printf("smithy card: deck count = 3,  test that the smithy card was removed from the player's hand: ");
		smithyRemoved = removedFromHand(&origState3, &state3, playerNum, smithy);
		asserttrue(smithyRemoved);


	/*
         * CASE #4: no cards in deck, >3 cards in discard
         */

		randomSeed = 100;
		players = 2;
		playerNum = 0;  // concerned with player 0, first player 
		
		struct gameState state4;
		int supply4[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

		// Minimum information to initialize game is number of players, list of supply cards, random seed, and the gameState struct
		initializeResult = initializeGame(players, supply4, randomSeed, &state4);

		// Make sure the turn belongs to the player we're interested in 
		state4.whoseTurn = playerNum;			

		// Place the smithy card in first position in the player's hand
		state4.hand[playerNum][0] = smithy;			

		// Empty the deck and move it to the discard, which starts off empty from initializeGame()
		deckCount = state4.deckCount[playerNum];
		for (i=deckCount-1; i>=0; i--) // i is the index of the card in the deck array
		{
			state4.discard[playerNum][i] = state4.deck[playerNum][i];
			state4.discardCount[playerNum]++;
			state4.deck[playerNum][i] = -1;
			state4.deckCount[playerNum]--;
		}

		// Make a copy of the game state
		struct gameState origState4;
		origState4 = state4;		

		// play the smithy card
		cardEffect(smithy, 0, 0, 0, &state4, 0, 0);
	
	
		numCardsDrawn = numFromDeckToHand(&origState4, &state4, playerNum);

		printf("smithy card, deck not discard empty, test that number of cards removed from deck, added to hand = 3: ");
		asserttrue(numCardsDrawn == 3);
		
		// Check that the hand count is 2 greater (only 2 because smithy discarded)
		
		printf("smithy card, deck not discard empty, Test that the hand count is 2 greater (3 - smithy): ");
		asserttrue(state4.handCount[playerNum] == origState4.handCount[playerNum]+2); 

		// Check that the discard count is 1 
		printf("smithy card, deck not discard empty, Test that discard count = 1 (smithy discarded): ");
		asserttrue(state4.discardCount[playerNum] == 1);

		// Check that the deck is the size of the original discard pile - 3
		printf("smithy card, deck not discard empty, test that the deck is the size of the original discard - 3: ");
		asserttrue(state4.deckCount[playerNum] == origState4.discardCount[playerNum]-3);	
		 
		// Check that the smithy card was added to the discard pile
		printf("smithy card, deck not discard empty, test that the smithy card was added to the discard pile: ");
		smithyInDiscard =  checkAddToDiscard(&origState4, &state4, playerNum, smithy);
		asserttrue(smithyInDiscard);

		// Check that the smithy card was removed from the player's hand
		printf("smithy card, deck not discard empty, test that smithy card was removed from the player's hand: ");
		smithyRemoved = removedFromHand(&origState3, &state3, playerNum, smithy);
		asserttrue(smithyRemoved);
  
	return 0;
}


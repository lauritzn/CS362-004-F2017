/*
 * File: unittest4.c
 * Author: Nina Lauritzen
 * Date: 10/18/2017
 * Description: unit tests for the drawCard() function
 */ 

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define SHOW_MESSAGES 1
#define NUM_CARDS 27 // constant which expresses count of supply cards(0-26)


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
 * Function: addedToHand
 * Description: takes two game states and a player number and returns the number of
 *              the card added to the second game state but not in the first.  Returns
 *              -1 if no card correctly added.  Correctness means one and only one
 *              card added.
 **************************************************************************************/ 

int addedToHand (struct gameState *stateBefore, struct gameState *stateAfter, int player)
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
			
	int cardAdded = -1; // holds number of card that was added to hand			
	int found = 0; // tracks number of added cards found

	// Update the count in the array for cards in the player's hand before and after and compare
	for (i = 0; i<NUM_CARDS; i++)
	{
		//call function to count that card in the hand and store that count in the array
		countHandAfter[i] = countCardNum(i, stateAfter->hand[player], stateAfter->handCount[player]);
		countHandBefore[i] = countCardNum(i, stateBefore->hand[player], stateBefore->handCount[player]);

		if (countHandBefore[i] == countHandAfter[i]-1)
		{
			cardAdded = i;
			found++;
		}
		else if (countHandBefore[i]!=countHandAfter[i])
		{
			return -1; // meaning some card was lost from the hand, so there is a bug 
		}
	
	}

	if (found !=1) // if no added card has been found, or too many have been found
	{
		return -1; // meaning hand invalid
	}				

	return cardAdded;
}

/*************************************************************************************************
 *  Function: removedFromDeck
 *  Description: Checks whether one and only one card has been removed from a player's deck
 *	             from one game state to another.  Returns -1 if a card has not been correctly 
 *				 removed from the deck, otherwise returns the  number of the card removed
 *************************************************************************************************/ 

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


int main()
{


     /***************************************************************
     * Case #1: Deck is empty and discard is empty
     ***************************************************************/ 
		
		// Set up the game with an empty deck and empty discard
		
		int randomSeed = 1000;
		int players = 2;
	
		struct gameState state1;
		int initializeResult;
		int supply1[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

		initializeResult = initializeGame(players, supply1, randomSeed, &state1);

		// Empty the deck
		int i;
		int deckCount = state1.deckCount[0];
		for (i=0; i<deckCount; i++)
		{
			state1.deck[0][i] = -1; // -1 means card is not in deck
			state1.deckCount[0]--;
		} 

		// Check that the function returns -1
		printf("drawCard: test for return value -1 when deck and discard empty: ");
		asserttrue(drawCard(0, &state1)==-1); 

	
	/***************************************************************
 	 * Case #2: Deck is empty and discard is non-empty
 	 ***************************************************************/
		
		randomSeed = 1000;
		players = 2;
	
		struct gameState state2;
		int supply2[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

		initializeResult = initializeGame(players, supply2, randomSeed, &state2);

		// Empty the deck
		deckCount = state2.deckCount[0];
		for (i=0; i<deckCount; i++)
		{
			state2.deck[0][i] = -1; // -1 means card is not in deck
			state2.deckCount[0]--;
		} 

		// Add 5 cards to the discard pile
		state2.discard[0][0] = embargo;
		state2.discard[0][1] = adventurer;
		state2.discard[0][2] = sea_hag;
		state2.discard[0][3] = smithy;
		state2.discard[0][4] = village;

		state2.discardCount[0] = 5;  // Update the count

		// Make a copy of the game state
		struct gameState origState2;
		origState2 = state2;		

		// Draw a card
		drawCard(0, &state2);	

		// origState had an empty deck, which will now contain the discard
		// so to compare its to the new deck we need to make the old discard the deck
		origState2.deck[0][0] = embargo;
		origState2.deck[0][1] = adventurer;
		origState2.deck[0][2] = sea_hag;
		origState2.deck[0][3] = smithy;
		origState2.deck[0][4] = village;

		origState2.deckCount[0] = 5;  // Update the count

		int discardCount = origState2.discardCount[0];
		for (i=0; i<discardCount; i++)
		{
			origState2.discard[0][i] = -1; // -1 means card is not in deck
			origState2.discardCount[0]--;
		} 
 	
		// Make sure the card got added to the player's hand
		
		int cardAdded = addedToHand(&origState2, &state2, 0);
		
		printf("drawCard: deck only empty, test for exactly one card added to hand: ");
		asserttrue(cardAdded != -1); // means invalid hand

		// Make sure the card got removed from the player's deck 

		int cardRemoved = removedFromDeck(&origState2, &state2, 0);

		printf("drawCard: deck only empty, test for exactly one card removed from deck: ");

		asserttrue(cardRemoved != -1); // means invalid deck
		
		// Make sure card added is the same as the one removed! 

		printf("drawCard: deck only empty, test that card added is same as one removed");		
		asserttrue(cardAdded == cardRemoved);

	/**************************************************************
 	 * Case #3: Deck is non-empty, discard is non-empty, player hand is non-empty 
 	 **************************************************************/  

		randomSeed = 1000;
		players = 2;
	
		struct gameState state3;
		int supply3[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

		// Minimum information to initialize game is number of players, list of supply cards, random seed, and the gameState struct
		initializeResult = initializeGame(players, supply3, randomSeed, &state3);

		// Add 5 cards to the discard pile
		state2.discard[0][0] = embargo;
		state2.discard[0][1] = adventurer;
		state2.discard[0][2] = sea_hag;
		state2.discard[0][3] = smithy;
		state2.discard[0][4] = village;

		state2.discardCount[0] = 5;  // Update the count

		// Make a copy of the game state
		struct gameState origState3;
		origState3 = state3;		

		// Draw a card
		drawCard(0, &state3);	
	
		// Make sure the card got added to the player's hand
		cardAdded = addedToHand(&origState2, &state2, 0);
		
		printf("drawCard: piles non-empty, test that exactly one card added to hand: ");
		asserttrue(cardAdded != -1); // means invalid hand

		// Make sure the card got removed from the player's deck 

		cardRemoved = removedFromDeck(&origState2, &state2, 0);

		printf("drawCard: piles non-empty, test for exactly one card removed from the deck: ");
		asserttrue(cardRemoved != -1); // means invalid deck
		
		// Make sure card added is the same as the one removed! 

		printf("drawCard: piles non-empty, test that card added is same as one removed: ");		
		asserttrue(cardAdded == cardRemoved);

	/*************************************************************
 	 * Case #4: Edge case: player hand is empty 
 	 ************************************************************/

		randomSeed = 1000;
		players = 2;
	
		struct gameState state4;
		int supply4[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

		initializeResult = initializeGame(players, supply4, randomSeed, &state4);

		// Add 5 cards to the discard pile
		state4.discard[0][0] = embargo;
		state4.discard[0][1] = adventurer;
		state4.discard[0][2] = sea_hag;
		state4.discard[0][3] = smithy;
		state4.discard[0][4] = village;

		state4.discardCount[0] = 5;  // Update the count

		int countDeckBefore[NUM_CARDS];
	
		//call function to count that card in the hand and store that count in the array
		int j;
		int loopCount = state4.deckCount[0];
		for (j=0; j<loopCount; j++)
		{ 
			countDeckBefore[j] = countCardNum(j, state4.deck[0], state4.deckCount[0]);
		}	

		// Empty the player's hand
		int handCount = state4.handCount[0];
		for (i=0; i<handCount; i++)
		{
			state4.hand[0][i] = -1; // -1 means card is not in hand
			state4.handCount[0]--;
		} 

		//make a copy of the game state
		struct gameState origState4;
		origState4 = state4;		

		// Draw a card
		drawCard(0, &state4);	
	
		// Make sure the card got added to the player's hand
		
		cardAdded = addedToHand(&origState4, &state4, 0);
		
		printf("drawCard: hand empty, test that exactly one card was added to the hand: ");
		asserttrue(cardAdded != -1); // means invalid hand

		// Make sure the card got removed from the player's deck 

		cardRemoved = removedFromDeck(&origState4, &state4, 0);

		printf("drawCard: hand empty, test that exactly one card was removed from the deck: ");
		asserttrue(cardRemoved != -1); // means invalid deck
		
		// Make sure card added is the same as the one removed! 

		printf("drawCard: hand empty, test that card added is same as one removed: ");		
		asserttrue(cardAdded == cardRemoved);

	/***************************************************************
 	 * Case #5: Edge case: deck has one card (empties when drawn)
 	 ***************************************************************/  
	 
		randomSeed = 1000;
		players = 2;
	
		struct gameState state5;
		int supply5[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

		initializeResult = initializeGame(players, supply5, randomSeed, &state5);

		// Empty the deck but leave one card
		deckCount = state5.deckCount[0];
		for (i=deckCount-1; i>0; i--)    // loops one time less than the count of cards
		{
			state5.deck[0][i] = -1; // -1 means card is not in deck
			state5.deckCount[0]--;
		} 

		// Add 5 cards to the discard pile
		state5.discard[0][0] = embargo;
		state5.discard[0][1] = adventurer;
		state5.discard[0][2] = sea_hag;
		state5.discard[0][3] = smithy;
		state5.discard[0][4] = village;

		state5.discardCount[0] = 5;  // Update the count

		// Make a copy of the game state
		struct gameState origState5;
		origState5 = state5;		

		// Draw a card
		drawCard(0, &state5);	
	
		// Make sure the card got added to the player's hand
		
		cardAdded = addedToHand(&origState5, &state5, 0);
		
		printf("drawCard: deck empties when drawn, test for exactly one card added to hand: ");
		asserttrue(cardAdded != -1); // means invalid hand

		// Make sure the card got removed from the player's deck 

		cardRemoved = removedFromDeck(&origState5, &state5, 0);

		printf("drawCard: deck empties when drawn, test for exactly one card removed from deck: ");
		asserttrue(cardRemoved != -1); // means invalid deck
		
		// Make sure card added is the same as the one removed! 

		printf("drawCard: deck empties when drawn, test that card added is same as one removed: ");		
		asserttrue(cardAdded == cardRemoved);

	return 0;	
}

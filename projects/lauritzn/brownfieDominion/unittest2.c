/********************************************************************************
 * File: unittest2.c
 * Author: Nina Lauritzen
 * Date: 10/18/2017 
 * Description: unit tests for discardCard()
 ********************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

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
		printf("passed\n");
	}
	else
	{
		printf("failed!\n");
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

/***************************************************************************************
 * Function: removedFromHand
 * Description: Checks whether given card has been correctly removed from a player's
 *              hand; assumes no other cards have been added or removed 
 ***************************************************************************************/ 

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

/************************************************************************************************************
 * Function: checkAddToDiscard
 * Description: Takes two gameState parameters, a player, and a card; checks to make sure the given card 
 *              ends up in the discard pile; assumes no other cards were added or removed 
 ************************************************************************************************************/ 

int checkAddToDiscard(struct gameState *stateBefore, struct gameState *stateAfter, int player, int cardAdded)
{
	// Create arrays for counting the number of each type of card before and after and initialize to zero

	int countPileBefore[NUM_CARDS]; // array stores count of cards of that type in discard
	
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

	// Variable to flag discard count as correct (or incorrect)		
	int discardAdded = 1; // default is true (correct)

	// Update the count in the array for any cards in the player's discard pile before and after and compare
	for (i = 0; i<NUM_CARDS; i++)
	{
		//call function to count that card in the discard pile and store that count in the array
		countPileAfter[i] = countCardNum(i, stateAfter->discard[player], stateAfter->discardCount[player]);
		countPileBefore[i] = countCardNum(i, stateBefore->discard[player], stateBefore->discardCount[player]);

		// If the card being counted is the discarded card, the count in the new discard pile should be one more
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
				discardAdded = 0; // meaning discard pile incorrect	
			}
		}
		
	}  
	return discardAdded;
}

/*******************************************************************************************
 * Function: checkPilesUnchanged
 * Description: Takes two gameState parameters and a player number and checks to make sure
 * that deck, discard pile, and played piles for that player are the same in both states
 *******************************************************************************************/ 

int checkPilesUnchanged(struct gameState *state1, struct gameState *state2, int player)
{
	int unchanged = 1; 	
	int i;
      
	//make sure deck count is the same for both
	if (state1->deckCount[player] != state2->deckCount[player])
	{
		unchanged = 0;
	}

	//make sure the deck is unchanged
	int cardCount1;
	int cardCount2;

	for (i = 0; i<NUM_CARDS;i++)
	{
		cardCount1 = countCardNum(i, state1->deck[player], state1->deckCount[player]);
		cardCount2 = countCardNum(i, state2->deck[player], state2->deckCount[player]);
		if (cardCount1 != cardCount2)
		{
			unchanged = 0;
		}
	}
  
	//make sure discard count is the same 
	if (state1->discardCount[player] != state2->discardCount[player])
	{
		unchanged = 0;
	}


	//make sure the discard pile is unchanged
	for (i = 0; i<NUM_CARDS;i++)
	{
		cardCount1 = countCardNum(i, state1->discard[player], state1->discardCount[player]);
		cardCount2 = countCardNum(i, state2->discard[player], state2->discardCount[player]);
		if (cardCount1 != cardCount2)
		{
			unchanged = 0;
		}
	}

    //make sure played count is the same for both: state->playedCardCount
	if (state1->playedCardCount != state2->playedCardCount)
	{
		unchanged = 0;
	}

	//make sure the played pile is unchanged
	for (i = 0; i<NUM_CARDS;i++)
	{
		cardCount1 = countCardNum(i, state1->playedCards, state1->playedCardCount);
		cardCount2 = countCardNum(i, state2->playedCards, state2->playedCardCount);
		if (cardCount1 != cardCount2)
		{
			unchanged = 0;
		}
	}

	return unchanged;
}


int main()
{
     /***************************************************************
     * Test set #1: trash flag set to 1, card is trashed 
     ***************************************************************/ 
	
		printf("\nTEST 1 for discardCard(): testing trash flag set to 1 (trash):\n");
		
		// 1a. last card is played
		
			int randomSeed = 1000;
			int players = 2;
	
			struct gameState state1a;
			int initializeResult;
			int supply1a[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

			// Minimum information to initialize game is number of players, list of supply cards, random seed, and the gameState struct
			initializeResult = initializeGame(players, supply1a, randomSeed, &state1a);

			// Make a copy of the game state
			struct gameState origState1a;
			origState1a = state1a;		

			// The position to remove the card from is the last position in player 1's hand
			int handPos = state1a.handCount[0] - 1; 

			// enum of the card removed
			int cardRemoved = state1a.hand[0][handPos];
			
			// Call discardCard(int handPos, int currentPlayer, struct gameState *state, int trashflag)
			// trashflag == 1			  
			discardCard(handPos, 0, &state1a, 1);

			// Check that card is no longer in player's hand; hand otherwise the same
			printf("discardCard: last card in hand played, test for correct hand after discard: ");
			int handCorrect;
			handCorrect = removedFromHand(&origState1a, &state1a, 0, cardRemoved);			
			asserttrue(handCorrect);	

			// Other piles are unchanged
			printf("discardCard: last card in hand played, test for no other piles altered: ");
			int pilesCorrect;
			pilesCorrect = checkPilesUnchanged(&origState1a, &state1a, 0);
			asserttrue(pilesCorrect); 

		// 1b. only one card in hand
	
			// Set up the game so that only one card is in the player's hand:
					
			randomSeed = 100;
			players = 2;
	
			struct gameState state1b;
	
			int supply1b[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

 		    	initializeResult = initializeGame(players, supply1b, randomSeed, &state1b);

			// Make a copy of the game state
			struct gameState origState1b;
			origState1b = state1b;		

			// The position to remove the card from is the first position in player 1's hand
			handPos = 0; 

			// Don't count other cards as being in the player's hand
			state1b.handCount[0] = 1;
			origState1b.handCount[0] = 1;

			// enum of the card removed
			cardRemoved = state1b.hand[0][handPos];
		
			// Call discardCard(int handPos, int currentPlayer, struct gameState *state, int trashflag)
			// trashflag == 1	  
			discardCard(handPos, 0, &state1b, 1);

			// Check that card is no longer in player's hand; hand otherwise the same
			printf("discardCard: one card in hand only, test for correct hand after discard: ");
			handCorrect = removedFromHand(&origState1b, &state1b, 0, cardRemoved);			
			asserttrue(handCorrect);	

			// Other piles are unchanged
			printf("discardCard: one card in hand only, test for no other piles altered: ");
			pilesCorrect = checkPilesUnchanged(&origState1b, &state1b, 0);
			asserttrue(pilesCorrect); 

		// 1c. more than one card in hand and not last card
			randomSeed = 1000;
			players = 2;
	
			struct gameState state1c;
			
			int supply1c[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

			initializeResult = initializeGame(players, supply1c, randomSeed, &state1c);

			// Make a copy of the game state
			struct gameState origState1c;
			origState1c = state1c;		

			// The position to remove the card from is in the middle in player 1's hand
			handPos = 2; 

			// enum of the card removed
			cardRemoved = state1c.hand[0][handPos];
			
			// Call discardCard(int handPos, int currentPlayer, struct gameState *state, int trashflag)
			// trashflag == 1
			discardCard(handPos, 0, &state1c, 1);

			// Check that card is no longer in player's hand; hand otherwise the same
			//int removedFromHand (struct gameState *stateBefore, struct gameState *stateAfter, int player, int cardRemoved)
			printf("discardCard: card played from middle of hand, test for correct hand after discard: ");
			handCorrect = removedFromHand(&origState1c, &state1c, 0, cardRemoved);			
			asserttrue(handCorrect);	

			// Other piles are unchanged
			printf("discardCard: card played from middle of hand, test for no other piles altered: ");
			pilesCorrect = checkPilesUnchanged(&origState1c, &state1c, 0);
			asserttrue(pilesCorrect); 

		// 1d. edge case: first card is played       

			randomSeed = 1000;
			players = 2;
	
			struct gameState state1d;
			
			int supply1d[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

			// Minimum information to initialize game is number of players, list of supply cards, random seed, and the gameState struct
			initializeResult = initializeGame(players, supply1d, randomSeed, &state1d);

			// Make a copy of the game state
			struct gameState origState1d;
			origState1d = state1d;		

			// The position to remove the card from is in the first card in player 1's hand
			handPos = 0; 

			// enum of the card removed
			cardRemoved = state1d.hand[0][handPos];
			
			// Call discardCard(int handPos, int currentPlayer, struct gameState *state, int trashflag)
			// trashflag == 1			  
			discardCard(handPos, 0, &state1d, 1);

			// Check that card is no longer in player's hand; hand otherwise the same
			//int removedFromHand (struct gameState *stateBefore, struct gameState *stateAfter, int player, int cardRemoved)
			printf("discardCard: first card in hand played, test for correct hand after discard: ");
			handCorrect = removedFromHand(&origState1d, &state1d, 0, cardRemoved);			
			asserttrue(handCorrect);	

			// Other piles are unchanged
			printf("discardCard: first card in hand played, test for no other piles altered: ");
			pilesCorrect = checkPilesUnchanged(&origState1d, &state1d, 0);
			asserttrue(pilesCorrect); 
						  

	/***************************************************************
     * Test set #2: trash flag set to 0, card is not trashed 
     ***************************************************************/ 
	
		printf("\nTEST 2 for discardCard(): testing trash flag set to 0 (discard):\n");
		
			// 2a. last card is played
		
			randomSeed = 100;
			players = 2;
	
			struct gameState state2a;
		
			int supply2a[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

			// Minimum information to initialize game is number of players, list of supply cards, random seed, and the gameState struct
			initializeResult = initializeGame(players, supply2a, randomSeed, &state2a);

			// Make a copy of the game state
			struct gameState origState2a;
			origState2a = state2a;		

			// The position to remove the card from is the last position in player 1's hand
			handPos = state2a.handCount[0] - 1; 

			// enum of the card removed
			cardRemoved = state2a.hand[0][handPos];
			
			// Call discardCard(int handPos, int currentPlayer, struct gameState *state, int trashflag)
			// trashflag == 0		  
			discardCard(handPos, 0, &state2a, 0);

			// Check that card is no longer in player's hand; hand otherwise the same
			printf("discardCard: last card in hand played, test for correct hand after discard: ");
			handCorrect = removedFromHand(&origState2a, &state2a, 0, cardRemoved);			
			asserttrue(handCorrect);	
	
			// Check that the card is added to the discard pile
			printf("discardCard: last card in hand played, test for added card in the discard pile: ");
			int discardAdded = checkAddToDiscard(&origState2a, &state2a, 0, cardRemoved); 
			asserttrue(discardAdded);	

		// 2b. only one card in hand

			// Set up the game so that only one card is in the player's hand:
					
			randomSeed = 100;
			players = 2;
	
			struct gameState state2b;
	
			int supply2b[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

			// Minimum information to initialize game is number of players, list of supply cards, random seed, and the gameState struct
			initializeResult = initializeGame(players, supply2b, randomSeed, &state2b);

			// Make a copy of the game state
			struct gameState origState2b;
			origState2b = state2b;		

			// The position to remove the card from is the first position in player 1's hand
			handPos = 0; 

			// Don't count other cards as being in the player's hand
			state2b.handCount[0] = 1;
			origState2b.handCount[0] = 1;

			// enum of the card removed
			cardRemoved = state2b.hand[0][handPos];
			
			// Call discardCard(int handPos, int currentPlayer, struct gameState *state, int trashflag)
			// trashflag == 0
			discardCard(handPos, 0, &state2b, 0);

			// Check that card is no longer in player's hand; hand otherwise the same
			printf("discardCard: only one card in hand, test for correct hand after discard: ");
			handCorrect = removedFromHand(&origState2b, &state2b, 0, cardRemoved);			
			asserttrue(handCorrect);				
		
			// Check that the card is added to the discard pile
			printf("discardCard: only one card in hand, test for added card in discard pile: ");
			discardAdded = checkAddToDiscard(&origState2b, &state2b, 0, cardRemoved); 
			asserttrue(discardAdded);

		// 2c. more than one card in hand and not last card
	
		    	randomSeed = 1000;
			players = 2;
	
			struct gameState state2c;
			
			int supply2c[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

			initializeResult = initializeGame(players, supply2c, randomSeed, &state2c);

			// Make a copy of the game state
			struct gameState origState2c;
			origState2c = state2c;		

			// The position to remove the card from is in the middle in player 1's hand
			handPos = 2; 

			// enum of the card removed
			cardRemoved = state2c.hand[0][handPos];
			
			// Call discardCard(int handPos, int currentPlayer, struct gameState *state, int trashflag)
			// trashflag == 0			  
			discardCard(handPos, 0, &state2c, 0);

			// Check that card is no longer in player's hand; hand otherwise the same
			printf("discardCard: card played from middle of hand, test for correct hand after discard: ");
			handCorrect = removedFromHand(&origState2c, &state2c, 0, cardRemoved);			
			asserttrue(handCorrect);	
	
			// Check that the card is added to the discard pile
			printf("discardCard: card played from middle of hand, test for added card in discard pile: ");
			discardAdded = checkAddToDiscard(&origState2c, &state2c, 0, cardRemoved); 
			asserttrue(discardAdded);

		// 2d. edge case: first card is played       

		    	randomSeed = 1000;
			players = 2;
	
			struct gameState state2d;
			
			int supply2d[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

			// Minimum information to initialize game is number of players, list of supply cards, random seed, and the gameState struct
			initializeResult = initializeGame(players, supply2d, randomSeed, &state2d);

			// Make a copy of the game state
			struct gameState origState2d;
			origState2d = state2d;		

			// The position to remove the card from is in the first card in player 1's hand
			handPos = 0; 

			// enum of the card removed
			cardRemoved = state2d.hand[0][handPos];
			
			// Call discardCard(int handPos, int currentPlayer, struct gameState *state, int trashflag)
			// trashflag == 0 
			  
			discardCard(handPos, 0, &state2d, 0);

			// Check that card is no longer in player's hand; hand otherwise the same
			//int removedFromHand (struct gameState *stateBefore, struct gameState *stateAfter, int player, int cardRemoved)
			printf("discardCard: first card in hand played, test for correct hand after discard: ");
			handCorrect = removedFromHand(&origState2d, &state2d, 0, cardRemoved);			
			asserttrue(handCorrect);	
			
			// Check that the card is added to the discard pile
			printf("discardCard: first card in hand played, test for added card in discard pile: ");
			discardAdded = checkAddToDiscard(&origState2d, &state2d, 0, cardRemoved); 
			asserttrue(discardAdded);	

	return 0;
}


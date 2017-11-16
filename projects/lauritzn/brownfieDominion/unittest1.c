/**********************************************************************************
 * File: unittest1.c
 * Author: Nina Lauritzen
 * Date: 10/18/2017
 * Description: Unit tests for the function isGameOver()
 **********************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>

/***********************************************************************************
 * Function: asserttrue
 * Description: displays a brief success message if statement evaluates to true
 *              and a failure message if statement evaluates to false 
 **********************************************************************************/
void asserttrue(int statement)
{
	if(statement)
	{
		printf("Test succeeded\n");
	}
	else
	{
		printf("Test failed!\n");
	}
	return;
}; 

int main()
{

	int randomSeed = 100;
	int players = 2;

	/************************************************************
     	*  Test #1:  Province supply == 0, no other piles at 0 
     	************************************************************/  

		struct gameState state;
		int initializeResult;
		int supply[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

		// Call the function that creates an initial game state
		initializeResult = initializeGame(players, supply, randomSeed, &state);

		// Set the province supply to 0
		state.supplyCount[province] = 0;

		// isGameOver() returns 1 if the game is over 
		int gameOver = isGameOver(&state);

		printf("isGameOver(): Test when province cards at 0: \n");
		asserttrue(gameOver);

	/***********************************************************
     	* Test #2: Test the case where there are >0 cards in the
     	* province pile and exactly 0 cards in exactly 3 piles  
     	**********************************************************/
 		struct gameState state2;
		int supply2[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

		initializeResult = initializeGame(players, supply2, randomSeed, &state2);

		// To ensure no pile is at 0 after initialization, set all piles to a positive number
		int i;	
		for (i=curse; i<=treasure_map; i++)
		{
			state2.supplyCount[i] = i+2; // will yield a number between 1 and 28
		} 
		// Set the count of 3 card piles to 0
		state2.supplyCount[embargo] = 0;
		state2.supplyCount[smithy] = 0;
		state2.supplyCount[steward] = 0;  
	
		// Call the isGameOver() function
		gameOver = isGameOver(&state2);
		
		printf("isGameOver(): Test when province cards > 0 and 3 supply cards = 0:\n");
		asserttrue(gameOver);  
	
	/************************************************************
     	* Test #3: Edge case: test the case where 2 of the 3 empty
     	* piles are at the extreme edges of the array (lowest and
     	* highest card numbers, 0 and 26)  
     	************************************************************/  

		struct gameState state3;
		int supply3[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, treasure_map};

		// Minimum information to initialize game is number of players, list of supply cards, random seed, and the gameState struct
		initializeResult = initializeGame(players, supply3, randomSeed, &state3);

		// Set the count of the "edge" cards curse and treasure_map to 0 
		state3.supplyCount[curse] = 0;
		state3.supplyCount[treasure_map] = 0;

		// Set the count of one (any) other card to 0
		state3.supplyCount[embargo] = 0;  		

		// Call the isGameOver() function
		gameOver = isGameOver(&state3);
		
		printf("isGameOver(): edge piles curse and treasure_map are 2 of 3 empty piles:\n");
		asserttrue(gameOver);  
	
	/**********************************************************
     	* Test #4: test the case where there are > 0 cards in the
     	* province pile and no supply pile has 0 cards
     	* (game not over)
     	*********************************************************/
     
		struct gameState state4;
		int supply4[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

		initializeResult = initializeGame(players, supply4, randomSeed, &state4);

		// To ensure no pile is 0, set all piles to a positive number
	
		for (i=curse; i<=treasure_map; i++)
		{
			state4.supplyCount[i] = i+2; // will yield a number between 1 and 28
		} 
		
		gameOver = isGameOver(&state4);
		
		printf("isGameOver(): test where no supply pile is at 0:\n");
		asserttrue(!gameOver);  // game should not be over
	

	/***********************************************************
     	* Test #5: Edge case: test the case where all supply 
     	*	 piles are at 0
     	**********************************************************/
  		
		struct gameState state5;
		int supply5[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

		initializeResult = initializeGame(players, supply5, randomSeed, &state5);

		// set all supply pile counts to 0
		for (i=curse; i<=treasure_map; i++)
		{
			state5.supplyCount[i] = 0; 
		}

		gameOver = isGameOver(&state5);

		printf("isGameOver: test when all supply piles are at 0:\n");
	    asserttrue(gameOver);
	
	/***********************************************************
	* Test #6: Edge case: test the case where there are >0
	* cards in the province pile and exactly 2 supply piles
     	* are at 0 (max that could be 0 when game is not over)
    	 ***********************************************************/

		struct gameState state6;
		int supply6[10] = {adventurer, council_room, embargo, gardens, sea_hag, remodel, smithy, village, steward, great_hall};

		// Minimum information to initialize game is number of players, list of supply cards, random seed, and the gameState struct
		initializeResult = initializeGame(players, supply6, randomSeed, &state6);

		// To ensure no pile is 0, set all piles to a positive number
		for (i=curse; i<=treasure_map; i++)
		{
			state6.supplyCount[i] = i+1; // will yield a number between 2 and 28
		} 
	
		// Set the count of exactly 2 card piles to 0
		state6.supplyCount[embargo] = 0;
		state6.supplyCount[smithy] = 0;
		  
		gameOver = isGameOver(&state6);
		
		printf("isGameOver(): Testing case where exactly 2 supply piles are at 0:\n");
		asserttrue(!gameOver); 

	return 0;
}


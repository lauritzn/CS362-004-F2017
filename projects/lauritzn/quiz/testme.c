#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
	/*
	 * Generate a random character
	 * There are 9 possible characters to generate
	 */

	int randNum = rand()%9;  // between 0 and 8
	char c;

	if (randNum == 0)
	{
		c = '[';
	}
	else if (randNum == 1)
	{
		c = '(';
	}
	else if (randNum == 2)
	{
		c = '{';
	}
	else if (randNum == 3)
	{
		c = ' ';
	}
	else if (randNum == 4)
	{
		c = 'a';
	}
	else if (randNum == 5)
	{
		c = 'x';
	}
	else if (randNum == 6)
	{
		c = '}';
	}
	else if (randNum == 7)
	{
		c = ')';
	}
	else
	{
		c = ']';
	}
	return c;
}

char *inputString()
{
	/*
 	 *  Generate a random string
 	 *  Up to 5 characters possible, including null terminator
 	 *  String length up to 6 (5 characters + null terminator)
 	 */   	

    	char *randString = malloc(6); // space for 5 characters and null terminator
	int i;
	int randNum;

	// Generate 6 random characters, for strings up to length 5 + '\0'
	for (i = 0; i < 6; i++)
	{
		randNum = rand()%5; // generates a number 0 - 4
		if (randNum == 0)
		{
			strcat(randString, "r");
		}
		else if (randNum == 1)
		{
			strcat(randString, "e");
		}
		else if (randNum == 2)
		{
			strcat(randString, "s");
		}
		else if (randNum == 3)
		{
			strcat(randString, "t");
		}
		else
		{
			strcat(randString, "\0");
		}	 
	}
    	return randString;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}

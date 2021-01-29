#include <stdio.h>
#include <stdlib.h>
#include <time.h> // used for srand comment.

/*
variables:
-- Numbers[16] Keeping values of the each square. 
-- Backup [16] Keeping backup values of the each square.
-- i,j and k are iterators. 
-- score storing user score, highscore is storing highest score.
-- a 1. box, b 2. box, c 3. box and, d is representing 4. box. 
-- increase tracking incrementing for each line.
-- max used to stop for loop.
-- counter used for keeping backup, counter2 tracking if the user reached the highest point.
-- star used for tracking last added number and putting star for its place.
-- control is keeping integer value of a direction.
-- no_move ve no_move2 tracking the cases of the values should not be able to change by user.
-- first_input used for first user input to start the game.
-- gameplay_input used for input in gameplay,
-- last_input used for last input in gameplay.
*/

// The app builded only in main function as requested
// Application used go to commands which should not use to in normal programs,
// The commands used with lecturer's recommendation because we were not allowed to use any fuction except main
int main()
{
	int Numbers[16], Backup[16];
	int i, j, k;
	int score, highscore;
	int a, b, c, d;
	int increase, max, counter = 0, counter2 = 0, star;
	int control = 0, no_move = 0, no_move2 = 0;
	char first_input, gameplay_input, last_input;

	// reset label clears high score.
reset:
	highscore = 0;

	// restart label clears user score but not high score.
restart:
	score = 0;

	// set all elements to zero in both arrays.
	for (i = 0; i < 16; i++)
	{
		Numbers[i] = 0;
		Backup[i] = 0;
	}

	/* the introduction and rules printed for the user.
   first input is taken from the user.
*/
	printf("     *****************************************  \n");
	printf("     *   //////  ///////     ///  ///////    *  \n");
	printf("     *       //  //   //    ////  //   //    *  \n");
	printf("     *   //////  //   //   // //  ///////    *  \n");
	printf("     *   //      //   //  /////// //   //    *  \n");
	printf("     *   //////  ///////      //  ///////    *  \n");
	printf("     ***************************************** 	\n");
	printf("                                                \n");
	printf("         ***WELLCOME to the GAME 2048***      \n\n");
	printf("               HIGHEST SCORE:	%d            \n\n", highscore);
	printf("         PLEASE PRESS A BUTTON THEN             \n");
	printf("            PRESS ENTER TO START              \n\n");
	printf("      HOW to PLAY :  			     			\n");
	printf("  In gameplay there are four directions the     \n");
	printf("player can choose:							    \n");
	printf("For down press 2 or s/S, for left press 4 or a/A\n");
	printf("for right press 6 or d/D, for up press 8 or w/W,\n");
	printf("then press Enter.								\n");
	printf("  E/e button is terminating the game, R/r is  \n");
	printf("restoring 1 previous gameboard			      \n\n");
	printf("  Press X and enter for reseting the game              \n:");
	scanf(" %c", &first_input);
	system("@cls||clear");
	if (first_input == 'X' || first_input == 'x')
	{
		goto reset;
	}

	// used srand command to generate random number
	srand(time(NULL));

	// used for returning the begining of the game.
start:

	// j setted for 1 or 0, if j is 0 new number will be 2 and if j is one new number will be 4.
	j = rand() % 2;

	// k is setted for a random cell. If the cell is 0 then new number printed that cell.
	k = rand() % 16;

	// if selected cell is empty then star value saved and new number added to that cell.
	if (Numbers[k] == 0)
	{
		star = k;
		if (j == 0)
			Numbers[k] = 2;
		else
			Numbers[k] = 4;
	}
	// If selected cell is not empty then we set new number cell the last empty cell.
	else
	{
		i = 15;
		while (Numbers[i] != 0 && i >= 0)
		{
			i--;
		}
		star = i;
		if (Numbers[i] == 0 && j == 0)
			Numbers[i] = 2;
		else if (Numbers[i] == 0 && j == 1)
			Numbers[i] = 4;
	}
	/* array elements tested in each round with a loop. if there is no possible way to move numbers the game is terminated.	
  the board tested with each possible direction with if statements. Control is made in that part of code because 
  game only over after adding a new number(or winning the game).
*/

	no_move2 = 4;
	control = 1;
	while (control < 5 && no_move2 == 4)
	{
		if (control == 1)
		{
			increase = 1;
			a = 12;
			b = 8;
			c = 4;
			d = 0;
			max = 4;
		}
		else if (control == 2)
		{
			increase = 4;
			a = 0;
			b = 1;
			c = 2;
			d = 3;
			max = 16;
		}
		else if (control == 3)
		{
			increase = 4;
			a = 3;
			b = 2;
			c = 1;
			d = 0;
			max = 16;
		}
		else if (control == 4)
		{
			increase = 1;
			a = 0;
			b = 4;
			c = 8;
			d = 12;
			max = 4;
		}
		no_move2 = 0;
		for (i = 0; i < max; i = i + increase)
		{
			if (Numbers[a + i] != 0)
			{
				if (Numbers[b + i] != 0)
				{
					if (Numbers[c + i] != 0)
					{
						if (Numbers[d + i] != 0)
						{
							if (Numbers[a + i] != Numbers[b + i] && Numbers[b + i] != Numbers[c + i] && Numbers[c + i] != Numbers[d + i])
								no_move2 = no_move2 + 1;
						}
						else
						{
							if (Numbers[a + i] != Numbers[b + i] && Numbers[b + i] != Numbers[c + i])
								no_move2 = no_move2 + 1;
						}
					}
					else
					{
						if (Numbers[d + i] == 0)
						{
							if (Numbers[a + i] != Numbers[b + i])
								no_move2 = no_move2 + 1;
						}
					}
				}
				else
				{
					if (Numbers[c + i] == 0 && Numbers[d + i] == 0)
						no_move2 = no_move2 + 1;
				}
			}
			else
			{
				if (Numbers[b + i] == 0 && Numbers[c + i] == 0 && Numbers[d + i] == 0)
					no_move2 = no_move2 + 1;
			}
		}

		control++;
	}

	/* control value is reached to 4 then there are no possible way to go in that way game is terminated.
	otherwise control and no move values resetted.
*/
	if (control > 4)
	{
		goto end;
	}
	else
	{
		control = 0;
		no_move2 = 0;
	}

	// this label moves function to print each move
print:

	// Game information is printed for the user

	printf("     *****************************************  \n");
	printf("     *   //////  ///////     ///  ///////    *  \n");
	printf("     *       //  //   //    ////  //   //    *  \n");
	printf("     *   //////  //   //   // //  ///////    *  \n");
	printf("     *   //      //   //  /////// //   //    *  \n");
	printf("     *   //////  ///////      //  ///////    *  \n");
	printf("     ***************************************** 	\n");
	printf("  Score:	%d                        High Score:	%d\n", score, highscore);
	printf("	 _________________________________\n");

	// board values printed in proper format and 0 values not printed because 0 representing an empty cell.

	// previous board is printed
	for (i = 0; i < 4; i++)
	{
		printf("         ");
		for (j = 0; j < 4; j++)
		{
			if (Backup[j + 4 * i] == 0)
				printf("|       ");
			else
				printf("| %d	 ", Backup[j + 4 * i]);
		}
		printf("|\n");
		printf("	 |_______|_______|_______|_______|\n");
	}
	printf("                    Old Board                \n ");
	printf("	 _________________________________\n");

	// current board is printed.
	for (i = 0; i < 4; i++)
	{
		printf("         ");
		for (j = 0; j < 4; j++)
		{
			if (Numbers[j + 4 * i] == 0)
				printf("|       ");
			// if user not restored to previous move star is printed for last added number in previous board.
			else if (j + 4 * i == star && counter == 0)
				printf("|*%d	 ", Numbers[star]);
			else
				printf("| %d	 ", Numbers[j + 4 * i]);
		}
		printf("|\n");
		printf("	 |_______|_______|_______|_______|\n");
	}
	printf("                    New Board                \n:");

	// if user has typed an invalid input then we get input again from the user.
entry:

	/* the user input in the game:
down = '2','S','s'
left = '4','A','A'
right = '6','D','d'
up = '8','W','w'
revert = 'R','r'
reset = 'X','x'
end Game = 'E','e'
screen cleared in reset, revert and end game.
*/
	scanf("%c", &gameplay_input);

	// direction values explained in few lines ahead.
	if (gameplay_input == '2' || gameplay_input == 'S' || gameplay_input == 's')
	{
		increase = 1;
		a = 12;
		b = 8;
		c = 4;
		d = 0;
		max = 4;
	}
	else if (gameplay_input == '4' || gameplay_input == 'A' || gameplay_input == 'a')
	{
		increase = 4;
		a = 0;
		b = 1;
		c = 2;
		d = 3;
		max = 16;
	}
	else if (gameplay_input == '6' || gameplay_input == 'D' || gameplay_input == 'd')
	{
		increase = 4;
		a = 3;
		b = 2;
		c = 1;
		d = 0;
		max = 16;
	}
	else if (gameplay_input == '8' || gameplay_input == 'W' || gameplay_input == 'w')
	{
		increase = 1;
		a = 0;
		b = 4;
		c = 8;
		d = 12;
		max = 4;
	}
	else if (gameplay_input == 'X' || gameplay_input == 'x')
	{
		system("@cls||clear");
		goto reset;
	}
	// user only revert game one time revert count tracked in counter.
	else if (gameplay_input == 'R' || gameplay_input == 'r')
	{
		system("@cls||clear");
		if (counter != 0)
		{
			printf("You can only revert 1 time.\n");
			goto print;
		}

		// if user is reverted backup values copied to array.®
		for (i = 0; i < 16; i++)
		{
			Numbers[i] = Backup[i];
		}
		counter++;
		goto print;
	}
	else if (gameplay_input == 'E' || gameplay_input == 'e')
	{
		system("@cls||clear");
		goto end;
	}

	// invalid entry returned back to get input again.
	else
	{
		goto entry;
	}

	// revert counter resetted.
	counter = 0;

	// board copied to backup board.
	for (i = 0; i < 16; i++)
	{
		Backup[i] = Numbers[i];
	}
	/* we need to evaluate each row and column for a a matched value or a gap. i tested each column with a for
loop for each direction we need to change values for each cell and next cell because each cell has different
neighbour in that perspective sı for up and down we need to change row order like that.
		0.  -->12.
		4.  -->8.
		8.  -->4.
		12. -->0.
for right and left cells increasing one by one so i need to user 0.1.2.3 and 3.2.1.0 values for that.
for top and bottom increase values are 4 and for left and right increase value is 1.
and i set max value to stop increasing eventually final values are:
	** up values set to a=0, b=4, c=8, d=12 increse is 1, max value is 4;
	** down values set to a=12, b=8, c=4, d=0 increse is 1, max value is 4; 
	** leftvalues set to a=0, b=1, c=2, d=3 increse is 4, max value is 16; 
	** right values set to a=3, b=2, c=1, d=0 increse is 4, max value is 16; 

score keeps tracking the score and no_move keeps tracking is it possible to move any of the cell in that column
*/
	for (i = 0; i < max; i = i + increase)
	{
		if (Numbers[a + i] != 0)
		{
			if (Numbers[b + i] != 0)
			{
				if (Numbers[c + i] != 0)
				{
					if (Numbers[a + i] == Numbers[b + i] && Numbers[c + i] == Numbers[d + i])
					{
						Numbers[a + i] = Numbers[a + i] + Numbers[b + i];
						score = score + Numbers[a + i];
						Numbers[b + i] = Numbers[c + i] + Numbers[d + i];
						score = score + Numbers[b + i];
						Numbers[c + i] = 0;
						Numbers[d + i] = 0;
					}
					else if (Numbers[a + i] == Numbers[b + i])
					{
						Numbers[a + i] = Numbers[a + i] + Numbers[b + i];
						score = score + Numbers[a + i];
						Numbers[b + i] = Numbers[c + i];
						Numbers[c + i] = Numbers[d + i];
						Numbers[d + i] = 0;
					}
					else if (Numbers[b + i] == Numbers[c + i])
					{
						Numbers[b + i] = Numbers[b + i] + Numbers[c + i];
						Numbers[c + i] = 0;
						score = score + Numbers[b + i];
						Numbers[c + i] = Numbers[d + i];
						Numbers[d + i] = 0;
					}
					else if (Numbers[c + i] == Numbers[d + i])
					{
						Numbers[c + i] = Numbers[c + i] + Numbers[d + i];
						score = score + Numbers[c + i];
						Numbers[d + i] = 0;
					}
					else
						no_move = no_move + 1;
				}
				else
				{
					if (Numbers[a + i] == Numbers[b + i])
					{
						Numbers[a + i] = Numbers[a + i] + Numbers[b + i];
						score = score + Numbers[a + i];
						Numbers[b + i] = Numbers[d + i];
						Numbers[d + i] = 0;
					}
					else if (Numbers[b + i] == Numbers[d + i])
					{
						Numbers[b + i] = Numbers[b + i] + Numbers[d + i];
						score = score + Numbers[b + i];
						Numbers[d + i] = 0;
					}
					else if (Numbers[d + i] != 0)
					{
						Numbers[c + i] = Numbers[d + i];
						Numbers[d + i] = 0;
					}
					else
						no_move = no_move + 1;
				}
			}
			else
			{
				if (Numbers[c + i] != 0)
				{
					if (Numbers[a + i] == Numbers[c + i])
					{
						Numbers[a + i] = Numbers[a + i] + Numbers[c + i];
						score = score + Numbers[a + i];
						Numbers[b + i] = Numbers[d + i];
						Numbers[c + i] = 0;
						Numbers[d + i] = 0;
					}
					else if (Numbers[c + i] == Numbers[d + i])
					{
						Numbers[b + i] = Numbers[c + i] + Numbers[d + i];
						score = score + Numbers[b + i];
						Numbers[c + i] = 0;
						Numbers[d + i] = 0;
					}
					else
					{
						Numbers[b + i] = Numbers[c + i];
						Numbers[c + i] = Numbers[d + i];
						Numbers[d + i] = 0;
					}
				}
				else
				{
					if (Numbers[a + i] == Numbers[d + i])
					{
						Numbers[a + i] = Numbers[a + i] + Numbers[d + i];
						score = score + Numbers[a + i];
						Numbers[d + i] = 0;
					}
					else if (Numbers[d + i] != 0)
					{
						Numbers[b + i] = Numbers[d + i];
						Numbers[d + i] = 0;
					}
					else
						no_move = no_move + 1;
				}
			}
		}
		else
		{
			if (Numbers[b + i] != 0)
			{
				if (Numbers[c + i] != 0)
				{
					if (Numbers[b + i] == Numbers[c + i])
					{
						Numbers[a + i] = Numbers[b + i] + Numbers[c + i];
						score = score + Numbers[a + i];
						Numbers[b + i] = Numbers[d + i];
						Numbers[c + i] = 0;
						Numbers[d + i] = 0;
					}
					else if (Numbers[c + i] == Numbers[d + i])
					{
						Numbers[a + i] = Numbers[b + i];
						Numbers[b + i] = Numbers[c + i] + Numbers[d + i];
						score = score + Numbers[b + i];
						Numbers[c + i] = 0;
						Numbers[d + i] = 0;
					}
					else
					{
						Numbers[a + i] = Numbers[b + i];
						Numbers[b + i] = Numbers[c + i];
						Numbers[c + i] = Numbers[d + i];
						Numbers[d + i] = 0;
					}
				}
				else
				{
					if (Numbers[b + i] == Numbers[d + i])
					{
						Numbers[a + i] = Numbers[b + i] + Numbers[d + i];
						score = score + Numbers[a + i];
						Numbers[b + i] = 0;
						Numbers[d + i] = 0;
					}
					else
					{
						Numbers[a + i] = Numbers[b + i];
						Numbers[b + i] = Numbers[d + i];
						Numbers[d + i] = 0;
					}
				}
			}
			else
			{
				if (Numbers[c + i] != 0)
				{
					if (Numbers[c + i] == Numbers[d + i])
					{
						Numbers[a + i] = Numbers[c + i] + Numbers[d + i];
						score = score + Numbers[a + i];
						Numbers[c + i] = 0;
						Numbers[d + i] = 0;
					}
					else
					{
						Numbers[a + i] = Numbers[c + i];
						Numbers[b + i] = Numbers[d + i];
						Numbers[c + i] = 0;
						Numbers[d + i] = 0;
					}
				}
				else
				{
					if (Numbers[d + i] != 0)
					{
						Numbers[a + i] = Numbers[d + i];
						Numbers[d + i] = 0;
					}
					else
						no_move = no_move + 1;
				}
			}
		}
	}

	// if no move is 4 then the board cant move that direction we get a new input from the user.
	if (no_move == 4)
	{
		no_move = 0;
		goto entry;
	}
	else
		no_move = 0;

	// if user score is higher than high score highscore is updated, counter 2 is saving the user is reached highscore.
	if (score > highscore)
	{
		counter2 = 1;
		highscore = score;
	}

	// if any value of the board is reached 2048 then game is completed.
	i = 0;
	while (Numbers[i] != 2048 && i < 16)
		i++;
	if (Numbers[i] == 2048)
	{
		system("@cls||clear");
		goto end;
	}

	// if there is no reason to finish the game then we jumped start of the application.
	system("@cls||clear");
	goto start;

	// end point is created for the end of the game
end:

	// end game output is printed.
	printf("     *****************************************  \n");
	printf("     *   //////  ///////     ///  ///////    *  \n");
	printf("     *       //  //   //    ////  //   //    *  \n");
	printf("     *   //////  //   //   // //  ///////    *  \n");
	printf("     *   //      //   //  /////// //   //    *  \n");
	printf("     *   //////  ///////      //  ///////    *  \n");
	printf("     ***************************************** 	\n");
	printf("                                                \n");

	// if user passed the high score then info printed.
	if (counter2 == 1)
	{
		printf("*** Congratulations! You got the highest score: %d* ** \n", score);
		printf("                                                \n");
	}
	counter2 = 0;

	// if user reached 2048 then info printed.
	for (i = 0; i < 16; i++)
	{
		if (Numbers[i] == 2048)
			printf(" *** Congratulations! You won the game***    \n");
	}

	// The score values and informations are printed.
	printf("                                                \n");
	printf("                  Your Score: %d	            \n", score);
	printf("                                                \n");
	printf("               Highest Score: %d                \n", highscore);
	printf("                                                \n");
	printf("            PRESS E TO PLAY AGAIN               \n");
	printf("                                                \n");
	printf("        PRESS X FOR RESETTING THE GAME          \n");
	printf("                                                \n");
	printf("             PRESS Q FOR QUIT                  \n:");
again:
	scanf(" %c", &last_input);
	if (last_input == 'E' || last_input == 'e')
	{
		system("@cls||clear");
		goto restart;
	}
	else if (last_input == 'X' || last_input == 'x')
	{
		system("@cls||clear");
		goto reset;
	}
	else if (last_input == 'Q' || last_input == 'q')
	{
		return 1;
	}
	// if input is invalid program returned to again for getting another input from the user.
	else
		goto again;
	return 0;
}

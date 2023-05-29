/* MASTERMIND GAME PLAY LOOP OVERVIEW:
	1. Validate Command line arguments and options (sends args to launch_check)
	2. Sets randomized secret code answer
	3. Send parsed flags and flag options to flag validator / manual option setter
	4. Print initial game prompt (from main())
	5. Run user_input function to take guesses from keyboard input
	6. Validate inputs guesses / check for win condition
	7. Print response (turn count and correct/incorrect placement of guessed digits)
	8. Program Exit: if win condition is met, or EOF is received from user_input
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "mmheader.h"
#include "mmglobals.h"

/* Main performs three (3) tasks:
	1. Launch_check to validate / set command line options / display usage manual using --help command.
	2. Displays initial game prompt text.
	3. Begins user_input and guess validation modules
*/
int main(int argc, char *argv[])
{
	// Sends command line arguments to validator and option setter before launching the game.
	int load_ok = launch_check(argc, argv);
	if (load_ok == -1)
	{
		return -1;
	} 
	// When launch check receives the "--help" flag it prints the usage manual
	else if (load_ok == 1) 
	{
		return 0;
	}
	
	print_banner();

	// Initial game prompt: display only once, at successful launch
	printSlowly("\n\nPlease enter a valid code\n---\nRound 0\n", 50);
	
	// Main game loop
	user_input();  

	return 0;
}
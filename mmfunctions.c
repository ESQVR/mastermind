#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "mmheader.h"
#include "mmglobals.h"

#define ANSI_COLOR_GREEN "\x1b[92m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_TEAL "\x1b[36m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_ORANGE  "\x1b[38;5;208m"

/* Validates the number, syntax, and content of command line arguments (program options):
	1. If number of command line arguments is valid (1, 3, or 5):
	2. Sets secret answer to "random" using set_code() with parameter '9999'
	2. Sends pairs of flags (strings) and options (integer values) to the flag validator to set program options.
*/
int launch_check(int argc, char *argv[])
{
	/* When receiving the "--help" command line flag: print the manual and exit the program.
		Flags here terminate the program early, and are not handled by set_options.
	*/
	if (argc == 2 && strcmp(argv[1], "--help") == 0) {
		print_manual();
        return 1;
    }

	// Valid option flags (except "--help") will send odd numbers of arguments (program + flag + option ... flag + option)
	if (argc == 1 || argc == 3 || argc == 5)
	{
		// Sets secret code to "random" (default) if 0 or more flags sent.
		set_code(9999); 

		// Sets first flag when 1 or 2 flags sent.
		if (argc == 3 || argc == 5) { 
			int entry = atoi(argv[2]);
			int options = flag_check(argv[1], entry);

			if (options == -1) { 
				print_error();
				return -1;
			}
		}
		// Sets second flag if 2 flags sent.
		if (argc == 5) { 
			int entry = atoi(argv[4]);
			int options = flag_check(argv[3], entry);
			
			if (options == -1) {
				print_error();
				return -1;
			}
		}
		return 0;

	// If invalid # of arguments sent - prints error msg & flag manual / returns code to terminate program.
	} else { 
	 print_error();
		return -1;
	}

}

/* Validates pairs of command line flags and sets the corresponding override options:
	1. Create string constants and code numbers (for passing flag content to option setter) for each flag used.
	2. Check each (pointer-passed string) command line argument against the list of flags (if-else).
	3. Validate each subsequent option (value passed integer).
	4. IF any argument passed by command line does not pass syntax validation, returns error code to launch_check (terminates program).
*/
int flag_check(char *flag, int override)
{
	/* Flag list for specifying command line options
		1. Each flag is assigned a:
			a. constant defining the flag.
			b. number to send to set_flag_options which processes the validated override option.
		2. Flag codes are read by set_flag_options, these need to be parallelized to function correctly
	*/ 
	const char code_flag[] = "-c";
	int c_flag = 1; 
	const char attempt_flag[] = "-t";
	int t_flag = 2;

	// Looks for -c [CODE] flag, validates manual override.
	if (strcmp(flag, code_flag) == 0) { 

		if (code_validator(override) == -1) { // Validates override code is formatted correctly (4 digits each !> 8).
			return -1;
		} else {
			set_flag_options(c_flag, override); // Sends flag code and validated override option content.
		}
		return 0;

	// Looks for -t [ATTEMPTS] flag, validates manual override.
	} else if (strcmp(flag, attempt_flag) == 0) { 
		
		if (override > 0 && override < 100) { // Validates min/maximum attempts allowed by override option, returns error (-1) if out of range
			set_flag_options(t_flag, override);
		} else {
			return -1;
		}
		return 0;

	// If a flag is passed that is not on the list - returns error (-1).
	} else { 
		return -1;
	}
	return 0;
}

/* Validates guesses received from user input:
	1. Splits digits from input buffer into a "guess" array (passed pointer from user_input).
		a. Converts numeral characters into integer values using ASCII arithmetic.
	2. Validates that each value in the 'guess' array is an integer or prints error msg and returns to user_input.
	3. Checks IF each value in the 'guess' array is identical to each value in the secret code array.
		a. IF each of guess = each of secret code: perform win condition function and leave validator
		b. IF secret code NOT guessed: 
			i. Calculate correct and misplaced elements.
			ii. Preform 'response' function, printing results.
			iii. Increment current turn #.
			iv. Return to user_input.
*/
void guess_validator(char *input)
{
	// Initialize guess array and variables for well placed and misplaced pieces.
	int guess[4] = {0};
	int well_p = 0;
	int mis_p = 0;

	// Guess array is filled with user input. ASCII arithmetic converts numeral character to integer value.
	int i = 0;
	while (i < 4)
	{	
		guess[i] = input[i] - '0'; 
		i++;
	}

	// Validates integer is 0-8.
	for (int i = 0; i < 4; i++) { 
		if (guess[i] < 0 || guess[i] > 8){ 
			printf("Wrong Input!\n");
			return;
		}
	}

	// Checks guess array for exact winning numbers.
	if (guess[0] == code[0] && guess[1] == code[1] && guess[2] == code[2] && guess[3] == code[3]) { 
		win_condition();
		return;
	} else {
		/*	This is the main response calculator (The primary game mechanic):
			1. Directly compare digits in guess to the corresponding digits in secret code looking for exact matches ("Well placed")
			2. IF there is no direct match:
				a. Count the number of occurrences of a digit in both code and guess.
				b. Count the quantity of each digits that occurs in both, and increment the number of misplaced pieces.
			3. Send totals of exact matches and digits that are not exact but do appear in both to response printer.
			4. Reset counts of well placed and misplaced back to 0 for the next round.
		*/

		// Count exact matches (well-placed pieces).
		for (int i = 0; i < 4; i++)
		{
			if (guess[i] == code[i])
			{
				well_p++;
			}
		}

		// Build tables of all digits.
		int code_counts[10] = {0};
		int guess_counts[10] = {0};

		// Loop through each digit in code and guess, looking for non-matches.
		for (int i = 0; i < 4; i++)
		{
			if (guess[i] != code[i])
			{
				code_counts[code[i]]++; // Index of table at the "name" of the digit (0-8) is incremented.
				guess_counts[guess[i]]++;
			}
		}

		// Increments misplaced by lesser of quantity of a digit in either (1 for both 0 for either/or)
		for (int i = 0; i < 10; i++)
		{
			mis_p += fmin(code_counts[i], guess_counts[i]); 
		}

		// Send exact and misplaced correct digit counts to response printer.
		print_response(well_p, mis_p); 
		well_p = 0;
		mis_p = 0;
		return;
	}
}

/* Sets global variable: code (the secret answer):
	1. If passed '9999' (selected because digit 9 is disallowed) sets random values to each digit.
		a. sets each element in 'code' variable to a pseudo-random number 0-8.
	2. Otherwise sets each element to value of each digit in validated code value from set_flag_options.
*/
void set_code(int override)
{
	int num;
	int num_max = 8;

	// 9999 is special code for random sent by start of the launch_check process
	if (override == 9999) { 
		int i = 0;
		srand(time(NULL));
		for (i = 0; i < 4; i++)
		{
			num = rand() % (num_max + 1);
			code[i] = num;
		}
	
	// Uses division / modulus arithmetic to set each digit of the secret code override.
	} else {
		code[0] = override / 1000;
		code[1] = (override / 100) % 10;
		code[2] = (override / 10) % 10;
		code[3] = override % 10;
	}
}

// Validates that user selected code from command line flag is 4 digits && no digit is larger than 8.
int code_validator(int number)
{
	// Launch_check sends '9999' if no code override flag is set.
	if (number > 9999)
	{
		printf("***ERROR***\nThe code must be 4 digits.\n");
		return -1;
	}

	// Uses division to validate each digit sent by code override option (0-8).
	else if ((number / 1000) == 9 || ((number / 100) % 10) == 9 || ((number / 10) % 10) == 9 || (number % 10) == 9)
	{ 
		printf("***ERROR***\nNo digit may be greater than 8\n");
		return -1;
	}

	return 0;
}

/* Primary game play loop - accepts user input (guesses) with read() from keyboard input:
	1. Makes a 5 element buffer (guesses need 4 digits) to store user guess and a pointer to the buffer to pass to guess_validator.
	3. Loops sending guesses to the guess_validator until user wins, or use ctrl+D to exit.
*/
void user_input()
{
	char key_buff[5];
	int bytes_read;
	char *game_input = key_buff;
	
	// Reads from input until read() receives 0 bytes (or Ctrl+D for program exit) OR until win condition is met.
	while ((bytes_read = read(0, key_buff, sizeof(key_buff))) > 0 && win != 1) { 
		guess_validator(game_input);
		if (win == 1){
			break;
		}
	}

	 // Ensures program leaves user_input (and terminates) after the win condition is met.
	if (bytes_read == 0) {
		printf(ANSI_COLOR_RESET);
		return;
	}
}

// IF guess_validator finds user input is an exact match for the secret code - prints "win" msg and sets win variable to 1.
void win_condition()
{

	printf(ANSI_COLOR_GREEN);
	printf("\nCODE:%d%d%d%d", code[0], code[1], code[2], code[3]);
	printSlowly(" ... CORRECT\n", 100);
	printf("\n************************************\n");
	printf(ANSI_COLOR_RED);
	printSlowly("!!! YOU ARE THE MASTERMIND !!! ", 50);
	printf(ANSI_COLOR_GREEN);
	printf("\n************************************\n");
	printf(ANSI_COLOR_RESET);
	win = 1;
}

// Receives validated flag codes and values from flag check list and sets global variables to these validated user overrides.
int set_flag_options(int flag, int options)
{
	if (flag == 1)
	{
		set_code(options);
	}

	if (flag == 2)
	{
		attempts = options;
	}

	return 0;
}

// Prints instructions for command line launch when encountering an error validating flags or their options.
void print_error()
{
	printf("\n***ERROR***\nInvalid command format.\n\n--------\nREQUIRED FORMAT:\n./my_mastermind [FLAG] [VALUE] [FLAG] [VALUE]\n\nPERMITTED FLAGS:\nSet Code:\n\t-c [CODE] (4 digits. 0-8)\nSet Attempts:\n\t-t [ATTEMPTS] (0-99)\n--------\n\nPROGRAM TERMINATED\n");
}

// Increments turn then prints response for each round of the game showing matching or correct, but misplaced, digits in user guesses.
void print_response(int well, int mis)
{
	turn++;
	if (attempts < turn){
		you_lose();
		exit(0);
	}
	printf(ANSI_COLOR_TEAL);
	printf("Correctly Placed Digits: %d\n", well);
	printf(ANSI_COLOR_MAGENTA);
	printf("Misplaced Digits: %d", mis);
	printf(ANSI_COLOR_RESET);
	printf("\n------------\n");
	if (attempts - turn > 4){
		printf(ANSI_COLOR_GREEN);
	} else if (attempts - turn > 1 && attempts - turn <= 4){
		printf(ANSI_COLOR_YELLOW);
	} else {
		printf(ANSI_COLOR_RED);
	}
	printf("Round: %d\n", turn);
}

// Prints command line options manual if launch_check receives exactly two (2) arguments and the second is "--help"
void print_manual()
{
	printf(ANSI_COLOR_RESET);
	printf("\nUsage: ./my_mastermind [options]\n");
	printf("Options:\n");
    printf("\t--help\t\tDisplay this help message\n");
    printf("\t-c [CODE]\tManually select secret code answer - (4 digits 0-8: 1234)\n");
	printf("\t-t [ATTEMPTS]\tManually select maximum number of guess attempts - (number 0-99)\n\n");
}

void printSlowly(const char *str, unsigned int delay) 
{
    while (*str) {
        putchar(*str++);
        fflush(stdout);
        usleep(delay * 1000);
    }
}

// Clears screen before play
void clearScreen() {
    #ifdef _WIN32
        system("cls"); // Windows
    #else
        system("clear"); // macOS and Linux
    #endif
}

// ASCII graphic banner
void print_banner()
{
	clearScreen();
	int delay = 20;
	printf(ANSI_COLOR_GREEN);
    printf("\n\n");
	printf("███    ███  █████  ███████ ████████ ███████ ██████  ███    ███ ██ ███    ██ ██████  \n");
    printf("████  ████ ██   ██ ██         ██    ██      ██   ██ ████  ████ ██ ████   ██ ██   ██ \n");
    printf("██ ████ ██ ███████ ███████    ██    █████   ██████  ██ ████ ██ ██ ██ ██  ██ ██   ██ \n");
    printf("██  ██  ██ ██   ██      ██    ██    ██      ██   ██ ██  ██  ██ ██ ██  ██ ██ ██   ██ \n");
    printf("██      ██ ██   ██ ███████    ██    ███████ ██   ██ ██      ██ ██ ██   ████ ██████  \n");
    printf("                                                                           By ESQVR \n");
    printf("                                                                                    \n");

    printSlowly("The computer has just chosen a secret code made of 4 digits between 0-8.\n", delay);
    printSlowly("You need to guess the code before the computer permanently locks down - but you have a limited number of tries.\n", delay);
    printSlowly("If you don't enter the correct code within", delay);
	printf(" %d", attempts);
	printSlowly(" guesses, you lose.\n\n", delay);
    printSlowly("For each guess the computer will display the number of digits that are in the correct location in the code.\n", delay);
    printSlowly("The number of correct but misplaced digits will be displayed below that.", delay);

}

void you_lose()
{
	printf(ANSI_COLOR_RESET);
	printSlowly("\n\n... ERROR: TOO MANY ATTEMPTS ...\n\n", 60);
	printf(ANSI_COLOR_RED);
	printSlowly("L     OOO   CCC  K  K  DDD    OOO   W     W  N   N \n", 10);
    printSlowly("L    O   O C     K K   D  D  O   O  W     W  NN  N \n", 10);
    printSlowly("L    O   O C     KK    D  D  O   O  W  W  W  N N N \n", 10);
    printSlowly("L    O   O C     K K   D  D  O   O   W W W   N  NN \n", 10);
    printSlowly("LLLL  OOO   CCC  K  K  DDD    OOO     W W    N   N \n\n\n", 10);
	printf(ANSI_COLOR_RESET);
}
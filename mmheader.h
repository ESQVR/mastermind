#ifndef MMHEADER_H
#define MMHEADER_H

int launch_check(int argc, char *argv[]);		// Verifies program is launched correctly from command line
int flag_check(char *flag, int value);			// Verifies command line options are valid
int code_validator(int number);					// Verifies user override for secret code is valid
int set_flag_options(int flag, int options);	// Sets user overrides to game options
void set_code(int override);					// Generates secret code (or sets to override)
void user_input();								// Takes input from keyboard and sends it to validator
void guess_validator(char *input);				// Validates user input (guesses): Along with user_input, constitutes main game play loop
void print_response(int well, int mis);			// Prints results of guesses between each game round
void win_condition();							// Prints win message
void print_error();								// Prints invalid flags error msg
void print_manual();							// Prints manual when "--help" is passed from command line

#endif
#include "mmglobals.h"

int attempts = 10;		//Maximum allowed attempts to guess the secret code
int code[4] = {0};		//Secret Code: this is the 4 digit number player must guess
int turn = 0;			//Current round of the game (current attempt #)
int win = 0;			//Win Condition: 0 = false, 1 = true
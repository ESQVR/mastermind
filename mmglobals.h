#ifndef MMGLOBALS_H
#define MMGLOBALS_H

extern int attempts;		//Maximum allowed attempts to guess the secret code
extern int code[4];			//Secret Code: this is the 4 digit number player must guess
extern int turn;			//Current round of the game (current attempt #)
extern int win;				//Win Condition: 0 = false, 1 = true

#endif
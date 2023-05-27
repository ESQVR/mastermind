# My Mastermind
A game where you are asked to guess a 4 digit secret code, where each digit is 0-8.

After each guess, the player is told how many digits in their guess is correct (AKA: Well Placed Digits)
	and how many are correct digits but in the incorrect position (AKA: Misplaced Digits).

Guessing the correct number sequence will result in a "Win"!

## Description
A command line program written in C for playing a version of the game "Mastermind."


## File Directory

Makefile:			Makefile for installing program (see Installation below).
mmfunctions.c:		All function definitions.
mmglobals.c/.h:		Global variables (see inline comments for description) and global variables header.
mmheader.h:			Header file for custom functions.
my_mastermind.c:	Main Program.

## Installation
This project includes a Makefile with the following targets:

	1 - make : Builds the project files incl. executable: my_mastermind
	2 - clean : removes .o files
	3 - fclean : removes .o + executables
	3 - re : fclean + make 

## Usage
Run program: ./my_mastermind

There are three available flags that can be run from the command line at launch: 
1. --help: 	Print this usage manual.
2. -c: [CODE]		Manually set the secret code (4 digits: 0-8).
3. -t: [ATTEMPTS]	Manually set the maximum number of guesses (0-99).

COMMAND LINE FLAG FORMATS:
"Help" can only be used by itself, the two other options may be used together, or separately.

./my_mastermind --help
./my_mastermind [FLAG] [VALUE] ... [FLAG] [VALUE]

The program can be exited at any time using Ctrl + D

## Issues / Future updates
Currently the game allows users to take infinite turns / guesses. No "out of guesses / turns" losing condition exists.
This will be updated soon.

### The Core Team
Christopher A. Deetz

<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px'></span>

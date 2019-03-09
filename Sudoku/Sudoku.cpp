// Sudoku.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SudokuBoard.h"

const char * hardcodedBoard = "....5..3..9...315......2..4.1...4.757.2..5.9..3....2...5....92732.....6.6.9.....1";

// used for testing 
int main()
{
	SudokuBoard board(hardcodedBoard);
	return 0;
}


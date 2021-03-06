// Sudoku.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SudokuBoard.h"
#include <iostream>

// uses a hardcoded board to do things
const char * hardcodedBoard = "....5..3..9...315......2..4.1...4.757.2..5.9..3....2...5....92732.....6.6.9.....1";

// used for testing 
int main()
{
	SudokuBoard board(hardcodedBoard);
	std::cout << board << std::endl;
	board.solve();
	std::cout << board << std::endl;
	return 0;
}


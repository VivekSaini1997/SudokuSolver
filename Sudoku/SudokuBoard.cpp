#include "stdafx.h"

#include "SudokuBoard.h"
#include <cstring>

// the board is empty so set everything to true
SudokuBoard::SudokuBoard()
{
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			rowValidityChecker[i][j] = colValidityChecker[i][j] = boxValidityChecker[i][j] = true;
			board[i][j] = 0;
		}
	}
}

// parses sudoku board assuming qqwing format
SudokuBoard::SudokuBoard(const char * str)
{
	SudokuBoard();
	// go through the string and construct the board from it
	// if a character in the string is a dot, the board is empty there
	// assumes 81 characters in string
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (str[i * 9 + j] == '.') {
				board[i][j] = 0;
			} else {
				board[i][j] = str[i * 9 + j] - '0';
			}
		}
	}
}

SudokuBoard::~SudokuBoard()
{
}

// check that an insertion given box is valid
// boxes are indexed as follows:
/* 
   -------------
   | 0 | 1 | 2 |
   -------------
   | 3 | 4 | 5 |
   -------------
   | 6 | 7 | 8 |
   -------------
 */
inline const bool& SudokuBoard::checkInsertIntoBox(unsigned box, unsigned char val) {
	return boxValidityChecker[box][val - 1];
}

// check that an insertion given row is valid
inline const bool& SudokuBoard::checkInsertIntoRow(unsigned row, unsigned char val) {
	return boxValidityChecker[row][val - 1];
}

// check that an insertion given col is valid
inline const bool& SudokuBoard::checkInsertIntoColumn(unsigned col, unsigned char val) {
	return boxValidityChecker[col][val - 1];
}

// do the checks above and then insert
bool SudokuBoard::insert(unsigned row, unsigned col, unsigned char val) {
	// gets the box from the row and column
	unsigned box = (row / 3) * 3 + (col / 3);
	// if the entry violates either the row, the column, or the box invariant, return false
	if (!(checkInsertIntoRow(row, val) && checkInsertIntoColumn(col, val) && checkInsertIntoBox(box, val))) {
		return false;
	// otherwise insert and update tables as neccessary
	} else {
		board[row][col] = val;
		boxValidityChecker[box][val - 1] = rowValidityChecker[row][val - 1] = colValidityChecker[col][val - 1] = false;
		return true;
	}
}

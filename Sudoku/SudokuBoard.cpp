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
	emptySpaces = 81;
}

// parses sudoku board assuming qqwing format
SudokuBoard::SudokuBoard(const char * str) : SudokuBoard()
{
	// go through the string and construct the board from it
	// if a character in the string is a dot, the board is empty there
	// assumes 81 characters in string
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (str[i * 9 + j] == '.') {
				board[i][j] = 0;
			} else {
				insert(i, j, str[i * 9 + j] - '0');
			}
		}
	}
}

// do nothing
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

// the above but it takes a row and col as arguments and deduces the box
inline const bool& SudokuBoard::checkInsertIntoBox(unsigned row, unsigned col, unsigned char val) {
	unsigned box = (row / 3) * 3 + (col / 3);
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

// sets a cell to be invalid
// used after insertion
inline void SudokuBoard::setValidity(unsigned row, unsigned col, unsigned box, unsigned val) {
	boxValidityChecker[box][val - 1] = rowValidityChecker[row][val - 1] = colValidityChecker[col][val - 1] = false;
}

// returns a vector of valid options to enter into a given cell
std::vector<unsigned char> SudokuBoard::getValidValsForCell(unsigned row, unsigned col) {
	unsigned box = (row / 3) * 3 + (col / 3);
	// first get the correct array for the row, col, and box
	bool * cellValidityRow = rowValidityChecker[row];
	bool * cellValidityCol = colValidityChecker[col];
	bool * cellValidityBox = boxValidityChecker[box];
	// now go thorugh and construct the vector from the elements that are valid in the row, col, and box
	std::vector<unsigned char> cellValidValues;
	for (unsigned char i = 0; i < 9; i++) {
		if (cellValidityBox[i] && cellValidityCol[i] && cellValidityRow[i]) {
			cellValidValues.push_back(i);
		}
	}
	return cellValidValues;
}

// sets a cell to be valid
// used before removal of a cell from the board
inline void SudokuBoard::unsetValidity(unsigned row, unsigned col) {
	unsigned box = (row / 3) * 3 + (col / 3);
	unsigned val = board[row][col];
	boxValidityChecker[box][val - 1] = rowValidityChecker[row][val - 1] = colValidityChecker[col][val - 1] = true;
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
		setValidity(row, col, box, val);
		return true;
	}
	// we have one less empty space
	emptySpaces--;
}

// remove a value from the cell from a given row and col
void SudokuBoard::remove(unsigned row, unsigned col) {
	// unsets the validity tables
	unsetValidity(row, col);
	// change the cell state back to empty (denoted by a 0)
	board[row][col] = 0;
	// we have one more empty space
	emptySpaces++;
}

// actually solve the game
// i.e. what this program was made for
void SudokuBoard::solve() {

}

// overloads the << operator so you can pipe the class to a stream
// and print in a fancy way
std::ostream& operator<< (std::ostream& os, const SudokuBoard& sb) {
	for (int i = 0; i < 9; i++) {
		if (i == 3 || i == 6) {
			os << "-------|-------|-------" << std::endl;
		}
		for (int j = 0; j < 9; j++) {
			if (j == 3 || j == 6) {
				os << " |";
			}
			if (sb.board[i][j] == 0) {
				os << " .";
			} else {
				os << " " << +sb.board[i][j];
			}
		}
		os << std::endl;
	}
	return os;
}
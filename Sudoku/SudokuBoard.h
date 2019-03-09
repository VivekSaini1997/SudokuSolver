#pragma once
#include <string>
#include <stack>
#include <vector>
#include <iostream>

// a struct to represent a move
// consists of a row and col for the location
// and the value in that location
struct Move {
	unsigned char row;
	unsigned char col;
	unsigned char val;
};

// this is the class that holds the Sudoku Board
// should also have methods to check for validity
class SudokuBoard
{
public:
	SudokuBoard();
	~SudokuBoard();
	// parses the board assuming the string is generated from QQWing Sudoku
	SudokuBoard(const char * board);

	// check that a given row/column/box is valid
	// probably not all that useful
	bool checkRow(unsigned row);
	bool checkColumn(unsigned col);
	bool checkBox(unsigned box);

	// checks that inserting into a given row/col/box is valid
	inline const bool& checkInsertIntoRow(unsigned row, unsigned char val);
	inline const bool& checkInsertIntoColumn(unsigned col, unsigned char val);
	inline const bool& checkInsertIntoBox(unsigned box, unsigned char val);
	inline const bool& checkInsertIntoBox(unsigned row, unsigned col, unsigned char val);

	// returns all of the valid values that can be entered in a cell without
	// disrupting the row, column, and box invariants in sudoku
	std::vector<unsigned char> getValidValsForCell(unsigned row, unsigned col);

	// sets and unsets the validity of a cell
	inline void setValidity(unsigned row, unsigned col, unsigned box, unsigned val);
	inline void unsetValidity(unsigned row, unsigned col);

	// insert into the cell after doing the checks
	bool insert(unsigned row, unsigned col, unsigned char val);

	// remove a value from a cell und update the validity tables
	void remove(unsigned row, unsigned col);

	// overload the << operator to print
	friend std::ostream& operator<< (std::ostream& os, const SudokuBoard& sb);

	// solves the sudoku puzzle
	void solve();
private:
	// store the board
	unsigned char board[9][9];
	// not using this right now, want to get the logic working first
	unsigned char boardTransposed[9][9];
	// used to check if a given row/col/box is valid for insertion
	bool rowValidityChecker[9][9];
	bool colValidityChecker[9][9];
	bool boxValidityChecker[9][9];
	// stacks to keep track of moves that have been made and moves that are yet to be made
	std::stack<Move> movePossiblities;
	std::stack<Move> moveTrace;
	// keeps track of how many moves remain
	short emptySpaces;
};




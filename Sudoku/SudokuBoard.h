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
	// boolean to specify that this value was in the sudoku board to begin with
	// and not added later as part of the backtracking solution process
	bool inOriginal = true;

	// default constructor
	Move() {}
	// constructor used to create a move
	Move(unsigned char r, unsigned char c, unsigned char v = 0) {
		row = r;
		col = c;
		val = v;
	}
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

	// gets the next empty space in the grid
	Move getNextEmptySpace();

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
	bool insert(Move move);

	// remove a value from a cell und update the validity tables
	void remove(unsigned row, unsigned col);
	void remove(Move move);

	// remove all the values from a range
	// used to backtrack really
	void remove(Move firstMove, Move secondMove);

	// overload the << operator to print
	friend std::ostream& operator<< (std::ostream& os, const SudokuBoard& sb);

	// solves the sudoku puzzle
	void solve();
private:
	// store the board
	unsigned char board[9][9];
	// not using this right now, want to get the logic working first
	unsigned char boardTransposed[9][9];
	// store whether a given space is "original"
	bool isSpaceOriginal[9][9];
	// used to check if a given row/col/box is valid for insertion
	bool rowValidityChecker[9][9];
	bool colValidityChecker[9][9];
	bool boxValidityChecker[9][9];
	// stack to keep track of which moves are possible to make
	// basically the struct used to search in the DFS
	std::stack<Move> movePossiblities;
	// stack to keep track of which moves have been made
	std::stack<Move> moveTrace;
	// keeps track of the moves that remain
	std::vector<Move> movesRequired;
	short moveRequiredIndex;
	// pointers to isSpaceOriginal and board table
	// for perfomance and convenience
	bool * isSpaceOriginalPointer;
	unsigned char * boardPointer;
	// a debugging index for the next move
	unsigned char debugIdx;
	// another debugging car to check if board is solved
	bool solved;
};




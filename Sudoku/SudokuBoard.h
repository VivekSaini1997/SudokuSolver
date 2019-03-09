#pragma once
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
	const bool& checkInsertIntoRow(unsigned row, unsigned char val);
	const bool& checkInsertIntoColumn(unsigned col, unsigned char val);
	const bool& checkInsertIntoBox(unsigned box, unsigned char val);

	// insert into the cell after doing the checks
	bool insert(unsigned row, unsigned col, unsigned char val);

private:
	// store the board
	unsigned char board[9][9];
	// not using this right now, want to get the logic working first
	unsigned char boardTransposed[9][9];
	// used to check if a given row/col/box is valid for insertion
	bool rowValidityChecker[9][9];
	bool colValidityChecker[9][9];
	bool boxValidityChecker[9][9];
};




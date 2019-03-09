#pragma once
// this is the class that holds the Sudoku Board
// should also have methods to check for validity
class SudokuBoard
{
public:
	SudokuBoard();
	~SudokuBoard();

private:
	unsigned char board[9][9];
	unsigned char boardTransposed[9][9];
};

SudokuBoard::SudokuBoard()
{
}

SudokuBoard::~SudokuBoard()
{
}
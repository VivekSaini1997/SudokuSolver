#include "stdafx.h"

#include "SudokuBoard.h"
#include <cstring>

// the board is empty so set everything to true
// can't solve from an empty board so don't try lmao
SudokuBoard::SudokuBoard()
{
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			rowValidityChecker[i][j] = colValidityChecker[i][j] = boxValidityChecker[i][j] = true;
			board[i][j] = 0;
			isSpaceOriginal[i][j] = false;
		}
	}
	// define pointers to parts of the class to make the indexing more time efficient
	isSpaceOriginalPointer = &isSpaceOriginal[0][0];
	boardPointer = &board[0][0];
	// assume the board isn't solved to begin with
	solved = false;
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
				movesRequired.push_back(Move(i, j));
			} else {
				insert(i, j, str[i * 9 + j] - '0');
				isSpaceOriginal[i][j] = true;
			}
		}
	}
	// insert permutes this, we wish to negate that
	moveRequiredIndex = 0;
	// also empty the move trace stack
	while (!moveTrace.empty()) { 
		moveTrace.pop(); 
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
	return rowValidityChecker[row][val - 1];
}

// check that an insertion given col is valid
inline const bool& SudokuBoard::checkInsertIntoColumn(unsigned col, unsigned char val) {
	return colValidityChecker[col][val - 1];
}

// sets a cell to be invalid
// used after insertion
inline void SudokuBoard::setValidity(unsigned row, unsigned col, unsigned box, unsigned val) {
	boxValidityChecker[box][val - 1] = false;
	rowValidityChecker[row][val - 1] = false; 
	colValidityChecker[col][val - 1] = false;
}

// returns a vector of valid options to enter into a given cell
std::vector<unsigned char> SudokuBoard::getValidValsForCell(unsigned row, unsigned col) {
	unsigned box = (row / 3) * 3 + (col / 3);
	// now go thorugh and construct the vector from the elements that are valid in the row, col, and box
	std::vector<unsigned char> cellValidValues;
	for (unsigned char i = 0; i < 9; i++) {
		if (boxValidityChecker[box][i] && colValidityChecker[col][i] && rowValidityChecker[row][i]) {
			cellValidValues.push_back(i+1);
		}
	}
	return cellValidValues;
}

// sets a cell to be valid
// used before removal of a cell from the board
inline void SudokuBoard::unsetValidity(unsigned row, unsigned col) {
	unsigned box = (row / 3) * 3 + (col / 3);
	unsigned val = board[row][col];
	boxValidityChecker[box][val - 1] = true;
	rowValidityChecker[row][val - 1] = true;
	colValidityChecker[col][val - 1] = true;
}

// do the checks above and then insert
bool SudokuBoard::insert(unsigned row, unsigned col, unsigned char val) {
	// gets the box from the row and column
	unsigned box = (row / 3) * 3 + (col / 3);
	// if the entry violates either the row, the column, or the box invariant, return false
	if (!(checkInsertIntoRow(row, val) && checkInsertIntoColumn(col, val) && checkInsertIntoBox(box, val))) {
		return false;
	// otherwise insert and update tables and stacks as neccessary
	} else {
		board[row][col] = val;
		setValidity(row, col, box, val);
		//moveTrace.push(Move(row, col, val));
		// we have one less empty space
		moveRequiredIndex++;
		return true;
	}
}

// do it with a move
inline bool SudokuBoard::insert(Move move) {
	return insert(move.row, move.col, move.val);
}

// remove a value from the cell from a given row and col
void SudokuBoard::remove(unsigned row, unsigned col) {
	// unsets the validity tables
	unsetValidity(row, col);
	// change the cell state back to empty (denoted by a 0)
	board[row][col] = 0;
	// remove this move from the move trace
	//moveTrace.pop();
	// we have one more empty space
	moveRequiredIndex--;
	// janky trying to fixed the issue of removing the first move
	if (moveRequiredIndex < 0) {
		moveRequiredIndex = 0;
	}
}

// do it with a move
inline void SudokuBoard::remove(Move move) {
	remove(move.row, move.col);
}

// do it by specifying two moves
// remove all moves in the range firstMove to secondMove inclusive
// excluding those already in the board from the beginning
inline void SudokuBoard::remove(Move firstMove, Move secondMove) {
	for (unsigned char idx = firstMove.row * 9 + firstMove.col; idx <= secondMove.row * 9 + secondMove.col; idx++) {
		// if it's not an original space, proceed
		if (isSpaceOriginalPointer[idx] == false) {
			Move moveToBeRemoved = Move(idx / 9, idx % 9);
			remove(moveToBeRemoved);
		}

	}
}
// gets the next empty space the brute force way
Move SudokuBoard::getNextEmptySpace() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (board[i][j] == 0) {
				return Move(i, j, 0);
			}
		}
	}
	return Move(-1, -1, 0);
}

// return the relative index of a move
// i.e. instead of using row, col to determine where the move is
// use it's index assuming the 2d board was one dimensional with the top left
// being 0 and the bottom right being 80
inline unsigned char returnRelativeIndex(const Move& move) {
	return move.row * 9 + move.col;
}

// actually solve the game
// i.e. what this program was made for
// TODO: debug why this isn't working
// TODO as well: think about the algorithm a lot more and actually plan it out
void SudokuBoard::solve() {
	// the first set of possibilities is the set available from the first move
	Move currentMove = getNextEmptySpace();
	std::vector<unsigned char> validVals = getValidValsForCell(currentMove.row, currentMove.col);
	// push all the valid moves onto the stack
	for (auto &val : validVals) {
		movePossiblities.push(Move(currentMove.row, currentMove.col, val));
	}
	// default previous move for logic involving resetting the board from one branch to another 
	Move previousMove = currentMove;
	// while there are still moves to make do something
	// TODO: figure out the victory exit condition
	while (!movePossiblities.empty()) {

		// debugging condition variable
		bool cond = (board[0][0] == 2 && board[0][1] == 4 && board[3][3] == 2 && board[3][4] == 3 && board[2][3] == 1 && board[5][3] == 7);

		// get a new space to start off with
		currentMove = movePossiblities.top();
		// pop it so you don't go back to it
		movePossiblities.pop();
		// if neccessary, remove some entries from the board
		// only if the previous move had a higher index than the current move
		// this mean we backtracked and need to clean up the board
		if (returnRelativeIndex(previousMove) >= returnRelativeIndex(currentMove)) {
			remove(currentMove, previousMove);
		}
		// now actually draw the move onto the board
		if (!insert(currentMove)) {
			// how did you not insert, don't remember tbh
			std::cout << "???" << std::endl;
		}
		// get the next move's location
		Move nextMove = getNextEmptySpace();
		// this is to exit if the next empty space is invalid
		// means the board is solved
		// TODO: figure out better exit condition
		if (nextMove.row == 255 && nextMove.col == 255) {
			solved = true;
			break;
		}
		// check all of the valid values and push them onto the moves possible stack
		std::vector<unsigned char> validVals = getValidValsForCell(nextMove.row, nextMove.col);
		for (auto &val : validVals) {
			movePossiblities.push(Move(nextMove.row, nextMove.col, val));
		}
		// now this move is the previous move
		previousMove = currentMove;
		// draw this to debug for now
		// TODO: don't actually try to solve things with this
		// it will take a very long time to do
		debugIdx = returnRelativeIndex(nextMove);
		// debug conditions
		if (cond) {
			//std::cout << *this << std::endl;
		}
		/* Experimental logic that didn't quite work
		//Move move = getNextEmptySpace();
		// debugging
		if (board[move.row][move.col] != 0) {
			// you've already visited here
			int sx = 33;
			sx++;
			// this move led to a dead end so remove it
			remove(move);
			// take the top possible move to replace this one if you don't need to backtrack
			if (!(movePossiblities.top().col == move.col && movePossiblities.top().row == move.row)) {
				insert(movePossiblities.top());
				movePossiblities.pop();
			}

		} else {
			// check all of the valid values and push them onto the moves possible stack
			std::vector<unsigned char> validVals = getValidValsForCell(move.row, move.col);
			// if there are no valid values, remove the move and return
			if (validVals.empty()) {
				remove(move);
			}
			else {
				// otherwise push all the valid moves onto the stack
				for (auto &val : validVals) {
					movePossiblities.push(Move(move.row, move.col, val));
				}
				// select a new move to be the top one and go back to doing shit
				insert(movePossiblities.top());
				movePossiblities.pop();
			}
		}
		std::cout << *this << std::endl;
		*/
	}
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
			if (i * 9 + j == sb.debugIdx && !sb.solved) {
				os << " x";
			}
			else if (sb.board[i][j] == 0) {
				os << " .";
			} else {
				os << " " << +sb.board[i][j];
			}
		}
		os << std::endl;
	}
	return os;
}
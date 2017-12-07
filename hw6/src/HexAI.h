/*********************************************************************
 * HexAI.h
 * 
 * Assignment: hw6
 *
 * 12-2017
 *
 * Note to grader:
 *********************************************************************/

#ifndef HEXAI_H_HW5
#define HEXAI_H_HW5

#include <vector>

#include "Board.h"

class HexAI {
private:
	// Member Fields
	Board* BOARD_REFERENCE;
	Board BOARD_COPY;
	int* EVALUATION;
	char MY_COLOR;
	char OPPONENT_COLOR;
	int MY_FIRST_SIDE, MY_SECOND_SIDE, OPPONENT_FIRST_SIDE, OPPONENT_SECOND_SIDE;

	// Internal Functions
	void resetEvaluations();
	void copyBoard();
	void pathToSide(int, int);
	int monteCarlo(int);
	char randomMove();
public:
	// Constructor / Destructor
	HexAI(Board*, char, char, int, int, int, int);
	~HexAI();

	// Miscellaneous
	void evaulateBoard();
	void executeMove();
};

/* Constructor / Destructor */
HexAI::HexAI(Board* passedBoardReference, char passedMyColor, char passedOpponentColor, int passedMyFirstSide, int passedMySecondSide, int passedOponentFirstSide, int passedOponentSecondSide) : BOARD_COPY(passedBoardReference->getDimension(), passedOpponentColor, passedMyColor) {
	this->BOARD_REFERENCE = passedBoardReference;
	this->EVALUATION = new int[this->BOARD_REFERENCE->getCellCount()];
	this->MY_COLOR = passedMyColor;
	this->OPPONENT_COLOR = passedOpponentColor;
	this->MY_FIRST_SIDE = passedMyFirstSide;
	this->MY_SECOND_SIDE = passedMySecondSide;
	this->OPPONENT_FIRST_SIDE = passedOponentFirstSide;
	this->OPPONENT_SECOND_SIDE = passedOponentSecondSide;
	this->resetEvaluations();
}

HexAI::~HexAI() {
	delete this->EVALUATION;
}

/* Miscellaneous */
void HexAI::evaulateBoard() {
	this->resetEvaluations();
	for (int ii = 0; ii < this->BOARD_REFERENCE->getCellCount(); ii += 1) {
		char cellColor = this->BOARD_REFERENCE->getCellColor(ii);
		if (cellColor == ' ') {
			this->EVALUATION[ii] = this->monteCarlo(5);
		}
	}
}

/**
 * Performs "passedAttempts" random fills of the board and returns the number of wins.
 */
int HexAI::monteCarlo(int passedAttempts) {
	int victories = 0;
	for (int ii = 0; ii < passedAttempts; ii += 1) {
		this->copyBoard();
		for (int ii = 0; ii < this->BOARD_COPY.getCellCount(); ii += 1) {
			char cellColor = this->BOARD_COPY.getCellColor(ii);
			if (cellColor == ' ') {
				this->BOARD_COPY.setCell(randomMove(), ii);
			}
		}
		if (this->BOARD_COPY.checkWinner(this->MY_COLOR, this->MY_FIRST_SIDE, this->MY_SECOND_SIDE)) {
			victories += 1;
		}
	}
	return victories;
}

char HexAI::randomMove() {
	int random = (rand() % 2);
	return (random == 0) ? this->MY_COLOR : this->OPPONENT_COLOR;
}

void HexAI::copyBoard() {
	for (int ii = 0; ii < this->BOARD_REFERENCE->getCellCount(); ii += 1) {
		char cellColor = this->BOARD_REFERENCE->getCellColor(ii);
		this->BOARD_COPY.setCell(cellColor, ii);
	}
}

void HexAI::pathToSide(int passedStartVertex, int passedEndVertex) {
	vector<int> discoveredPath = vector<int>();
	this->BOARD_REFERENCE->dijkstraPath(&discoveredPath, this->OPPONENT_COLOR, passedStartVertex, passedEndVertex);
	for (int ii = 0; ii < discoveredPath.size(); ii += 1) {
		int iteratedCell = discoveredPath.at(ii);
		this->EVALUATION[iteratedCell] += 1;
	}
}

void HexAI::executeMove() {
	int bestMove = 0;
	int maxValue = -1;
	for (int ii = 0; ii < this->BOARD_REFERENCE->getCellCount(); ii += 1) {
		if (this->BOARD_REFERENCE->getCellColor(ii) == ' ') {
			if (this->EVALUATION[ii] > maxValue) {
				maxValue = this->EVALUATION[ii];
				bestMove = ii;
			}
		}
	}
	this->BOARD_REFERENCE->setCell(this->MY_COLOR, bestMove);
}

/* Internal Functions */
void HexAI::resetEvaluations() {
	for (int ii = 0; ii < this->BOARD_REFERENCE->getCellCount(); ii += 1) {
		this->EVALUATION[ii] = 0;
	}
}

#endif /* HEXAI_H_ */

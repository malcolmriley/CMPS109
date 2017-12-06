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
	int* EVALUATION;
	char MY_COLOR;
	char OPPONENT_COLOR;
	int MY_FIRST_SIDE, MY_SECOND_SIDE, OPPONENT_FIRST_SIDE, OPPONENT_SECOND_SIDE;

	// Internal Functions
	void resetEvaluations();
	void evaluatePaths(vector<int>*, char);
	void pathToSide(int, int);
public:
	// Constructor / Destructor
	HexAI(Board*, char, char, int, int, int, int);
	~HexAI();

	// Miscellaneous
	void evaulateBoard();
	void executeMove();
};

/* Constructor / Destructor */
HexAI::HexAI(Board* passedBoardReference, char passedMyColor, char passedOpponentColor, int passedMyFirstSide, int passedMySecondSide, int passedOponentFirstSide, int passedOponentSecondSide) {
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

	// TODO: Monte Carlo Algorithm
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

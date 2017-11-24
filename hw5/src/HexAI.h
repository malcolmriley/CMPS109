/*********************************************************************
 * HexAI.h
 * 
 * Assignment: hw5
 *
 * 11-2017
 *********************************************************************/

#ifndef HEXAI_H_HW5
#define HEXAI_H_HW5

#include "Board.h"

class HexAI {
private:
	Board* BOARD_REFERENCE;
	int* EVALUATION;
public:
	// Constructor / Destructor
	HexAI(Board*);
	~HexAI();

	// Miscellaneous
	void evaulateBoard();
	void executeMove();
};

/* Constructor / Destructor */
HexAI::HexAI(Board* passedBoardReference) {
	this->BOARD_REFERENCE = passedBoardReference;
	this->EVALUATION = new int[this->BOARD_REFERENCE->getCellCount()];
	for (int ii = 0; ii < this->BOARD_REFERENCE->getCellCount(); ii += 1) {
		this->EVALUATION[ii] = 0;
	}
}

HexAI::~HexAI() {
	delete this->EVALUATION;
}

#endif /* HEXAI_H_ */

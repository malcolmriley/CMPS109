/*********************************************************************
 * hw5.cpp
 *
 * Assignment: hw5
 *
 * 11-2017
 *
 * Implementation file for CMPS109 HW5: Minimax/Alphabeta AI Algorithm.
 *
 * Notes to grader:
 * - Graph implemnetation is found in Graph.h
 * - Board implementation (basically graph wrapper) is found in Board.h
 * - AI implementation is found in HexAI.h
 *
 * I'll be honest, this isn't AlphaBeta or even MinMax. I couldn't
 * really figure out how to do that, so take off points as you see fit.
 *
 * Instead, the AI evaluates various paths to the opponent's side of the
 * board and checks to see which particular hex cell has the most paths
 * thorugh it. It dubs this the most valuable cell (to its opponent) and
 * places its piece there to block.
 *********************************************************************/

#include <iostream>

#include "Board.h"
#include "HexAI.h"

using namespace std;

int main() {
	Board gameBoard = Board(11, 'W', 'B');
	gameBoard.printBoard(&cout);
	return 0;
}

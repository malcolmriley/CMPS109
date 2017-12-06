/*********************************************************************
 * hw6.cpp
 *
 * Assignment: hw6
 *
 * 12-2017
 *
 * Implementation file for CMPS109 HW5: Monte Carlo AI Algorithm.
 *********************************************************************/

#include <iostream>

#include "Board.h"
#include "HexAI.h"

using namespace std;

#define SIMPLE_PROMPT "> "

template <typename T>
void getParameter(string, T*);
int indexFromCoordinates(int, int, int);
bool winnerDeclared(Board*);
bool validateIndex(int, int);
int charToInt(char);

int main() {
	int boardSize;
	char playerColor, computerColor;

	// Define board parameters
	getParameter("What size board would you like to play on?", &boardSize);
	while ((boardSize < 2) || (boardSize > 26)) {
		getParameter("It wouldn't be much fun to play on a board of that size.\nPlease enter a number greater than 1 and less than 26.", &boardSize);
	}
	getParameter("What token would you like to use to represent your pieces?", &playerColor);
	while (playerColor == ' ') {
		getParameter("That's not fair! Nobody would be able to see your pieces!\nPlease enter a different character.", &playerColor);
	}
	getParameter("What token would you like to use to represent my pieces?", &computerColor);
	while (computerColor == ' ') {
		getParameter("As much as I'd love to play with invisible pieces, it wouldn't be fair to you. I might as well be cheating.\nPlease enter a different character.", &computerColor);
	}
	while (computerColor == playerColor) {
		getParameter("There's no way I'm using YOUR ugly pieces.\nPlease enter a different character.", &computerColor);
	}

	cout << endl << "Okay, generating an " << boardSize << " by " << boardSize << " board, using \"" << playerColor << "\" for your pieces, and \"" << computerColor << "\" for mine." << endl << endl;
	cout << "You'll be connecting top to bottom, and I'll be connecting left to right." << endl;

	Board gameBoard = Board(boardSize, playerColor, computerColor);
	HexAI ai = HexAI(&gameBoard, computerColor, playerColor, gameBoard.getEastVertex(), gameBoard.getWestVertex(), gameBoard.getNorthVertex(), gameBoard.getSouthVertex());

	do {
		char playerMoveColumn;
		int playerMoveRow;
		gameBoard.printBoard(&cout);

		// Player Move
		cout << "Your turn!" << endl;
		getParameter("COL: ", &playerMoveColumn);
		while (!validateIndex(boardSize, charToInt(playerMoveColumn))) {
			getParameter("COL: ", &playerMoveColumn);
		}
		getParameter("ROW: ", &playerMoveRow);
		while (!validateIndex(boardSize, playerMoveRow)) {
			getParameter("ROW: ", &playerMoveRow);
		}
		gameBoard.setCell(playerColor, indexFromCoordinates(boardSize, playerMoveRow, charToInt(playerMoveColumn)));

		// Computer Move
		cout << "My Turn!";
		ai.evaulateBoard();
		ai.executeMove();

	} while (!winnerDeclared(&gameBoard));
	return 0;
}

bool winnerDeclared(Board* passedBoard) {
	if (passedBoard->firstPlayerWon()) {
		cout << "You win!" << endl;
		return true;
	}
	if (passedBoard->secondPlayerWon()) {
		cout << "I win!" << endl;
		return true;
	}
	return false;
}

bool validateIndex(int boardSize, int passedIndex) {
	return (passedIndex >= 0) || (passedIndex <= boardSize);
}

int indexFromCoordinates(int boardSize, int passedRow, int passedColumn) {
	return ((passedRow * boardSize) + passedColumn);
}

int charToInt(char passedCharacter) {
	if ('a' <= passedCharacter) {
		return passedCharacter - 'a';
	}
	if ('A' <= passedCharacter) {
		return passedCharacter - 'A';
	}
	return passedCharacter - '0';
}

/********************** Imported from HW2 ****************************/

/**
 * Returns a parameter retrieved from the command line.
 */
template <typename T>
void getParameter(string passedString, T* passedType) {
	cout << passedString << endl << SIMPLE_PROMPT;
	cin >> *passedType;
}

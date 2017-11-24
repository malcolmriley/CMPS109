/*********************************************************************
 * Board.h
 * 
 * Assignment: hw5
 *
 * 11-2017
 *********************************************************************/

#ifndef BOARD_H_HW5
#define BOARD_H_HW5

#include <iostream>
#include <limits>

using namespace std;

#include "Graph.h"

#define DOUBLE_INFINITY std::numeric_limits<double>::max()
#define MIN_BOARD_SIZE 2

// Utility functions
void printString(ostream*, string, int);
int numDigits(int);

/* Board Cell Implementation */
struct Cell {
	double weight;
	int predecessor;
	char color;

	Cell() {
		weight = DOUBLE_INFINITY;
		predecessor = -1;
		color = ' ';
	}
};

class Board {
private:
	// Member fields
	Graph<Cell> BOARD_REPRESENTATION;
	int SIZE;
	int NORTH, SOUTH, EAST, WEST;
	char COLOR_PLAYER1, COLOR_PLAYER2;
	Graph<Cell> initBoard(int, int*, int*, int*, int*);

public:
	// Constructor-Destructor
	Board(int, char, char);
	virtual ~Board();

	// Miscellaneous
	void printBoard(ostream*);
};

Graph<Cell> initBoard(int passedDimension, int* passedNorthNode, int* passedSouthNode, int* passedEastNode, int* passedWestNode) {
	// Instantiate new graph
	Graph<Cell> newGraph = Graph<Cell>((passedDimension * passedDimension) + 4, false); // Four extra entries for special board-edge vertices

	// Store indices of "special" board-side vertices
	(*passedNorthNode) = (passedDimension * passedDimension) + 0;
	(*passedSouthNode) = (passedDimension * passedDimension) + 1;
	(*passedWestNode) = (passedDimension * passedDimension) + 2;
	(*passedEastNode) = (passedDimension * passedDimension) + 3;

	// Connect all internal board cells, except last 4 entries in Graph (reserved for special board-edge vertices)
	for (int currentIndex = 0; currentIndex < (passedDimension * passedDimension); currentIndex += 1) {
		// If not the first column, connect to previous index
		if ((currentIndex % passedDimension) > 0) {
			int previousIndex = currentIndex - 1;
			newGraph.addEdge(currentIndex, previousIndex);
		}
		// If not the first row, connect to top left and top right cells
		if (currentIndex >= passedDimension) {
			int topLeft = currentIndex - passedDimension;
			newGraph.addEdge(currentIndex, topLeft);

			// If not in the last column, also connect top right cell
			if ((currentIndex % passedDimension) != (passedDimension - 1)) {
				int topRight = currentIndex - (passedDimension - 1);
				newGraph.addEdge(currentIndex, topRight);
			}
		}
	}

	// Connect special board-edge vertices
	for (int currentIndex = 0; currentIndex < passedDimension; currentIndex += 1) {
		int iteratedNorthEdge = (currentIndex); // First row
		int iteratedSouthEdge = (currentIndex) + (passedDimension * (passedDimension - 1)); // Last row
		int iteratedEastEdge = (currentIndex * passedDimension) + (passedDimension - 1); // Last index of each row
		int iteratedWestEdge = (currentIndex * passedDimension); // First index of each row
		newGraph.addEdge((*passedNorthNode), iteratedNorthEdge);
		newGraph.addEdge((*passedSouthNode), iteratedSouthEdge);
		newGraph.addEdge((*passedEastNode), iteratedEastEdge);
		newGraph.addEdge((*passedWestNode), iteratedWestEdge);
	}
	return newGraph;
}

/* Constructors/Destructors */
Board::Board(int passedSize, char passedFirstPlayerColor, char passedSecondPlayerColor) {
	SIZE = passedSize;
	COLOR_PLAYER1 = passedFirstPlayerColor;
	COLOR_PLAYER2 = passedSecondPlayerColor;
	BOARD_REPRESENTATION = initBoard(SIZE, &NORTH, &SOUTH, &EAST, &WEST);
}

/**
 * Prints an ASCII representation of the board.
 */
void Board::printBoard(ostream* passedStream) {
	cout << endl;

	// Print Coordinate Header
	for (int iteratedRow = 0; iteratedRow < SIZE; iteratedRow += 1) {

	}

	// Print slashes
	printString(passedStream, "/  \\", SIZE);
	cout << endl;

	for (int iteratedRow = 0; iteratedRow < SIZE; iteratedRow += 1) {
		// Print row content
		printString(passedStream, " ", iteratedRow); // Padding
		cout << "| ";
		for (int iteratedColumn = 0; iteratedColumn < SIZE; iteratedColumn += 1) {
			int iteratedCell = (iteratedRow * SIZE) + iteratedColumn;
			char cellColor = BOARD_REPRESENTATION.getVertex(iteratedCell)->color;
			cout << cellColor << " |";
		}
		cout << endl;

		// Print slashes
		printString(passedStream, " ", iteratedRow); // Padding
		printString(passedStream, "\\  /", SIZE);
		if (iteratedRow != (SIZE - 1)) {
			cout << "\\";
		}
		cout << endl;
	}
	cout << endl;
}

/**
 * Prints the passed string a number of times.
 */
void printString(ostream* passedStream, string passedString, int passedSpaceCount) {
	for (int ii = 0; ii < passedSpaceCount; ii += 1) {
		(*passedStream) << passedString;
	}
}

/**
 * Gets the number of digits of the passed integer
 */
int numDigits(int passedValue) {
	int digits = 0;
	do {
		passedValue /= 10;
		digits++;
	} while (passedValue > 0);
	return digits;
}


#endif

/*********************************************************************
 * Board.h
 * 
 * Assignment: hw5
 *
 * 11-2017
 *********************************************************************/

#ifndef BOARD_H_HW5
#define BOARD_H_HW5

#include "Board.h"
#include "Graph.h"

#include <iostream>
#include <limits>
#include <vector>
#include <queue>

using namespace std;

#define DOUBLE_INFINITY std::numeric_limits<double>::max()

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

	// Accessors
	char getFirstPlayerColor();
	char getSecondPlayerColor();
	int getNorthVertex();
	int getSouthVertex();
	int getEastVertex();
	int getWestVertex();

	// Miscellaneous
	void printBoard(ostream*);
	bool checkWinner(char, int, int);
	void dijkstraPath(vector<int>*, char, int, int);
};

/**
 * Initializes the main board.
 */
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

/* Accessors */

/* Miscellaneous */
/**
 * Checks whether the player of the indicated color has a path from the start vertex index to the end vertex index.
 */
bool Board::checkWinner(char passedColor, int passedStartIndex, int passedEndIndex) {
	vector<int> pathVector = vector<int>();
	Board::dijkstraPath(&pathVector, passedColor, passedStartIndex, passedEndIndex);

	if (pathVector.size() > 2) {
		if ((pathVector.at(0) == passedStartIndex) && (pathVector.at(pathVector.size() - 1) == passedEndIndex)) {
			return true;
		}
	}
	cout << endl;
	return false;
}

/**
 * Calculates the shortest path through the Graph, using Dijkstra's Algorithm, returning
 * the result as an ordered sequence of entries in a vector.
 *
 * Originally implemented for HW2, modified for HW4.
 */
void Board::dijkstraPath(vector<int>* passedPathVector, char passedColor, int passedStartVertex, int passedEndVertex) {
	// Initialize starting nodes
	priority_queue<int> unvisited = priority_queue<int>();
	bool visited[BOARD_REPRESENTATION.getVertexCount()];
	for (int ii = 0; ii < BOARD_REPRESENTATION.getVertexCount(); ii += 1) {
		visited[ii] = false;
	}
	visited[passedStartVertex] = true;
	BOARD_REPRESENTATION.getVertex(passedStartVertex)->weight = 0;
	BOARD_REPRESENTATION.getVertex(passedStartVertex)->predecessor = passedStartVertex;
	unvisited.push(passedStartVertex);

	// Begin algorithm proper
	while (!unvisited.empty()) {
		int currentVertex = unvisited.top();
		unvisited.pop();
		Cell* currentNode = BOARD_REPRESENTATION.getVertex(currentVertex);
		if (currentNode->color == passedColor) {
			visited[currentVertex] = true;
			for (int ii = 0; ii < BOARD_REPRESENTATION.getVertexCount(); ii += 1) {
				if (!visited[ii]) {
					if (BOARD_REPRESENTATION.adjacent(currentVertex, ii) && (ii != currentVertex)) {
						Cell* iteratedNode = BOARD_REPRESENTATION.getVertex(ii);
						if (iteratedNode->color == passedColor) { // Only consider nodes of appropriate color
							double traversalWeight = BOARD_REPRESENTATION.getEdgeWeight(currentVertex, ii) + currentNode->weight;
							if (!visited[ii]) {
								unvisited.push(ii);
							}
							if (traversalWeight < iteratedNode->weight) {
								iteratedNode->weight = traversalWeight;
								iteratedNode->predecessor = currentVertex;
							}
						}
					}
				}
			}
		}
	}

	// Add discovered path to path vector
	int iteratedVertex = passedEndVertex;
	while ((iteratedVertex >= 0) && (iteratedVertex != passedStartVertex)) {
		passedPathVector->insert(passedPathVector->begin(), iteratedVertex);
		iteratedVertex = BOARD_REPRESENTATION.getVertex(iteratedVertex)->predecessor;
	}
	passedPathVector->insert(passedPathVector->begin(), passedStartVertex);

	// Reset weights and predecessors
	for (int ii = 0; ii < BOARD_REPRESENTATION.getVertexCount(); ii += 1) {
		BOARD_REPRESENTATION.getVertex(ii)->predecessor = -1;
		BOARD_REPRESENTATION.getVertex(ii)->weight = DOUBLE_INFINITY;
	}
}

/**
 * Prints an ASCII representation of the board.
 */
void Board::printBoard(ostream* passedStream) {
	(*passedStream) << endl;

	// Print Coordinate Header
	(*passedStream) << "\t";
	for (char iteratedRow = 0; iteratedRow < SIZE; iteratedRow += 1) {
		(*passedStream) << iteratedRow;
		printString(passedStream, " ", 3);
	}
	(*passedStream) << endl;

	// Print slashes
	printString(passedStream, " /\\ ", SIZE);
	printString(passedStream, "/  \\", SIZE);
	(*passedStream) << endl;

	for (int iteratedRow = 0; iteratedRow < SIZE; iteratedRow += 1) {
		// Print coordinate header
		(*passedStream) << iteratedRow << "\t";

		// Print row content
		printString(passedStream, " ", iteratedRow); // Padding
		(*passedStream) << "| ";
		for (int iteratedColumn = 0; iteratedColumn < SIZE; iteratedColumn += 1) {
			int iteratedCell = (iteratedRow * SIZE) + iteratedColumn;
			char cellColor = BOARD_REPRESENTATION.getVertex(iteratedCell)->color;
			(*passedStream) << cellColor << " |";
		}
		(*passedStream) << endl;

		// Print slashes
		printString(passedStream, " ", iteratedRow); // Padding
		printString(passedStream, "\\  /", SIZE);
		printString(passedStream, " \\/ ", SIZE);
		if (iteratedRow != (SIZE - 1)) {
			(*passedStream) << "\\";
		}
		(*passedStream) << endl;
	}
	(*passedStream) << endl;
}

/* Utility Functions */

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

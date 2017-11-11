/*********************************************************************
 * hw4.cpp
 *
 * Assignment: hw4
 *
 * 11-2017
 *
 * Implementation file for CMPS109 HW4: Hex Board and Win Calculation
 *
 * Note to grader:
 *
 *********************************************************************/

#include <cstdlib>
#include <iostream>
#include <iterator>
#include <limits>
#include <queue>
#include <vector>

#include "Graph.h"

using namespace std;

#define DOUBLE_INFINITY std::numeric_limits<double>::max()
#define SIMPLE_PROMPT "> "
#define MIN_BOARD_SIZE 2

/* Node Definition */
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

template <typename T>
void getParameter(string, T*);
void dijkstraPath(Graph<Cell>*, vector<int>*, char, int, int);
Graph<Cell> generateBoard(int, int*, int*, int*, int*);
void populateBoard(Graph<Cell>*);
void printBoard(Graph<Cell>*, int);
bool checkWinner(Graph<Cell>*, char, int, int);
int getRandomInteger(int);
void printString(string, int);

int main() {

	int boardDimensions = MIN_BOARD_SIZE;
	int NORTH, SOUTH, EAST, WEST;
	getParameter("Please enter the dimension of the board:", &boardDimensions);
	while (boardDimensions < MIN_BOARD_SIZE) {
		cout << "Minimum board dimension should be greater than two.";
		getParameter("Please enter the dimension of the board:", &boardDimensions);
	}

	Graph<Cell> board = generateBoard(boardDimensions, &NORTH, &SOUTH, &EAST, &WEST);
	populateBoard(&board);

	if (checkWinner(&board, 'R', NORTH, SOUTH)) {
		cout << "Red Wins!" << endl << endl;
	}
	else if (checkWinner(&board, 'B', EAST, WEST)) {
		cout << "Blue Wins!" << endl << endl;
	}
	else {
		cout << "Game is a draw!" << endl << endl;
	}
	printBoard(&board, boardDimensions);

	return 0;
}

Graph<Cell> generateBoard(int passedDimension, int* passedNorthNode, int* passedSouthNode, int* passedEastNode, int* passedWestNode) {
	cout << "Generating " << passedDimension << "x" << passedDimension << " board..." << endl;

	// Instantiate new graph
	Graph<Cell> newGraph = Graph<Cell>((passedDimension * passedDimension) + 4, false); // Four extra entries for special board-edge vertices

	// Store indices of "special" board-side vertices
	(*passedNorthNode) = passedDimension + 1;
	(*passedSouthNode) = passedDimension + 2;
	(*passedEastNode) = passedDimension + 3;
	(*passedWestNode) = passedDimension + 4;

	// Connect all internal board cells, except last 4 entries in Graph (reserved for special board-edge vertices)
	for (int currentIndex = 0; currentIndex < (passedDimension * passedDimension); currentIndex += 1) {
		// If not the first column, connect to previous index
		if ((currentIndex % passedDimension) != 0) {
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

	cout << "\tBoard generated!" << endl;
	return newGraph;
}

/**
 * Populates the board with the appropriate edges.
 */
void populateBoard(Graph<Cell>* passedBoardGraph) {
	cout << "Executing random moves on board..." << endl;
	vector<int> cellsRemaining = vector<int>();

	for (int ii = 0; ii < (passedBoardGraph->getVertexCount() - 4); ii += 1) { // Ignore special vertices
		cellsRemaining.push_back(ii);
	}

	int iteration = 0;
	while(!cellsRemaining.empty()) {
		int randomIndex = getRandomInteger(cellsRemaining.size() - 1);
		char selectedColor = 'R';
		if ((iteration % 2) == 0) {
			selectedColor = 'B';
		}
		passedBoardGraph->getVertex(cellsRemaining.at(randomIndex))->color = selectedColor;
		cellsRemaining.erase(cellsRemaining.begin() + randomIndex);
		iteration += 1;
	}

	cout << "\tBoard filled!";
}

/**
 * Prints an ASCII representation of the board.
 */
void printBoard(Graph<Cell>* passedBoardGraph, int passedBoardDimensions) {

	// Print slashes
	printString("/\\", passedBoardDimensions);
	cout << endl;

	for (int iteratedRow = 0; iteratedRow < passedBoardDimensions; iteratedRow += 1) { // Ignore special vertices
		// Print row content
		printString(" ", iteratedRow); // Padding
		cout << "|";
		for (int iteratedColumn = 0; iteratedColumn < passedBoardDimensions; iteratedColumn += 1) {
			int iteratedCell = iteratedRow + iteratedColumn;
			char cellColor = passedBoardGraph->getVertex(iteratedCell)->color;
			cout << cellColor << "|";
		}
		cout << endl;
		// Print slashes
		printString(" ", iteratedRow); // Padding
		printString("\\/", passedBoardDimensions);
		cout << endl;
	}
}

/**
 * Checks whether the player of the indicated color has a path from the start vertex index to the end vertex index.
 */
bool checkWinner(Graph<Cell>* passedBoardGraph, char passedColor, int passedStartIndex, int passedEndIndex) {
	vector<int> pathVector = vector<int>();
	dijkstraPath(passedBoardGraph, &pathVector, passedColor, passedStartIndex, passedEndIndex);
	if ((pathVector.at(0) == passedEndIndex) && (pathVector.at(pathVector.size() - 1) == passedStartIndex)) {
		return true;
	}
	return false;
}

/**
 * Prints the passed string a number of times.
 */
void printString(string passedString, int passedSpaceCount) {
	for (int ii = 0; ii < passedSpaceCount; ii += 1) {
		cout << passedString;
	}
}


/********************** Imported from HW2 ****************************/

/**
 * Returns a parameter retrieved from the command line.
 */
template<typename T>
void getParameter(string passedString, T* passedType) {
	cout << passedString << endl << SIMPLE_PROMPT;
	cin >> *passedType;
}

/**
 * Calculates the shortest path through the Graph, using Dijkstra's Algorithm, returning
 * the result as an ordered sequence of entries in a vector.
 *
 * Originally implemented for HW2, modified for HW4.
 */
void dijkstraPath(Graph<Cell>* passedGraph, vector<int>* passedPathVector, char passedColor, int passedStartVertex, int passedEndVertex) {
	// Initialize starting nodes
	priority_queue<int> unvisited = priority_queue<int>();
	bool visited[passedGraph->getVertexCount()];
	for (int ii = 0; ii < passedGraph->getVertexCount(); ii += 1) {
		visited[ii] = false;
	}
	visited[passedStartVertex] = true;
	passedGraph->getVertex(passedStartVertex)->weight = 0;
	passedGraph->getVertex(passedStartVertex)->predecessor = passedStartVertex;
	unvisited.push(passedStartVertex);

	// Begin algorithm proper
	while (!unvisited.empty()) {
		int currentVertex = unvisited.top();
		unvisited.pop();
		Cell* currentNode = passedGraph->getVertex(currentVertex);
		visited[currentVertex] = true;
		for (int ii = 0; ii < passedGraph->getVertexCount(); ii += 1) {
			if (!visited[ii]) {
				if (passedGraph->adjacent(currentVertex, ii) && (ii != currentVertex)) {
					Cell* iteratedNode = passedGraph->getVertex(ii);
					double traversalWeight = DOUBLE_INFINITY; // Consider nodes of different color to have maximum possible traversal weight (untraversable)
					if (iteratedNode->color == passedColor) {
						traversalWeight = passedGraph->getEdgeWeight(currentVertex, ii) + currentNode->weight;
					}
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

	// Add discovered path to path vector
	int iteratedVertex = passedEndVertex;
	while ((iteratedVertex >= 0) && (iteratedVertex != passedStartVertex)) {
		passedPathVector->insert(passedPathVector->begin(), iteratedVertex);
		iteratedVertex = passedGraph->getVertex(iteratedVertex)->predecessor;
	}
	passedPathVector->insert(passedPathVector->begin(), passedStartVertex);
}

/**
 * Returns a random integer from zero to passedUpperBound, inclusive
 *
 * Originally written for HW2
 */
int getRandomInteger(int passedUpperBound) {
	return (rand() % (passedUpperBound + 1));
}

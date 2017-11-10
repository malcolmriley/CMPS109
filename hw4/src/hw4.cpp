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
Graph<Cell> generateBoard(int);
void populateBoard(Graph<Cell>*);
void printBoard(Graph<Cell>*);

int main() {

	int boardDimensions = MIN_BOARD_SIZE;
	int NORTH, SOUTH, EAST, WEST;
	getParameter("Please enter the dimension of the board:", &boardDimensions);
	while (boardDimensions < MIN_BOARD_SIZE) {
		cout << "Minimum board dimension should be greater than two.";
		getParameter("Please enter the dimension of the board:", &boardDimensions);
	}

	return 0;
}

Graph<Cell> generateBoard(int passedDimensions) {
	cout << "Generating " << passedDimensions << "x" << passedDimensions << " board..." << endl;

	cout << "Board generated!" << endl;
}

/**
 * Populates the board with the appropriate edges.
 */
void populateBoard(Graph<Cell>* passedBoardGraph, int* passedNorthNode, int* passedSouthNode, int* passedEastNode, int* passedWestNode) {
	cout << "Executing random moves on board...";

	cout << "Board filled!";
}

/**
 * Prints an ASCII representation of the board.
 */
void printBoard(Graph<Cell>* passedBoardGraph) {

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
	cout << "Calculating path from vertex " << passedStartVertex << " to vertex " << passedEndVertex << "..." << endl;
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
						double traversalWeight = passedGraph->getEdgeWeight(currentVertex, ii) + currentNode->weight;
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

/*********************************************************************
 * Graph.cpp
 *
 * Assignment: hw2
 *
 * 10-2017
 *
 * Implementation file for CMPS109 HW2: Dijkstra's Algorithm.
 *
 * Note to grader:
 * - For the random graph population implementation, see function
 * populateGraph.
 * - For the graph path implementation, see function dijkstraPath.
 *********************************************************************/

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <limits>
#include <queue>
#include <string>
#include <vector>
#include <fstream>

#include "Graph.h"

using namespace std;

#define ERROR_INVALID_ARGUMENT "ERROR: Invalid argument."
#define DOUBLE_INFINITY 5000
#define SIMPLE_PROMPT "> "


/* Node Definition */
struct Node {
	double weight;
	int predecessor;

	Node() {
		weight = DOUBLE_INFINITY;
		predecessor = -1;
	}
};

/* Function Declarations */
template<typename T>
void populateGraph(Graph<T>*, double);
void dijkstraPath(Graph<Node>*, vector<int>*, int, int);
template<typename T>
void printGraph(Graph<T>*, ostream*);
template<typename T>
void printPath(vector<int>*, Graph<T>*, ostream*);
double getRandomDouble(double, double);
int getRandomInteger(int, int);
template<typename T>
void getParameter(string, T*);
void getIndex(string, int*, int);

int main() {
	srand(time(NULL));
	// Parse and validate command line arguments
	string filename;
	int vertices;
	double density;
	char printout;

	/**
	 * TODO:
	 * This is the maximal density of a simple undirected graph. For future iterations that supports other graph types, this will be a value calculated by the program.
	 */
	double maxDensity = 1;

	// Get number of vertices
	getParameter("Enter the number of vertices the graph should have: ", &vertices);
	while (vertices <= 1) {
		cout << ERROR_INVALID_ARGUMENT << endl;
		if (vertices == 1) {
			cout << "Look, you and I both know that's the trivial case. Here's your shortest path, you cheeky goob:" << endl;
			cout << "0" << endl << endl << "Program terminated.";
			return 0;
		}
		else {
			cout << "Please enter a positive nonzero number greater than one." << endl;
		}
		getParameter(SIMPLE_PROMPT, &vertices);
	}

	// Get target density
	getParameter("Enter the target density of the graph: ", &density);
	while (density < 0) {
		cout << ERROR_INVALID_ARGUMENT << endl;
		cout << "Density must be positive." << endl;
	}
	if (density > maxDensity) {
		cout << ERROR_INVALID_ARGUMENT << endl;
		cout << "Density may not exceed " << maxDensity << " for a graph of " << vertices << " vertices; setting target density to " << maxDensity << endl;
		density = maxDensity;
	}

	// Get whether the graph should print out
	getParameter("Print graph representation to command line? (y/n)", &printout);
	bool print = (printout == 'y') || (printout == 'Y');

	// Get filename
	getParameter("Enter the file name for the program output: ", &filename);
	while(filename.size() <= 0) {
		cout << ERROR_INVALID_ARGUMENT << endl;
		cout << "Please enter a valid filename.";
		getParameter(SIMPLE_PROMPT, &filename);
	}
	ofstream file;
	file.open(filename);

	// Initialize and print graph
	cout << "Initializing graph with " << vertices << " vertices and intended density " << density << "." << endl;
	Graph<Node> graph = Graph<Node>(vertices, false);
	populateGraph(&graph, density);
	cout << "Graph initialized: " << vertices << " vertices, density " << graph.getDensity();
	if (print) {
		cout << ":" << endl << endl;
		printGraph(&graph, &cout);
		printGraph(&graph, &file);
	}
	else {
		cout << "." << endl;
	}
	cout << endl;

	// Get path from user
	int start;
	int end;
	getIndex("Enter index of start point:", &start, graph.getVertexCount());
	getIndex("Enter index of end point:", &end, graph.getVertexCount());

	// Calculate path
	vector<int> path = vector<int>();
	dijkstraPath(&graph, &path, start, end);
	printPath(&path, &graph, &cout);
	printPath(&path, &graph, &file);
	cout << endl;

	// Complete!
	cout << "Program terminated." << endl;
	file.close();
	return 0;
}

/**
 * Populates the passed Graph with random edges, attempting to meet the
 * indicated target density.
 */
template<typename T>
void populateGraph(Graph<T>* passedGraph, double passedTargetDensity) {
	double quantityVertices = passedGraph->getVertexCount();
	double minimumDensity = 1 / quantityVertices;
	if (passedTargetDensity < minimumDensity) {
		cout << "Warning: Minimum density for a connected graph with " << quantityVertices << " vertices is: " << minimumDensity << endl;
		cout << "This will be the final density of the graph instead of " << passedTargetDensity << endl;
		passedTargetDensity = minimumDensity;
	}

	// Establish minimum connected graph by walking between all vertices
	vector<int> vertexList = vector<int>();
	vector<int> unvisited = vector<int>();
	vertexList.reserve(passedGraph->getVertexCount());
	unvisited.reserve(passedGraph->getVertexCount());
	for (int ii = 0; ii < quantityVertices; ii += 1) {
		vertexList.push_back(ii);
	}
	while (vertexList.size() > 0) {
		int index = getRandomInteger(0, (vertexList.size() - 1));
		int value = vertexList.at(index);
		unvisited.push_back(value);
		vertexList.erase(vertexList.begin() + index);
	}
	cout << endl;
	for (int ii = 0; ii < (quantityVertices - 1); ii += 1) {
		int first = unvisited.at(ii);
		int second = unvisited.at(ii + 1);
		/**
		 * TODO:
		 * Future iterations could have a configurable value for edgeWeight.
		 */
		double edgeWeight = getRandomDouble(0.001, 5.000);
		passedGraph->addEdge(first, second, edgeWeight);
	}

	// Add random edges until density requirement is satisfied
	while (passedGraph->getDensity() < passedTargetDensity) {
		int first = getRandomInteger(0, quantityVertices - 1);
		int second = getRandomInteger(0, quantityVertices - 1);
		passedGraph->addEdge(first, second);
	}
}

/**
 * Calculates the shortest path through the Graph, using Dijkstra's Algorithm, returning
 * the result as an ordered sequence of entries in a vector.
 */
void dijkstraPath(Graph<Node>* passedGraph, vector<int>* passedPathVector, int passedStartVertex, int passedEndVertex) {
	cout << "Calculating path from vertex " << passedStartVertex << " to vertex " << passedEndVertex << "..." << endl;
	// Initialize starting nodes
	priority_queue<int> unvisited = priority_queue<int>();
	bool visited[passedGraph->getVertexCount()];
	for (int ii = 0; ii < passedGraph->getVertexCount(); ii += 1) {
		visited[ii] = false;
	}
	passedGraph->getVertex(passedStartVertex)->weight = 0;
	passedGraph->getVertex(passedStartVertex)->predecessor = passedStartVertex;
	unvisited.push(passedStartVertex);

	// Begin algorithm proper
	while (!unvisited.empty()) {
		int currentVertex = unvisited.top();
		unvisited.pop();
		Node* currentNode = passedGraph->getVertex(currentVertex);
		visited[currentVertex] = true;
		for (int ii = 0; ii < passedGraph->getVertexCount(); ii += 1) {
			if (passedGraph->adjacent(currentVertex, ii) && (ii != currentVertex)) {
				Node* iteratedNode = passedGraph->getVertex(ii);
				double traversalWeight = passedGraph->getEdgeWeight(currentVertex, ii) + currentNode->weight;
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

	// Add discovered path to path vector
	int iteratedVertex = passedEndVertex;
	while ((iteratedVertex >= 0) && (iteratedVertex != passedStartVertex)) {
		passedPathVector->push_back(iteratedVertex);
		iteratedVertex = passedGraph->getVertex(iteratedVertex)->predecessor;
	}
	passedPathVector->push_back(passedStartVertex);
}

/**
 * Prints a representation of the Graph to cout.
 */
template<typename T>
void printGraph(Graph<T>* passedGraph, ostream* passedStream) {
	if (passedGraph->getVertexCount() > 0) {
		for (int ii = 0; ii < passedGraph->getVertexCount(); ii += 1) {
			(*passedStream) << "Vertex " << ii << " is adjacent to: " << endl << "\t( ";
			for (int jj = 0; jj < passedGraph->getVertexCount(); jj += 1) {
				if (passedGraph->adjacent(ii, jj)) {
					(*passedStream) << jj << " ";
				}
			}
			(*passedStream) << ")" << endl;
		}
		(*passedStream) << endl;
	}
}

/**
 * Prints the passed vector, represented as a path through a graph.
 */
template<typename T>
void printPath(vector<int>* passedVector, Graph<T>* passedGraph, ostream* passedStream) {
	if (passedVector->size() > 0) {
		(*passedStream) << "Found path: (" << passedVector->at(0) << " to " << passedVector->at(passedVector->size() - 1) << ")" << endl;
		double pathWeight = 0;
		(*passedStream) << passedVector->at(0);
		for (int ii = (passedVector->size() - 1); ii >= 0; ii -= 1) {
			(*passedStream) << " -> " << passedVector->at(ii);
			pathWeight += passedGraph->getEdgeWeight(passedVector->at(ii - 1), passedVector->at(ii));
		}
		(*passedStream) << endl << "Path has total weight: " << pathWeight << endl << endl;
	}
}

/**
 * Retrieves a random double in the passed range, inclusive
 */
double getRandomDouble(double passedLowerBound, double passedUpperBound) {
	double value = ((double) rand()) / RAND_MAX;
	return passedLowerBound + (value * (passedUpperBound - passedLowerBound));
}

/**
 * Returns a random integer in the passed range, inclusive
 */
int getRandomInteger(int passedLowerBound, int passedUpperBound) {
	return (rand() % (passedUpperBound - passedLowerBound + 1)) + passedLowerBound;
}

/**
 * Returns a parameter retrieved from the command line.
 */
template<typename T>
void getParameter(string passedString, T* passedType) {
	cout << passedString << endl << SIMPLE_PROMPT;
	cin >> *passedType;
}

/**
 * Returns a validated index value from the command line.
 */
void getIndex(string passedString, int* passedValue, int passedMaxValue) {
	getParameter(passedString, passedValue);
	while ((*passedValue < 0) || (*passedValue >= passedMaxValue)) {
		cout << ERROR_INVALID_ARGUMENT << endl;
		cout << "Please enter a vertex index in the range 0 to " << passedMaxValue - 1 << endl;
		getParameter(passedString, passedValue);
	}
}

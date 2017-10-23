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
#include <fstream>
#include <ctime>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <vector>
#include "Graph.h"
using namespace std;

#define ERROR_INVALID_ARGUMENT "ERROR: Invalid argument."
#define SIMPLE_PROMPT "> "


/* Node Definition */
struct Node {
	double weight;
	int predecessor;

	Node() {
		weight = std::numeric_limits<double>::max();
		predecessor = -1;
	}
};

/* Function Declarations */
template<typename T>
void populateGraph(Graph<T>*, double);
vector<int> dijkstraPath(Graph<Node>*, int, int);
template<typename T>
void printGraph(Graph<T>*, ostream*);
template<typename T>
void printPath(vector<int>*, Graph<T>*, ostream*);
double getRandomDouble(double, double);
int getRandomInteger(int, int);
template<typename T>
void getParameter(string, T*);

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
	while (vertices <= 0) {
		cout << ERROR_INVALID_ARGUMENT << endl;
		cout << "No paths available through a graph containing no vertices!" << endl;
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

	// Calculate path
	int start = getRandomInteger(0, (vertices - 1));
	int end = getRandomInteger(0, (vertices - 1));
	cout << "Calculating path from vertex " << start << " to vertex " << end << "..." << endl;
	vector<int> path = dijkstraPath(&graph, start, end);
	cout << "Found path: " << endl;
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
	double quantityVertices = (*passedGraph).getVertexCount();
	double minimumDensity = 1 / quantityVertices;
	if (passedTargetDensity < minimumDensity) {
		cout << "Warning: Minimum density for a connected graph with " << quantityVertices << " vertices is: " << minimumDensity << endl;
		cout << "This will be the final density of the graph instead of " << passedTargetDensity << endl;
	}

	// Establish minimum connected graph by walking between all vertices
	vector<int> vertexList = vector<int>();
	vector<int> unvisited = vector<int>();
	for (int ii = 0; ii < quantityVertices; ii += 1) {
		vertexList.push_back(ii);
	}
	while (vertexList.size() > 0) {
		int index = getRandomInteger(0, (vertexList.size() - 1));
		int value = vertexList.at(index);
		unvisited.push_back(value);
		vertexList.erase(vertexList.begin() + index);
	}
	for (int ii = 0; ii < (quantityVertices - 1); ii += 1) {
		int first = unvisited.at(ii);
		int second = unvisited.at(ii + 1);
		/**
		 * TODO:
		 * Future iterations could have a configurable value for edgeWeight.
		 */
		double edgeWeight = getRandomDouble(0.001, 5.000);
		(*passedGraph).addEdge(first, second, edgeWeight);
	}

	// Add random edges until density requirement is satisfied
	while ((*passedGraph).getDensity() < passedTargetDensity) {
		int first = getRandomInteger(0, quantityVertices - 1);
		int second = getRandomInteger(0, quantityVertices - 1);
		(*passedGraph).addEdge(first, second);
	}
}

/**
 * Calculates the shortest path through the Graph, using Dijkstra's Algorithm, returning
 * the result as an ordered sequence of entries in a vector.
 */
vector<int> dijkstraPath(Graph<Node>* passedGraph, int passedStartVertex, int passedEndVertex) {
	vector<int> path = vector<int>();
	// Initialize starting nodes
	(*(*passedGraph).getVertex(passedStartVertex)).weight = 0;
	vector<int> unvisited = vector<int>();
	unvisited.push_back(0);

	// Begin algorithm proper
	while (unvisited.size() > 0) {
		int currentVertex = unvisited.at(unvisited.size() - 1);
		for (int iteratedVertex = 0; iteratedVertex < (*passedGraph).getVertexCount(); iteratedVertex += 1) {
			Node* vertex = (*passedGraph).getVertex(iteratedVertex);
			if ((*passedGraph).adjacent(currentVertex, iteratedVertex)) {
				double traversalCost = (*passedGraph).getEdgeWeight(currentVertex, iteratedVertex) + (*vertex).weight;
				if (traversalCost < (*vertex).weight) {
					(*vertex).weight = traversalCost;
					(*vertex).predecessor = currentVertex;
				}
				unvisited.push_back(iteratedVertex);
			}
			unvisited.pop_back();
		}
	}

	// Add discovered path to path vector
	int iteratedVertex = passedEndVertex;
	while ((*(*passedGraph).getVertex(iteratedVertex)).predecessor >= 0) {
		path.push_back(iteratedVertex);
		iteratedVertex = (*(*passedGraph).getVertex(iteratedVertex)).predecessor;
	}
	return path;
}

/**
 * Prints a representation of the Graph to cout.
 */
template<typename T>
void printGraph(Graph<T>* passedGraph, ostream* passedStream) {
	if ((*passedGraph).getVertexCount() > 0) {
		for (int ii = 0; ii < (*passedGraph).getVertexCount(); ii += 1) {
			(*passedStream) << "Vertex " << ii << " is adjacent to: " << endl << "\t( ";
			for (int jj = 0; jj < (*passedGraph).getVertexCount(); jj += 1) {
				if ((*passedGraph).adjacent(ii, jj)) {
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
	if ((*passedVector).size() > 0) {
		(*passedStream) << (*passedVector).at(0);
		for (int ii = 1; ii < (*passedVector).size(); ii += 1) {
			(*passedStream) << " -> " << (*passedVector).at(ii);
		}
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

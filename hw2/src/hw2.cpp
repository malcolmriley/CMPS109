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
#include <string>
#include <vector>

#include "Graph.h"

#define ERROR_INVALID_ARGUMENT "ERROR: Invalid argument."

using namespace std;

/* Function Declarations */
template <typename T>
void populateGraph(Graph<T>, double);
template <typename T>
vector<int> dijkstraPath(Graph<T>);
template <typename T>
void printGraph(Graph<T>);
template <typename T>
void printPath(vector<int>, Graph<T>);
double getRandomDouble(double, double);
int getRandomInteger(int, int);

int main(int passedArgumentCount, char* passedArguments[]) {
	srand(time(NULL));
	// TODO: Switch to file output type
	if (passedArgumentCount < 3) {
		cout << ERROR_INVALID_ARGUMENT << endl << "Syntax: dijkstra [outputfile] [quantity] [density] [printgraph]" << endl;
		cout << "\tWhere \"outputfile\" is the path to the output file, \"quantity\" is an integer quantity of vertices to generate, and \"density\" is a floating-point value representing the target graph density." << endl;
		cout << "\tOptional parameter [printgraph] indicates whether the graph should be printed before calculating the path - \"y\" for yes, all else for no." << endl;
		cout << "If the output file is unreachable, undefined, or otherwise inaccessible to the program, it will print to the console instead." << endl;
	}
	else {
		// Parse and validate command line arguments
		string filename = string(passedArguments[1]);
		int vertices = atoi(passedArguments[2]); // Parse number of vertices from command line
		double density = strtod(passedArguments[3], nullptr); // Parse density target from command line
		double maxDensity = 3;
		string arg3 = string(passedArguments[4]);
		bool print = ((arg3.compare("y") == 0) || (arg3.compare("Y") == 0)); // Parse optional argument to print graph
		if (vertices <= 0) {
			cout << ERROR_INVALID_ARGUMENT << endl;
			cout << "No paths available through a graph containing no vertices!" << endl;
			return 1;
		}
		if (density < 0) {
			cout << ERROR_INVALID_ARGUMENT << endl;
			cout << "Density must be positive." << endl;
			return 1;
		}
		if (density > maxDensity) {
			cout << ERROR_INVALID_ARGUMENT << endl;
			cout << "Density may not exceed " << maxDensity << " for a graph of " << vertices << " vertices; setting target density to " << maxDensity << endl;
			density = 1;
		}

		// Initialize and print graph
		cout << "Initializing graph with " << vertices << " vertices and intended density " << density << "." << endl;
		Graph<double> graph = Graph<double>(vertices, false);
		populateGraph(graph, density);
		cout << "Graph initialized: " << vertices << " vertices, density " << graph.getDensity();
		if (print) {
			cout << ":" << endl << endl;
			printGraph(graph);
		}
		else {
			cout << "." << endl;
		}
		cout << endl;

		// Calculate path
		cout << "Calculating path..." << endl;
		vector<int> path = dijkstraPath(graph);
		cout << "Found path: " << endl;
		printPath(path, graph);
		cout << endl;

	}
	// Complete!
	cout << "Program terminated." << endl;
	return 0;
}

/**
 * Populates the passed Graph with random edges, attempting to meet the
 * indicated target density.
 */
template<typename T>
void populateGraph(Graph<T> passedGraph, double passedTargetDensity) {
	int quantityVertices = passedGraph.getVertexCount();
	double minimumDensity = (quantityVertices - 1) / quantityVertices;
	if (minimumDensity < passedTargetDensity) {
		cout << "Warning: Minimum density for a connected graph with " << quantityVertices << " vertices is: " << minimumDensity << endl;
		cout << "This will be the final density of the graph instead of " << passedTargetDensity;
	}

	// Establish minimum connected graph by walking between all vertices
	vector<int> vertexList = vector<int>();
	vector<int> unvisited = vector<int>();
	for (int ii = 0; ii < quantityVertices; ii += 1) {
		vertexList.push_back(ii);
	}
	while(unvisited.size() < quantityVertices) {
		int index = getRandomInteger(0, (vertexList.size() - 1));
		int value = vertexList.at(index);
		unvisited.push_back(value);
		vertexList.erase(vertexList.begin() + index);
	}
	for (int ii = 0; ii < (quantityVertices - 1); ii += 1) {
		int first = unvisited.at(ii);
		int second = unvisited.at(ii + 1);
		passedGraph.addEdge(first, second);
	}

	// Add random edges until density requirement is satisfied
	while (passedGraph.getDensity() < passedTargetDensity) {
		int first = getRandomInteger(0, quantityVertices - 1);
		int second = getRandomInteger(0, quantityVertices - 1);
		while (first == second) {
			second = getRandomInteger(0, quantityVertices - 1);
		}
		passedGraph.addEdge(first, second);
	}
}

/**
 * Calculates the shortest path through the Graph, using Dijkstra's Algorithm, returning
 * the result as an ordered sequence of entries in a vector.
 */
template <typename T>
vector<int> dijkstraPath(Graph<T> passedGraph) {
	vector<int> path = vector<int>();
	// TODO:
	return path;
}

/**
 * Prints a representation of the Graph to cout.
 */
template <typename T>
void printGraph(Graph<T> passedGraph) {
	if (passedGraph.getVertexCount() > 0) {
		for (int ii = 0; ii < passedGraph.getVertexCount(); ii += 1) {
			cout << "Vertex " << ii << " is adjacent to: " << endl << "\t";
			for (int jj = 0; jj < passedGraph.getVertexCount(); jj += 1) {
				if (passedGraph.adjacent(ii, jj)) {
					cout << jj << " ";
				}
			}
			cout << endl << endl;
		}
	}
}

/**
 * Prints the passed vector, represented as a path through a graph.
 */
template <typename T>
void printPath(vector<int> passedVector, Graph<T> passedGraph) {
	if (passedVector.size() > 0) {
		cout << passedVector.at(0);
		for (int ii = 1; ii < passedVector.size(); ii += 1) {
			cout << " -> " << passedVector.at(ii);
		}
	}
}

/**
 * Retrieves a random double in the passed range, inclusive
 */
double getRandomDouble(double passedLowerBound, double passedUpperBound) {
	double value = ((double)rand()) / RAND_MAX;
	return passedLowerBound + (value * (passedUpperBound - passedLowerBound));
}

/**
 * Returns a random integer in the passed range, inclusive
 */
int getRandomInteger(int passedLowerBound, int passedUpperBound) {
	return (rand() % (passedUpperBound - passedLowerBound + 1)) + passedLowerBound;
}

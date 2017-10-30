/*********************************************************************
 * hw3.cpp
 *
 * Assignment: hw3
 *
 * 10-2017
 *
 * Implementation file for CMPS109 HW3: Minimum Spanning Tree Algorithm.
 *********************************************************************/

#include <cstdlib>
#include <iostream>
#include <iterator>
#include <vector>

#include "Graph.h"

#define SIMPLE_PROMPT "> "
#define ERROR_INVALID_ARGUMENT "ERROR: Invalid argument."

using namespace std;

// New Functions

// HW2 Imported Functions
template<typename T>
void populateGraph(Graph<T>*, double, double, double);
double getRandomDouble(double, double);
int getRandomInteger(int);
template <typename T>
void getParameter(string, T*);
template<typename T>
void printGraph(Graph<T>*, ostream*);
template<typename T>
void calculateMST(Graph<T>*, Graph<T>*);

int main() {
	int vertices;
	double density, minWeight, maxWeight, avgEdges;

	// Get number of vertices
	getParameter("Enter the number of vertices the graph should have: ", &vertices);
	while (vertices <= 1) {
		cout << ERROR_INVALID_ARGUMENT << endl;
		cout << "Please enter a positive nonzero number greater than one." << endl;
		getParameter(SIMPLE_PROMPT, &vertices);
	}

	// Get target density
	getParameter("Enter the average number of connected edges per vertex: ", &avgEdges);
	while (avgEdges <= 0) {
		cout << ERROR_INVALID_ARGUMENT << endl;
		cout << "Please enter a positive nonzero decimal number:" << endl;
		getParameter(SIMPLE_PROMPT, &avgEdges);
	}
	density = (1 / ((double)vertices) * avgEdges);

	// Get min and max weights
	getParameter("Enter minimum edge weight: ", &minWeight);
	while (minWeight < 0) {
		cout << ERROR_INVALID_ARGUMENT << endl;
		cout << "Please enter a non-negative decimal number for the minimum edge weight: " << endl;
		getParameter(SIMPLE_PROMPT, &minWeight);
	}
	getParameter("Enter maximum edge weight: ", &maxWeight);
	while ((maxWeight < 0) && (maxWeight < minWeight)) {
		cout << ERROR_INVALID_ARGUMENT << endl;
		if (maxWeight < 0) {
			cout << "Please enter a non-negative decimal number for the maximum edge weight. " << endl;
		}
		if (maxWeight < minWeight) {
			cout << "Maximum edge weight must exceed minimum edge weight, please enter a value greater than " << minWeight << ": " << endl;
		}
		getParameter(SIMPLE_PROMPT, &maxWeight);
	}

	// Initialize and print graph
	cout << "Initializing graph with " << vertices << " vertices and intended density " << density << "." << endl;
	Graph<double> sourceGraph = Graph<double>(vertices, false);
	Graph<double> destGraph = Graph<double>(vertices, false);
	populateGraph(&sourceGraph, density, minWeight, maxWeight);
	cout << "Graph initialized: " << vertices << " vertices, " << sourceGraph.getEdgeCount() << " edges, and density " << sourceGraph.getDensity() << ": " << endl;
	printGraph(&sourceGraph, &cout);
	cout << endl << endl;

	// Calculate MST
	cout << "Calculating minimum spanning tree for graph..." << endl;
	calculateMST(&sourceGraph, &destGraph);

	cout << "Minimum spanning tree found!" << endl;
	printGraph(&destGraph, &cout);

	return 0;
}

template<typename T>
void calculateMST(Graph<T>* passedSourceGraph, Graph<T>* passedDestinationGraph) {

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
 * Prints a representation of the Graph to the passed ostream (fstream or cout, for instance).
 *
 * Originally written for HW2
 */
template<typename T>
void printGraph(Graph<T>* passedGraph, ostream* passedStream) {
	if (passedGraph->getVertexCount() > 0) {
		for (int ii = 0; ii < passedGraph->getVertexCount(); ii += 1) {
			(*passedStream) << "Vertex\t( " << ii << " ) is adjacent to: " << endl;
			for (int jj = 0; jj < passedGraph->getVertexCount(); jj += 1) {
				if (passedGraph->adjacent(ii, jj)) {
					(*passedStream) << "\t( " << jj << " ), by edge with weight: " << passedGraph->getEdgeWeight(ii, jj) << endl;
				}
			}
			(*passedStream) << endl;
		}
		(*passedStream) << endl;
	}
}

/**
 * Populates the passed graph with random edges, guaranteeing a connected graph by performing a random edge walk between all included vertices
 *
 * Originally written for HW2
 */
template<typename T>
void populateGraph(Graph<T>* passedGraph, double passedTargetDensity, double passedMinimumWeight, double passedMaximumWeight) {
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
		int index = getRandomInteger(vertexList.size() - 1);
		int value = vertexList.at(index);
		unvisited.push_back(value);
		vertexList.erase(vertexList.begin() + index);
	}
	for (int ii = 0; ii < (quantityVertices - 1); ii += 1) {
		int first = unvisited.at(ii);
		int second = unvisited.at(ii + 1);
		double edgeWeight = getRandomDouble(passedMinimumWeight, passedMaximumWeight);
		passedGraph->addEdge(first, second, edgeWeight);
	}

	// Add random edges until density requirement is satisfied
	while (passedGraph->getDensity() < passedTargetDensity) {
		int first = getRandomInteger(quantityVertices - 1);
		int second = getRandomInteger(quantityVertices - 1);
		if (first != second) {
			double edgeWeight = getRandomDouble(passedMinimumWeight, passedMaximumWeight);
			passedGraph->addEdge(first, second, edgeWeight);
		}
	}
}

/**
 * Retrieves a random double in the passed range, inclusive
 *
 * Originally written for HW2
 */
double getRandomDouble(double passedLowerBound, double passedUpperBound) {
	double value = ((double) rand()) / ((double)RAND_MAX);
	return (value * (passedUpperBound - passedLowerBound)) + passedLowerBound;
}

/**
 * Returns a random integer from zero to passedUpperBound, inclusive
 *
 * Originally written for HW2
 */
int getRandomInteger(int passedUpperBound) {
	return (rand() % (passedUpperBound + 1));
}

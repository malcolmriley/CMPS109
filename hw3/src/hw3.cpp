/*********************************************************************
 * hw3.cpp
 *
 * Assignment: hw3
 *
 * 10-2017
 *
 * Implementation file for CMPS109 HW3: Minimum Spanning Tree Algorithm.
 *
 * Note to grader:
 * - MST Algorithm implementation is within calculateMST and
 * getMinimalEdge functions.
 *********************************************************************/

#include <cstdlib>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

#include "Graph.h"

#define SIMPLE_PROMPT "> "
#define ERROR_INVALID_ARGUMENT "ERROR: Invalid argument."
#define DOUBLE_INFINITY std::numeric_limits<double>::max()

using namespace std;

// New Functions
struct Edge {
	Edge() {
		first = -1;
		second = -1;
		index = -1;
		weight = DOUBLE_INFINITY;
		initialized = false;
	}
	int first, second, index;
	double weight;
	bool initialized;
};

template <typename T>
void calculateMST(Graph<T>*, Graph<T>*);
template <typename T>
Edge getMinimalEdge(Graph<T>*, vector<int>*, vector<int>*);
template <typename T>
void compareGraphs(Graph<T>*, Graph<T>*, ostream*);

// HW2 Imported Functions
template<typename T>
void populateGraph(Graph<T>*, double, double, double);
double getRandomDouble(double, double);
int getRandomInteger(int);
template <typename T>
void getParameter(string, T*);
template<typename T>
void printGraph(Graph<T>*, ostream*);

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
	cout << "Graph initialized: " << vertices << " vertices, " << sourceGraph.getEdgeCount() << " edges, and density " << sourceGraph.getDensity() << ": " << endl << endl;
	printGraph(&sourceGraph, &cout);
	cout << endl << endl;

	// Calculate MST
	cout << "Calculating minimum spanning tree for graph..." << endl;
	calculateMST(&sourceGraph, &destGraph);

	cout << "Minimum spanning tree found!" << endl << endl;
	printGraph(&destGraph, &cout);

	compareGraphs(&sourceGraph, &destGraph, &cout);

	return 0;
}

/*
 * Calculates the minimum spanning tree of the first passed graph, using Prim's Algorithm, outputting to the second passed graph.
 */
template<typename T>
void calculateMST(Graph<T>* passedSourceGraph, Graph<T>* passedDestinationGraph) {
	int vertices = passedSourceGraph->getVertexCount();
	vector<int> unconnected = vector<int>();
	vector<int> connected = vector<int>();

	// Add zero as first connected node
	connected.push_back(0);
	for (int ii = 1; ii < vertices; ii += 1) {
		unconnected.push_back(ii);
	}

	while (!unconnected.empty()) {
		Edge edge = getMinimalEdge(passedSourceGraph, &connected, &unconnected);
		if (edge.initialized) {
			passedDestinationGraph->addEdge(edge.first, edge.second, passedSourceGraph->getEdgeWeight(edge.first, edge.second));
			unconnected.erase(unconnected.begin() + edge.index);
			connected.push_back(edge.second);
		}
	}
}

/**
 * Returns the minimal edge discovered between the two vertex sets: that of the connected vertices, and the unconnected vertices.
 */
template <typename T>
Edge getMinimalEdge(Graph<T>* passedGraph, vector<int>* passedConnectedVertices, vector<int>* passedUnconnectedVertices) {
	Edge minimalEdge;
	for (int ii = 0; ii < passedConnectedVertices->size(); ii += 1) {
		int connected = passedConnectedVertices->at(ii);
		for (int jj = 0; jj < passedUnconnectedVertices->size(); jj += 1) {
			int unconnected = passedUnconnectedVertices->at(jj);
			if (passedGraph->adjacent(connected, unconnected)) {
				double weight = passedGraph->getEdgeWeight(connected, unconnected);
				if (weight < minimalEdge.weight) {
					minimalEdge.weight = weight;
					minimalEdge.first = connected;
					minimalEdge.second = unconnected;
					minimalEdge.initialized = true;
					minimalEdge.index = jj;
				}
			}
		}
	}
	return minimalEdge;
}

template <typename T>
void compareGraphs(Graph<T>* passedFirstGraph, Graph<T>* passedSecondGraph, ostream* passedStream) {
	(*passedStream) << "Comparison: " << endl;
	(*passedStream) << "\tOriginal had " << passedFirstGraph->getEdgeCount() << " edges, reduced to " << passedSecondGraph->getEdgeCount() << endl;
	(*passedStream) << "\tOriginal weighed " << passedFirstGraph->getTotalWeight() << ", reduced to " << passedSecondGraph->getTotalWeight() << endl;
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

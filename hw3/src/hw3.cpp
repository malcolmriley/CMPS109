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

using namespace std;

template<typename T>
void populateGraph(Graph<T>*, double, double, double);
double getRandomDouble(double, double);
int getRandomInteger(int);

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}

/**
 * Prints a representation of the Graph to the passed ostream (fstream or cout, for instance).
 */
template<typename T>
void printGraph(Graph<T>* passedGraph, ostream* passedStream) {
	if (passedGraph->getVertexCount() > 0) {
		for (int ii = 0; ii < passedGraph->getVertexCount(); ii += 1) {
			(*passedStream) << "Vertex\t( " << ii << " ) is adjacent to: " << endl;
			for (int jj = 0; jj < passedGraph->getVertexCount(); jj += 1) {
				if (passedGraph->adjacent(ii, jj)) {
					(*passedStream) << "\t( " <<jj << " ), by edge with weight: " << passedGraph->getEdgeWeight(ii, jj) << endl;
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
 * Algorithm originally implemented for HW2
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
	cout << endl;
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
 */
double getRandomDouble(double passedLowerBound, double passedUpperBound) {
	double value = ((double) rand()) / ((double)RAND_MAX);
	return (value * (passedUpperBound - passedLowerBound)) + passedLowerBound;
}

/**
 * Returns a random integer from zero to passedUpperBound, inclusive
 */
int getRandomInteger(int passedUpperBound) {
	return (rand() % (passedUpperBound + 1));
}

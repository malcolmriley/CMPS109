/*********************************************************************
 * Graph.cpp
 * 
 * Assignment: hw2
 *
 * 10-2017
 *
 * Graph class implementation. Uses matrix to track edges (connectivity
 * matrix implementation type), with the value stored in the matrix being
 * the edge weight. A value less than zero stored in the matrix is
 * considered to be "disconnected", that is to say, there is no edge
 * connecting the relevant nodes.
 *
 * The graph may be set to be directed or undirected.
 *********************************************************************/

#include "Graph.h"

/* Constructor-Destructor */

/**
 * Initializer function
 */
template <typename T>
void Graph<T>::init(int passedNodeQuantity, bool passedIsDirected) {
	// Initialize internal values
	directed = passedIsDirected;
	vertices = passedNodeQuantity;
	edgeSum = 0;
	edges = 0;
	edgeMatrix = new double*[passedNodeQuantity];
	nodeValues = new T[passedNodeQuantity];
}

template <typename T>
Graph<T>::Graph(int passedNodeQuantity, bool passedIsDirected) {
	Graph::init(passedNodeQuantity, passedIsDirected);
}

/**
 * Initializes a new Graph with passedNodeQuantity number of nodes; passedIsDirected determines
 * whether or not this is a directed Graph.
 */
template <typename T>
Graph<T>::Graph(int passedNodeQuantity, bool passedIsDirected, T passedNodeArray[]) {
	Graph::init(passedNodeQuantity, passedIsDirected);

	// Initialize node value array
	if (passedNodeArray) {
		for (int ii = 0; ii < passedNodeQuantity; ii += 1) {
			nodeValues[ii] = passedNodeArray[ii];
		}
	}

	// Initialize "dynamic" 2D array
	for (int ii = 0; ii < passedNodeQuantity; ii += 1) {
		edgeMatrix[ii] = new double[passedNodeQuantity];
		for (int jj = 0; jj < passedNodeQuantity; jj += 1) {
			edgeMatrix[ii][jj] = EDGE_UNDEFINED;
		}
	}
}

template <typename T>
Graph<T>::~Graph() {
	// Free stored edgeMatrix
	for (int ii = 0; ii < vertices; ii += 1) {
		delete[] edgeMatrix[ii];
	}
	delete[] edgeMatrix;
	delete[] nodeValues;
}

/* Internal Methods */
template <typename T>
bool Graph<T>::validateIndex(int passedIndex) {
	return (passedIndex > 0);
}

template <typename T>
bool Graph<T>::validateEdge(int passedFirstIndex, int passedSecondIndex) {
	return(Graph::validateIndex(passedFirstIndex) && Graph::validateIndex(passedSecondIndex) && Graph::edgeDefined(passedFirstIndex, passedSecondIndex));
}

template <typename T>
bool Graph<T>::edgeDefined(int passedFirstIndex, int passedSecondIndex) {
	return(edgeMatrix[passedFirstIndex][passedSecondIndex] >= 0);
}

template <typename T>
void Graph<T>::setEdge(int passedFirstNode, int passedSecondNode, double passedEdgeWeight) {
	edgeMatrix[passedFirstNode][passedSecondNode] = passedEdgeWeight;
	if (!directed) {
		edgeMatrix[passedSecondNode][passedFirstNode] = passedEdgeWeight;
	}
}

/* Accessors */

/**
 * Returns the number of vertices (nodes) in this graph.
 */
template <typename T>
int Graph<T>::getVertexCount() {
	return vertices;
}

/**
 * Returns the number of edges in this graph.
 */
template <typename T>
int Graph<T>::getEdgeCount() {
	return edges;
}

/**
 * Returns whether or not the Graph has an edge between the first and second nodes.
 *
 * If the Graph is directed, checks for edge in the direction passedFirstNode -> passedSecondNode.
 */
template <typename T>
bool Graph<T>::adjacent(int passedFirstNode, int passedSecondNode) {
	if (validateEdge(passedFirstNode, passedSecondNode)) {
		if (directed) {
			return(Graph::edgeDefined(passedFirstNode, passedSecondNode));
		}
		else {
			return(Graph::edgeDefined(passedFirstNode, passedSecondNode) || Graph::edgeDefined(passedSecondNode, passedFirstNode));
		}
	}
	return false;
}

/**
 * Returns the edge weight, or EDGE_UNDEFINED if no such edge is defined.
 */
template <typename T>
double Graph<T>::getEdgeWeight(int passedFirstNode, int passedSecondNode) {
	if (validateEdge(passedFirstNode, passedSecondNode)) {
		return(edgeMatrix[passedFirstNode][passedSecondNode]);
	}
	return EDGE_UNDEFINED;
}

/**
 * Calculates and returns the density of this Graph.
 */
template <typename T>
double Graph<T>::getDensity() {
	double effectiveEdges = edges;
	if (!directed) {
		effectiveEdges *= 2;
	}
	return (effectiveEdges / (vertices * (vertices - 1)));
}

/**
 * Returns whether this Graph is considered directed.
 */
template <typename T>
bool Graph<T>::isDirected() {
	return directed;
}

/**
 * Returns the average weight of all the edges of this Graph.
 */
template <typename T>
double Graph<T>::getAverageWeight() {
	return (edgeSum / vertices);
}

/* Manipulators */

/**
 * Adds an edge to the Graph between the two nodes, with the passed weight.
 *
 * If the Graph is directed, adds an edge in the direction passedFirstNode->passedSecondNode.
 */
template <typename T>
void Graph<T>::addEdge(int passedFirstNode, int passedSecondNode, double passedEdgeWeight) {
	if(Graph::validateEdge(passedFirstNode, passedSecondNode)) {
		Graph::setEdge(passedFirstNode, passedSecondNode, passedEdgeWeight);
		edges += 1;
		edgeSum += passedEdgeWeight;
	}
}

/**
 * Convenience method to add an edge of weight 1 to the Graph.
 *
 * If the Graph is directed, adds an edge in the direction passedFirstNode->passedSecondNode.
 */
template <typename T>
void Graph<T>::addEdge(int passedFirstNode, int passedSecondNode) {
	Graph::addEdge(passedFirstNode, passedSecondNode, 1);
}

/**
 * Removes the indicated edge from the Graph.
 *
 * If the Graph is directed, removes the edge in the direction passedFirstNode->passedSecondNode.
 */
template <typename T>
void Graph<T>::removeEdge(int passedFirstNode, int passedSecondNode) {
	if(Graph::validateEdge(passedFirstNode, passedSecondNode)) {
		double weight = edgeMatrix[passedFirstNode][passedSecondNode];
		Graph::setEdge(passedFirstNode, passedSecondNode, EDGE_UNDEFINED);
		edges -= 1;
		edgeSum -= weight;
	}
}

/**
 * Returns the value of the node specified by the passed index, or nullptr if no such node exists.
 */
template <typename T>
T Graph<T>::getNode(int passedIndex) {
	if (Graph::validateIndex(passedIndex)) {
		return nodeValues[passedIndex];
	}
	return 0;
}

/**
 * Adds the value to the Graph at the passed index.
 */
template <typename T>
void Graph<T>::setNode(T passedValue, int passedIndex) {
	if (Graph::validateIndex(passedIndex)) {
		nodeValues[passedIndex] = passedValue;
	}
}


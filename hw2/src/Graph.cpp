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
 * Initializes a new Graph with passedNodeQuantity number of nodes; passedIsDirected determines
 * whether or not this is a directed Graph.
 */
Graph::Graph(int passedNodeQuantity, bool passedIsDirected) {
	directed = passedIsDirected;
	vertices = 0;
	edges = 0;
	values = new double[passedNodeQuantity][passedNodeQuantity];
	for(int ii = 0; ii < passedNodeQuantity; ii += 1) {
		for (int jj = 0; jj < passedNodeQuantity; jj += 1) {
			values[ii][jj] = EDGE_UNDEFINED;
		}
	}
}

Graph::~Graph() {
	// Free stored values
	for (int ii = 0; ii < vertices; ii += 1) {
		delete[] values[ii];
	}
	delete[] values;
}

/* Internal Methods */
bool Graph::validateIndex(int passedIndex) {
	return (passedIndex > 0);
}

bool Graph::validateEdge(int passedFirstIndex, int passedSecondIndex) {
	return(Graph::validateIndex(passedFirstIndex) && Graph::validateIndex(passedSecondIndex));
}

bool Graph::edgeDefined(int passedFirstIndex, int passedSecondIndex) {
	return(values[passedFirstIndex][passedSecondIndex] >= 0);
}

void Graph::setEdge(int passedFirstNode, int passedSecondNode, double passedEdgeWeight) {
	values[passedFirstNode][passedSecondNode] = passedEdgeWeight;
	if (!directed) {
		values[passedSecondNode][passedFirstNode] = passedEdgeWeight;
	}
}

/* Accessors */

/**
 * Returns the number of vertices (nodes) in this graph.
 */
int Graph::getVertexCount() {
	return vertices;
}

/**
 * Returns the number of edges in this graph.
 */
int Graph::getEdgeCount() {
	return edges;
}

/**
 * Returns whether or not the Graph has an edge between the first and second nodes.
 *
 * If the Graph is directed, checks for edge in the direction passedFirstNode -> passedSecondNode.
 */
bool Graph::hasEdge(int passedFirstNode, int passedSecondNode) {
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
double Graph::getEdgeWeight(int passedFirstNode, int passedSecondNode) {
	if (validateEdge(passedFirstNode, passedSecondNode)) {
		return(values[passedFirstNode][passedSecondNode]);
	}
	return EDGE_UNDEFINED;
}

/**
 * Calculates and returns the density of this Graph.
 */
double Graph::getDensity() {
	double effectiveEdges = edges;
	if (!directed) {
		effectiveEdges *= 2;
	}
	return (effectiveEdges / (vertices * (vertices - 1)));
}

/**
 * Returns whether this Graph is considered directed.
 */
bool Graph::isDirected() {
	return directed;
}

/* Manipulators */

/**
 * Adds an edge to the Graph between the two nodes, with the passed weight.
 *
 * If the Graph is directed, adds an edge in the direction passedFirstNode->passedSecondNode.
 */
void Graph::addEdge(int passedFirstNode, int passedSecondNode, double passedEdgeWeight) {
	if(Graph::validateEdge(passedFirstNode, passedSecondNode)) {
		Graph::setEdge(passedFirstNode, passedSecondNode, passedEdgeWeight);
	}
}

/**
 * Convenience method to add an edge of weight 1 to the Graph.
 *
 * If the Graph is directed, adds an edge in the direction passedFirstNode->passedSecondNode.
 */
void Graph::addEdge(int passedFirstNode, int passedSecondNode) {
	if(Graph::validateEdge(passedFirstNode, passedSecondNode)) {
		Graph::setEdge(passedFirstNode, passedSecondNode, 1);
	}
}

/**
 * Removes the indicated edge from the Graph.
 *
 * If the Graph is directed, removes the edge in the direction passedFirstNode->passedSecondNode.
 */
void Graph::removeEdge(int passedFirstNode, int passedSecondNode) {
	if(Graph::validateEdge(passedFirstNode, passedSecondNode)) {
		Graph::setEdge(passedFirstNode, passedSecondNode, EDGE_UNDEFINED);
	}
}


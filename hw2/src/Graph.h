/*********************************************************************
 * Graph.h
 * 
 * Assignment: hw2
 *
 * 10-2017
 *
 * Header file for Graph class.
 *********************************************************************/

#ifndef GRAPH_H_HW2
#define GRAPH_H_HW2

#define EDGE_UNDEFINED -1

template <typename T>
class Graph {
private:
	// Internal Methods
	bool validateIndex(int);
	bool validateEdge(int, int);
	bool edgeDefined(int, int);
	void setEdge(int, int, double);
protected:
	int vertices;
	int edges;
	bool directed;
	double** edgeMatrix;
	double edgeSum;
	T nodeValues[];
public:
	// Constructor-Destructor
	Graph(int, bool);
	virtual ~Graph();

	// Accessors
	int getVertexCount();
	int getEdgeCount();
	bool adjacent(int, int);
	double getEdgeWeight(int, int);
	double getDensity();
	bool isDirected();
	double getAverageWeight();

	// Manipulators
	void addEdge(int, int, double);
	void addEdge(int, int);
	void removeEdge(int, int);
};

#endif /* GRAPH_H_ */

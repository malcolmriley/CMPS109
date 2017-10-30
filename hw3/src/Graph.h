/*********************************************************************
 * Graph.h
 * 
 * Assignment: hw3
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
 *
 * Typically function implementations are left out of header files.
 * Since this is a template class, the implementation of the function
 * is required to be in the same file as it is declared.
 *********************************************************************/

#ifndef GRAPH_H_HW2
#define GRAPH_H_HW2

#define EDGE_UNDEFINED -1

template <typename T>
class Graph {
private:
	// Internal Methods
	void init(int size, bool directed);
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
	T* vertexValues;
public:
	// Constructor-Destructor
	Graph<T>(int, bool, T[]);
	Graph<T>(int, bool);
	virtual ~Graph();

	// Accessors
	int getVertexCount();
	int getEdgeCount();
	bool adjacent(int, int);
	double getEdgeWeight(int, int);
	double getDensity();
	bool isDirected();
	double getAverageWeight();
	T* getVertex(int);
	void setVertex(T, int);

	// Manipulators
	void addEdge(int, int, double);
	void addEdge(int, int);
	void removeEdge(int, int);
};

template <typename T>
void Graph<T>::init(int passedVertexQuantity, bool passedIsDirected) {
	// Initialize internal values
	directed = passedIsDirected;
	vertices = passedVertexQuantity;
	edgeSum = 0;
	edges = 0;
	edgeMatrix = new double*[passedVertexQuantity];
	vertexValues = new T[passedVertexQuantity];

	// Initialize "dynamic" 2D array
	for (int ii = 0; ii < passedVertexQuantity; ii += 1) {
		edgeMatrix[ii] = new double[passedVertexQuantity];
		for (int jj = 0; jj < passedVertexQuantity; jj += 1) {
			edgeMatrix[ii][jj] = EDGE_UNDEFINED;
		}
	}
}

template <typename T>
Graph<T>::Graph(int passedVertexQuantity, bool passedIsDirected) {
	Graph<T>::init(passedVertexQuantity, passedIsDirected);
}

/**
 * Initializes a new Graph with passedVertexQuantity number of vertices; passedIsDirected determines
 * whether or not this is a directed Graph.
 */
template <typename T>
Graph<T>::Graph(int passedVertexQuantity, bool passedIsDirected, T passedVertexArray[]) {
	Graph<T>::init(passedVertexQuantity, passedIsDirected);

	// Initialize vertex value array
	if (passedVertexArray) {
		for (int ii = 0; ii < passedVertexQuantity; ii += 1) {
			vertexValues[ii] = passedVertexArray[ii];
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
	delete[] vertexValues;
}

/* Internal Methods */
template <typename T>
bool Graph<T>::validateIndex(int passedIndex) {
	return (passedIndex >= 0);
}

template <typename T>
bool Graph<T>::validateEdge(int passedFirstIndex, int passedSecondIndex) {
	return(Graph::validateIndex(passedFirstIndex) && Graph::validateIndex(passedSecondIndex));
}

template <typename T>
bool Graph<T>::edgeDefined(int passedFirstIndex, int passedSecondIndex) {
	return(edgeMatrix[passedFirstIndex][passedSecondIndex] >= 0);
}

template <typename T>
void Graph<T>::setEdge(int passedFirstVertex, int passedSecondVertex, double passedEdgeWeight) {
	edgeMatrix[passedFirstVertex][passedSecondVertex] = passedEdgeWeight;
	if (!directed) {
		edgeMatrix[passedSecondVertex][passedFirstVertex] = passedEdgeWeight;
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
 * Returns whether or not the Graph has an edge between the first and second vertices.
 *
 * If the Graph is directed, checks for edge in the direction passedFirstVertex -> passedSecondVertex.
 */
template <typename T>
bool Graph<T>::adjacent(int passedFirstVertex, int passedSecondVertex) {
	if (Graph::validateEdge(passedFirstVertex, passedSecondVertex)) {
		if (directed) {
			return(Graph::edgeDefined(passedFirstVertex, passedSecondVertex));
		}
		else {
			return(Graph::edgeDefined(passedFirstVertex, passedSecondVertex) || Graph::edgeDefined(passedSecondVertex, passedFirstVertex));
		}
	}
	return false;
}

/**
 * Returns the edge weight, or EDGE_UNDEFINED if no such edge is defined.
 */
template <typename T>
double Graph<T>::getEdgeWeight(int passedFirstVertex, int passedSecondVertex) {
	if (validateEdge(passedFirstVertex, passedSecondVertex)) {
		return(edgeMatrix[passedFirstVertex][passedSecondVertex]);
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
 * Adds an edge to the Graph between the two vertices, with the passed weight.
 *
 * If the Graph is directed, adds an edge in the direction passedFirstVertex->passedSecondVertex.
 */
template <typename T>
void Graph<T>::addEdge(int passedFirstVertex, int passedSecondVertex, double passedEdgeWeight) {
	if(Graph::validateEdge(passedFirstVertex, passedSecondVertex)) {
		Graph::setEdge(passedFirstVertex, passedSecondVertex, passedEdgeWeight);
		edges += 1;
		edgeSum += passedEdgeWeight;
	}
}

/**
 * Convenience method to add an edge of weight 1 to the Graph.
 *
 * If the Graph is directed, adds an edge in the direction passedFirstVertex->passedSecondVertex.
 */
template <typename T>
void Graph<T>::addEdge(int passedFirstVertex, int passedSecondVertex) {
	Graph::addEdge(passedFirstVertex, passedSecondVertex, 1);
}

/**
 * Removes the indicated edge from the Graph.
 *
 * If the Graph is directed, removes the edge in the direction passedFirstVertex->passedSecondVertex.
 */
template <typename T>
void Graph<T>::removeEdge(int passedFirstVertex, int passedSecondVertex) {
	if(Graph::validateEdge(passedFirstVertex, passedSecondVertex)) {
		double weight = edgeMatrix[passedFirstVertex][passedSecondVertex];
		Graph::setEdge(passedFirstVertex, passedSecondVertex, EDGE_UNDEFINED);
		edges -= 1;
		edgeSum -= weight;
	}
}

/**
 * Returns the value of the vertex specified by the passed index, or nullptr if no such vertex exists.
 */
template <typename T>
T* Graph<T>::getVertex(int passedIndex) {
	if (Graph::validateIndex(passedIndex)) {
		return &(vertexValues[passedIndex]);
	}
	return nullptr;
}

/**
 * Adds the value to the Graph at the passed index.
 */
template <typename T>
void Graph<T>::setVertex(T passedValue, int passedIndex) {
	if (Graph::validateIndex(passedIndex)) {
		vertexValues[passedIndex] = passedValue;
	}
}

#endif /* GRAPH_H_ */

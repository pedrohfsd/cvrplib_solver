#ifndef VERTEX_H_
#define VERTEX_H_

#include "Edge.h"

class Vertex {
public:
	std::vector<Edge> edges;
	int demand;
	int edgeCount;
	Vertex();
	Vertex(int edgeCount);
	void init(int edgeCount);
};

#endif  // VERTEX_H_
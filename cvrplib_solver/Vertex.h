#ifndef VERTEX_H_
#define VERTEX_H_

#include "Edge.h"
#include <vector>

class Vertex {
public:
	std::vector<Edge> edges;
	int demand;
	double coordinate[2];
	Vertex();
};

#endif  // VERTEX_H_
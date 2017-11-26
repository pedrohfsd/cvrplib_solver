#ifndef Data_H_
#define Data_H_

#include <vector>

#include "Edge.h"
#include "Vertex.h"

class Data {
public:
	std::vector<Vertex> vertices;
	std::vector<Edge> edges;
	int vehicle_capacity;
	int vehicles;
	int depot;
	Data();

};

#endif  // Data_H_
#include <memory>
#include <vector>

#include "Edge.h"
#include "Vertex.h"

using namespace std;

Vertex::Vertex() {
	init(0);
};

Vertex::Vertex(int edgeCount) {
	init(edgeCount);
};

void Vertex::init(int edgeCount) {
	edges.clear();
	Vertex::edgeCount = edgeCount;
	for (int i = 0; i < edgeCount; i++) edges.push_back(Edge());
};
#ifndef CUT_H
#define CUT_H

#include <vector>
#include "Vertex.h"

class Cut {
public:
	std::vector<Edge> edges;
	std::vector<int> vertices;
	int sense = 1;
	int ks; // minimum number of cars needed
	int rhs;
	static const int greater_equal = 1;
	static const int equal = 0;
	static const int lower_equal = -1;
	Cut();
	Cut(int rhs, int sense);
};

#endif //CUT_H

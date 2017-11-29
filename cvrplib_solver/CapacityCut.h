#ifndef CAPACITYCUT_H
#define CAPACITYCUT_H

#include "Cut.h"

class CapacityCut : public Cut
{
public:
	CapacityCut();

	int ks;
	std::vector<int> vertices;

	void build();
};

#endif //CAPACITYCUT_H
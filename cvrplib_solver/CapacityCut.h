#ifndef CAPACITYCUT_H
#define CAPACITYCUT_H

#include "Cut.h"

class CapacityCut : public Cut
{
public:
	CapacityCut();
	CapacityCut(double rhs, Sense sense);

	int ks;
};

#endif //CAPACITYCUT_H
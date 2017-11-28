#ifndef ABSTRACTCUTSEPARATION_H
#define ABSTRACTCUTSEPARATION_H

#include "Data.h"
#include "Cut.h"

class AbstractCutSeparation {
public:
	AbstractCutSeparation(Data& data);
	//~AbstractCutSeparation();
public:
	Data& data;
	int best;
	std::vector<Cut> cuts;
};

#endif //ABSTRACTCUTSEPARATION_H

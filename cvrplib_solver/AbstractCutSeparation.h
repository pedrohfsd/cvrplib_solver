#ifndef ABSTRACTCUTSEPARATION_H
#define ABSTRACTCUTSEPARATION_H

#include "Data.h"
#include "Cut.h"

class AbstractCutSeparation {
public:
	AbstractCutSeparation(Data& data);
	//~AbstractCutSeparation();
	Cut getCut();
protected:
	Data& data;
	int best;
	std::vector<Cut> cuts;
	void addCutEdges(Cut& cut);
};

#endif //ABSTRACTCUTSEPARATION_H

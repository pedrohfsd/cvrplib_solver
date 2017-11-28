#ifndef LYSGAARDCUTSEPARATION_H
#define LYSGAARDCUTSEPARATION_H

#include "AbstractCutSeparation.h"

#include "cnstrmgr.h"

class LysgaardCutSeparation : public AbstractCutSeparation
{
public:
	explicit LysgaardCutSeparation(Data& data);
	virtual ~LysgaardCutSeparation();

	void init(const std::vector<double>& solution);

protected:
	CnstrMgrPointer MyCutsCMP;
	CnstrMgrPointer MyOldCutsCMP;
	std::vector<int> demands;

	std::vector<int> tail;
	std::vector<int> head;
	std::vector<double> x;
};

#endif

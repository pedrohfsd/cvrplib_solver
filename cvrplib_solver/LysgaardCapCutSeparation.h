#ifndef LYSGAARDCAPCUTSEPARATION_H
#define LYSGAARDCAPCUTSEPARATION_H

#include "LysgaardCutSeparation.h"

class LysgaardCapCutSeparation : public LysgaardCutSeparation
{
public:
	LysgaardCapCutSeparation(Data& data);
	bool run(const std::vector<double>& solution);
};

#endif

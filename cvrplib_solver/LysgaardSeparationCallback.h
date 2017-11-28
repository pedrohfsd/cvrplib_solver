#ifndef LYSGAARDSEPARATIONCALLBACK_H
#define LYSGAARDSEPARATIONCALLBACK_H

#include <ilcplex/ilocplex.h>
#include "Data.h"

class LysgaardSeparationCallback : public IloCplex::LazyConstraintCallbackI
{
public:
	LysgaardSeparationCallback(IloEnv env, IloIntVarArray x, Data& data);

	void main();

	IloCplex::CallbackI* duplicateCallback() const;
private:
	IloIntVarArray x;
	Data& data;
};

#endif

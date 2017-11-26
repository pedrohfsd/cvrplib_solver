#ifndef LYSGAARDSEPARATIONCALLBACK_H
#define LYSGAARDSEPARATIONCALLBACK_H

#include <ilcplex/ilocplex.h>

class LysgaardSeparationCallback : public IloCplex::LazyConstraintCallbackI
{
public:
	LysgaardSeparationCallback(IloEnv env, IloIntVarArray x);

	void main();

	IloCplex::CallbackI* duplicateCallback() const;
private:
	IloIntVarArray x;
};

#endif

#include "LysgaardSeparationCallback.h"

#include <vector>

#include "LysgaardCapCutSeparation.h"
#include "Cut.h"

#define EPS 1e-7

using namespace std;

LysgaardSeparationCallback::LysgaardSeparationCallback(IloEnv env, IloIntVarArray x, Data& data):data(data), IloCplex::LazyConstraintCallbackI(env), x(x) {}

void LysgaardSeparationCallback::main()
{
	IloNumArray vals(getEnv(), x.getSize());
	getValues(vals, x);

	vector<double> solution;
	for (int e = 0; e < vals.getSize(); e++)
		solution.push_back(vals[e]);

	LysgaardCapCutSeparation sep(data);
	if (sep.run(solution))
	{
		const auto& cut = sep.getCut();

		IloExpr expr(getEnv());
		for (int e = 0; e < (int)cut.coeff.size(); e++) {
			expr += x[cut.coeff[e].first];
		}
		switch (cut.sense)
		{
		case Cut::equal: add(IloRange(expr == cut.rhs)); break;
		case Cut::greater_equal: add(IloRange(expr >= cut.rhs)); break;
		case Cut::lower_equal: add(IloRange(expr <= cut.rhs)); break;
		}
		expr.end();
	}
}

IloCplex::CallbackI* LysgaardSeparationCallback::duplicateCallback() const
{
	return (new (getEnv()) LysgaardSeparationCallback(*this));
}

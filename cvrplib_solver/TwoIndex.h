#ifndef TWO_INDEX_H_
#define TWO_INDEX_H_

#include <ilcplex/ilocplex.h>

#include "Data.h"

class TwoIndex {
public:
	const std::string PROBLEM = "TwoIndex";
	TwoIndex();
	void run(Data&);

private:
	void addDecisionVariables(IloModel model, IloIntVarArray x, IloRangeArray con, Data& data);
	void addDepotConstraints(IloModel model, IloIntVarArray x, IloRangeArray con, Data& data);
	void addObjectiveFunction(IloModel model, IloIntVarArray x, IloRangeArray con, Data& data);
	void addCustomerConstraints(IloModel model, IloIntVarArray x, IloRangeArray con, Data& data);
	void print(IloCplex cplex, IloIntVarArray x, IloRangeArray con, Data& data);
};

#endif TWO_INDEX_H_

#include "TwoIndex.h"
#include "LysgaardSeparationCallback.h"

#include <ilcplex/ilocplex.h>
#include <string>

#define EPS 1e-5

using namespace std;

TwoIndex::TwoIndex() {}

void TwoIndex::run(Data& data) {
	IloEnv   env;
	IloModel model(env, PROBLEM.c_str());
	IloIntVarArray x(env);
	IloRangeArray cons(env);

	addDecisionVariables(model, x, cons, data);
	addObjectiveFunction(model, x, cons, data);
	addDepotConstraints(model, x, cons, data);
	addCustomerConstraints(model, x, cons, data);

	model.add(cons);
	IloCplex cplex(model);
	cplex.exportModel((PROBLEM + ".lp").c_str());
	cplex.setParam(IloCplex::Threads, 1);
	cplex.use(new LysgaardSeparationCallback<IloCplex::LazyConstraintCallbackI>(env, x, data));
	cplex.use(new LysgaardSeparationCallback<IloCplex::UserCutCallbackI>(env, x, data));

	if (!cplex.solve()) {
#ifdef _DEBUG
		return;
#else
		throw exception("Failed to optimize IP");
#endif
	}

	print(cplex, x, cons, data);
}

void TwoIndex::addDecisionVariables(IloModel model, IloIntVarArray x, IloRangeArray con, Data& data) {
	IloEnv env = model.getEnv();
	for (int i = 0; i < data.edges.size(); i++) {
		if (data.edges[i].to == data.depot) x.add(IloIntVar(env, 0, 2, ("x_" + to_string(data.edges[i].from) + "_" + to_string(data.edges[i].to)).c_str()));
		else x.add(IloBoolVar(env, ("x_" + to_string(data.edges[i].from) + "_" + to_string(data.edges[i].to)).c_str()));
	}
};

void TwoIndex::addDepotConstraints(IloModel model, IloIntVarArray x, IloRangeArray con, Data& data) {
	IloExpr expr(model.getEnv());
	for (int e = 0; e < data.vertices[data.depot].edges.size(); e++) {
		expr += x[data.vertices[data.depot].edges[e].id];
	}
	con.add(IloRange(expr == 2*data.vehicles));
	expr.end();
};

void TwoIndex::addCustomerConstraints(IloModel model, IloIntVarArray x, IloRangeArray con, Data& data) {
	IloEnv env = model.getEnv();
	for (int v = 0; v < data.vertices.size(); v++) {
		if (v == data.depot) continue;
		IloExpr expr(env);
		for (int e = 0; e < data.vertices[v].edges.size(); e++) {
			expr += x[data.vertices[v].edges[e].id];
		}
		con.add(IloRange(expr == 2));
		expr.end();
	}
};

void TwoIndex::addObjectiveFunction(IloModel model, IloIntVarArray x, IloRangeArray con, Data& data) {
	IloEnv env = model.getEnv();
	IloExpr expr(env);
	for (int e = 0; e < data.edges.size(); e++) {
		Edge edge = data.edges[e];
		expr += edge.cost * x[edge.id];
	}
	model.add(IloMinimize(env, expr));
	expr.end();
};

void TwoIndex::print(IloCplex cplex, IloIntVarArray x, IloRangeArray cons, Data& data) {
	IloEnv env = cplex.getEnv();
	IloNumArray vals(env);

	env.out() << "Solution status = " << cplex.getStatus() << endl;
	env.out() << "Solution value  = " << cplex.getObjValue() << endl;
	cplex.getValues(vals, x);

	vector<vector<double>> adj;
	adj.resize(data.vertices.size());
	for (int i = 0; i < data.vertices.size(); i++) adj[i].resize(data.vertices.size());
	int pos = 0;
	for (int i = 0; i < data.vertices.size(); i++)
	{
		for (int j = 0; j < i; j++)
		{
			adj[i][j] = vals[pos++];
		}
	}
	cout << "solution matrix:" << endl;
	for (int i = 0; i < data.vertices.size(); i++)
	{
		for (int j = 0; j < data.vertices.size(); j++)
		{
			cout << adj[i][j] << ",";
		}
		cout << endl;
	}
	/*env.out() << "Values        = " << vals << endl;
	cplex.getSlacks(vals, cons);
	env.out() << "Slacks        = " << vals << endl;*/
	/*cplex.getDuals(vals, con);
	env.out() << "Duals         = " << vals << endl;
	cplex.getReducedCosts(vals, var);
	env.out() << "Reduced Costs = " << vals << endl;*/
};
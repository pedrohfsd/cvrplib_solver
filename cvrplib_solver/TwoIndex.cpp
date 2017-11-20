#include "TwoIndex.h"

#include <ilcplex/ilocplex.h>
#include <string>

#define EPS 1e-5

using namespace std;

TwoIndex::TwoIndex() {}

void TwoIndex::run(Data& data) {
	IloEnv   env;
	IloModel model(env, PROBLEM.c_str());
	IloNumVarArray x(env);
	IloRangeArray cons(env);

	addDecisionVariables(model, x, cons, data);
	addObjectiveFunction(model, x, cons, data);
	addDepotConstraints(model, x, cons, data);
	addClientConstraints(model, x, cons, data);

	model.add(cons);
	IloCplex cplex(model);
	cplex.exportModel((PROBLEM + ".lp").c_str());
	cplex.setParam(IloCplex::Threads, 1);

	if (!cplex.solve()) throw exception("Failed to optimize IP");

	print(cplex, x, cons);
}

void TwoIndex::addDecisionVariables(IloModel model, IloNumVarArray x, IloRangeArray con, Data& data) {
	IloEnv env = model.getEnv();
	for (int i = 0; i < data.dimension; i++) {
		for (int j = 0; j < data.dimension; j++) {
			if (i == j) x.add(IloIntVar(env, 0, 0, ("x_" + to_string(i) + "_" + to_string(j)).c_str()));
			else x.add(IloBoolVar(env, ("x_"+ to_string(i) +"_"+ to_string(j)).c_str()));
		}
	}
};

void TwoIndex::addDepotConstraints(IloModel model, IloNumVarArray x, IloRangeArray con, Data& data) {
	IloExpr expr(model.getEnv());
	for (int i = 0; i < data.dimension; i++) {
		expr += x[data.vertices[0].edges[i].id];
		expr += x[data.vertices[i].edges[0].id];
	}
	con.add(IloRange(expr == 2*4));
	expr.end();
};

void TwoIndex::addClientConstraints(IloModel model, IloNumVarArray x, IloRangeArray con, Data& data) {
	IloEnv env = model.getEnv();
	size_t n = data.vertices.size();
	for (int i = 1; i < n; i++) {
		IloExpr expr(env);
		for (int j = 0; j < n; j++)
			expr += x[data.vertices[i].edges[j].id];
		con.add(IloRange(expr == 1));
		expr.end();

		expr = IloExpr(env);
		for (int j = 0; j < n; j++)
			expr += x[data.vertices[j].edges[i].id];
		con.add(IloRange(expr == 1));
		expr.end();
	}
};

void TwoIndex::addObjectiveFunction(IloModel model, IloNumVarArray var, IloRangeArray con, Data& data) {
	IloEnv env = model.getEnv();
	IloExpr expr(env);
	for (int i = 0; i < data.dimension; i++) {
		for (int j = 0; j < data.dimension; j++) {
			Edge edge = data.vertices[i].edges[j];
			expr += edge.cost * var[edge.id];
		}
	}
	model.add(IloMinimize(env, expr));
	expr.end();
};

void TwoIndex::print(IloCplex cplex, IloNumVarArray x, IloRangeArray cons) {
	IloEnv env = cplex.getEnv();
	IloNumArray vals(env);

	env.out() << "Solution status = " << cplex.getStatus() << endl;
	env.out() << "Solution value  = " << cplex.getObjValue() << endl;
	cplex.getValues(vals, x);
	env.out() << "Values        = " << vals << endl;
	cplex.getSlacks(vals, cons);
	env.out() << "Slacks        = " << vals << endl;
	/*cplex.getDuals(vals, con);
	env.out() << "Duals         = " << vals << endl;
	cplex.getReducedCosts(vals, var);
	env.out() << "Reduced Costs = " << vals << endl;*/
};
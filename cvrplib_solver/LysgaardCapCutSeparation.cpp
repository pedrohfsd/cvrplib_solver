#include "LysgaardCapCutSeparation.h"

#include <iostream>
#include <cmath>
#include <vector>

#include "Vertex.h"
#include "Edge.h"

#include "CapacityCut.h"

#include "capsep.h"

#define EPS 1e-7

using namespace std;

LysgaardCapCutSeparation::LysgaardCapCutSeparation(Data& data):LysgaardCutSeparation(data) {
}

bool LysgaardCapCutSeparation::run(const vector<double>& solution)
{
	init(solution);

	char IntegerAndFeasible;
	double MaxViolation;
	CAPSEP_SeparateCapCuts((int)data.vertices.size() - 1, &demands[0], data.vehicle_capacity, (int)x.size() - 1, &tail[0], &head[0], &x[0],
		MyOldCutsCMP, 10, 0.0001, &IntegerAndFeasible, &MaxViolation, MyCutsCMP); // call lysgaard routines

	bool ok = false;
	if ((!IntegerAndFeasible) && (MyCutsCMP->Size > 0))
	{
		int error = 0;
		double bestViolation = 0.0;
		for (int i = 0; i < MyCutsCMP->Size; i++)
		{
			CapacityCut cut;
			int S = MyCutsCMP->CPL[i]->IntListSize;
			int demand = 0;
			for (int j = 1; j <= MyCutsCMP->CPL[i]->IntListSize; j++) {
				if (MyCutsCMP->CPL[i]->IntList[j] == data.vertices.size())
				{
					cout << "Lysgaard CapCut Separation Error: depot is in the cut!" << endl;
					exit(-1);
				}
				else
				{
					cut.vertices.push_back(MyCutsCMP->CPL[i]->IntList[j]);
					demand += data.vertices[MyCutsCMP->CPL[i]->IntList[j]].demand;
				}
			}

			cut.ks = (int)ceil(demand / (double)data.vehicle_capacity - 1e-7); // minimum number of vehicles
			cut.sense = Cut::lower_equal;
			cut.rhs = S - cut.ks;

			addCutEdges(cut);

			cuts.push_back(cut);
			/*double violation = calculateViolation(cuts.back(), solution);
			if (violation <= EPS)
				error++;
			else if (violation > bestViolation)
			{
				best = (int)cuts.size() - 1;
				bestViolation = violation;
			}*/
		}

		if (error > 0)
		{
			cout << "Lysgaard CapCut Separation Error: " << error << " cuts are not violated!" << endl;
			exit(-1);
		}

		if (error < MyCutsCMP->Size) ok = true;

		for (int i = 0; i < MyCutsCMP->Size; i++)
			CMGR_MoveCnstr(MyCutsCMP, MyOldCutsCMP, i, 0);
		MyCutsCMP->Size = 0;
	}

	return ok;
}

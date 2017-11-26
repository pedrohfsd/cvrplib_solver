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

bool LysgaardCapCutSeparation::run(const vector<double>& solution)
{
	init(solution); // builds solution adjacency matrix, tail, head and x vectors.

	char IntegerAndFeasible;
	double MaxViolation;
	CAPSEP_SeparateCapCuts((int)data.vertices.size() - 1, &demands[0], data.vehicle_capacity, (int)x.size() - 1, &tail[0], &head[0], &x[0],
		MyOldCutsCMP, 10, 0.0001, &IntegerAndFeasible, &MaxViolation, MyCutsCMP); // call lysgaard routines

	bool ok = false;
	if ((!IntegerAndFeasible) && (MyCutsCMP->Size > 0))
	{
		int error = 0;
		double bestViolation = 0.0;
		for (int i = 0; i < MyCutsCMP->Size; i++) // for each connected component
		{
			CapacityCut cut;

			int S = MyCutsCMP->CPL[i]->IntListSize;
			int demand = 0;
			for (int j = 1; j <= MyCutsCMP->CPL[i]->IntListSize; j++)
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

			cut.ks = (int)ceil(demand / (double)data.vehicle_capacity - 1e-7); // minimum number of vehicles

			if (S <= (int)data.vertices.size() / 2) // if small use the complementary cut
			{
				cuts.push_back(Cut(S - cut.ks, Cut::lower_equal));
				addCutEdges(cuts.back(), cut.vertices_map, inner_edge, 1.0);
			}
			else
			{
				cuts.push_back(Cut((int)data.vertices.size() - 1 - S - cut.ks, Cut::lower_equal));
				addCutEdges(cuts.back(), cut.vertices_map, outer_edge, 1.0);
				addCutEdges(cuts.back(), cut.vertices_map, outer_depot_edge, 0.5);
				addCutEdges(cuts.back(), cut.vertices_map, inner_depot_edge, -0.5);
			}

			double violation = calculateViolation(cuts.back(), solution);
			if (violation <= EPS)
				error++;
			else if (violation > bestViolation)
			{
				best = (int)cuts.size() - 1;
				bestViolation = violation;
			}
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

#include "LysgaardCutSeparation.h"

#include "Data.h"
#include "Vertex.h"
#include "Edge.h"
#include "Parameters.h"

#define EPS 1e-7

using namespace std;

LysgaardCutSeparation::LysgaardCutSeparation(Data& data):AbstractCutSeparation(data)
{
	CMGR_CreateCMgr(&MyCutsCMP, 10);
	CMGR_CreateCMgr(&MyOldCutsCMP, 10);
	for (int v = 0; v < (int)data.vertices.size(); v++)
		demands.push_back(data.vertices[v].demand);
}

LysgaardCutSeparation::~LysgaardCutSeparation()
{
	CMGR_FreeMemCMgr(&MyCutsCMP);
	CMGR_FreeMemCMgr(&MyOldCutsCMP);
}

void LysgaardCutSeparation::init(const vector<double>& solution)
{
	best = -1;
	cuts.clear();
	MyCutsCMP->Size = 0;

	vector<vector<double>> adjMat(data.vertices.size());
	for (int i = 0; i < (int)data.vertices.size(); i++)
		adjMat[i].resize(data.vertices.size(), 0.0);

	for (int e = 0; e < (int)solution.size(); e++)
		if (solution[e] > EPS)
		{
			int from = data.edges[e].from;
			int to = data.edges[e].to;

			adjMat[from][to] += solution[e];
			adjMat[to][from] += solution[e];
		}

	tail.assign(1, 0);
	head.assign(1, 0);
	x.assign(1, 0.0);

	for (int i = 0; i < (int)adjMat.size(); i++)
		for (int j = i + 1; j < (int)adjMat[i].size(); j++)
			if (adjMat[i][j] > EPS)
			{
				tail.push_back(i);
				if (tail.back() == 0) tail.back() = (int)data.vertices.size();
				head.push_back(j);
				if (head.back() == 0) head.back() = (int)data.vertices.size();
				x.push_back(adjMat[i][j]);
			}
}

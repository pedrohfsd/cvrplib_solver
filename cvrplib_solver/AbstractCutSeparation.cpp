#include "AbstractCutSeparation.h"
#include <vector>
#include <unordered_set>

using namespace std;

AbstractCutSeparation::AbstractCutSeparation(Data& data):data(data){
}

Cut AbstractCutSeparation::getCut() {
	return cuts[0];
}

void AbstractCutSeparation::addCutEdges(Cut& cut) {
	unordered_set<int> inCut;
	inCut.insert(cut.vertices.begin(), cut.vertices.end());
	for (Edge edge : data.edges) 
		if(inCut.find(edge.from) != inCut.end() && inCut.find(edge.to) != inCut.end())
			cut.coeff.push_back(pair<int, double>(edge.id, edge.cost));
}

//double AbstractCutSeparation::calculateViolation(Cut& cuts, const vector<double>& solution) {
//
//}
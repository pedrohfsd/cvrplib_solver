#include "Data.h"

#include <limits>
#include <queue>
#include <set>
#include <stack>

#define EPS 1e-5

using namespace std;

Data::Data() {
	init(0);
};

Data::Data(int dimension) {
	init(dimension);
};

void Data::init(int dimension) {
	Data::dimension = dimension;
	vertices.clear();
	if (dimension > 0) {
		for (int i = 0; i < dimension; i++) vertices.push_back(Vertex(dimension));
	}
};

void Data::toMatrix(const vector<double>& edgeValues, int expectedRows, vector<vector<double>>& valueMatrix) {
	for (int i = 0; i < edgeValues.size(); i++) valueMatrix[Data::edgeIdToRow(i, expectedRows)][Data::edgeIdToColumn(i, expectedRows)] = edgeValues[i];
};

int Data::edgeIdToRow(int id, int rows) {
	return id / rows;
};

int Data::edgeIdToColumn(int id, int rows) {
	return id % rows;
};

void Data::edgeIdToIndex(int id, int rows, int(&index)[2]) {
	index[0] = id / rows;
	index[1] = id % rows;
};

int Data::indexToId(int rows, int i, int j) {
	return rows*i + j;
};
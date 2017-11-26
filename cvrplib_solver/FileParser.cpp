#include "FileParser.h"

#include <exception>
#include <sstream>

using namespace std;

FileParser::FileParser(Data& data):data(data) {}

void FileParser::parse(const string& filepath) {
	ifstream infile(filepath);
	if (!infile.is_open()) throw exception(("Filepath " + filepath + " does not exists").c_str());
	unordered_map<string, string> spec;
	readSpecification(infile, spec);
	data.vertices.resize(stoi(spec["DIMENSION"]));

	function<int(double[], double[])> dist = NULL;
	if (spec["EDGE_WEIGHT_TYPE"] == "EUC_2D") dist = [this](double f[2], double t[2]) {return euclidianDistance2D(f, t); };
	else if (spec["EDGE_WEIGHT_TYPE"] == "ATT") dist = [this](double f[2], double t[2]) {return attDistance(f, t); };
	else if (spec["EDGE_WEIGHT_TYPE"] == "GEO") dist = [this](double f[2], double t[2]) {return geoDistance(f, t); };
	else if (spec["EDGE_WEIGHT_TYPE"] == "EXPLICIT") dist = NULL;
	else throw exception("Unsupported distance function");

	if (spec["NODE_COORD_SECTION"] == "true") {
		readNodes2D(infile);
		buildDataFromCoord(dist);
	} else if (spec["EDGE_WEIGHT_FORMAT"] == "LOWER_DIAG_ROW") {
		readLowDiagEdges2D(infile, true);
	} else if (spec["EDGE_WEIGHT_FORMAT"] == "LOWER_ROW") {
		readLowDiagEdges2D(infile, false);
	}else throw exception("Unsupported edge data format");
	
	readDemands(infile);
};

int FileParser::attDistance(double node1[2], double node2[2]) {
	double x[2] = { node1[0], node2[0] };
	double y[2] = { node1[1], node2[1] };
	double xd = x[0] - x[1];
	double yd = y[0] - y[1];
	double rij = sqrt((xd*xd + yd*yd) / 10.0);
	int tij = nint(rij);
	if (tij < rij) return tij + 1;
	return tij;
};

void FileParser::buildDataFromCoord(function<int(double[], double[])> dist) {
	int pos = 0;
	for (int i = 0; i < data.vertices.size(); i++) {
		Vertex& from = data.vertices[i];
		for (int j = 0; j < i; j++) {
			Vertex& to = data.vertices[j];
			Edge edge;
			edge.id = pos++;
			edge.from = i;
			edge.to = j;
			edge.cost = dist(from.coordinate, to.coordinate);
			data.edges.push_back(edge);
			from.edges.push_back(edge);
		}
	}
};

int FileParser::euclidianDistance2D(double node1[2], double node2[2]) {
	double x[2] = { node1[0], node2[0] };
	double y[2] = { node1[1], node2[1] };
	double xd = x[0] - x[1];
	double yd = y[0] - y[1];
	return nint(sqrt(xd*xd + yd*yd));
};

int FileParser::geoDistance(double node1[2], double node2[2]) {
	double x[2] = { node1[0], node2[0] };
	double y[2] = { node1[1], node2[1] };
	double PI = 3.141592;
	double latitude[2];
	double longitude[2];

	int deg = (int)x[0];
	double min = x[0] - deg;
	latitude[0] = PI * (deg + 5.0 * min / 3.0) / 180.0;
	deg = (int)y[0];
	min = y[0] - deg;
	longitude[0] = PI * (deg + 5.0 * min / 3.0) / 180.0;

	deg = (int)x[1];
	min = x[1] - deg;
	latitude[1] = PI * (deg + 5.0 * min / 3.0) / 180.0;
	deg = (int)y[1];
	min = y[1] - deg;
	longitude[1] = PI * (deg + 5.0 * min / 3.0) / 180.0;

	double RRR = 6378.388;
	double q1 = cos(longitude[0] - longitude[1]);
	double q2 = cos(latitude[0] - latitude[1]);
	double q3 = cos(latitude[0] + latitude[1]);
	return (int)(RRR * acos(0.5*((1.0 + q1)*q2 - (1.0 - q1)*q3)) + 1.0);
};

int FileParser::nint(double x) {
	return (int)(x + 0.5);
};

void FileParser::readDemands(ifstream& infile) {
	string line;
	infile >> line;
	if (line != "DEMAND_SECTION") throw exception("Error reading demands data");
	for (int i = 0; i < data.vertices.size(); i++) {
		int node;
		infile >> node;
		infile >> data.vertices[node-1].demand;
	}
}

void FileParser::readLowDiagEdges2D(ifstream& infile, bool includeDiagonal) {
	int pos = 0;
	for (int i = 0; i < data.vertices.size(); i++) {
		Vertex& from = data.vertices[i];
		for (int j = 0; includeDiagonal ? j <= i : j < i; j++) {
			Edge edge;
			edge.id = pos++;
			edge.from = i;
			edge.to = j;
			infile >> edge.cost;
			data.edges.push_back(edge);
			from.edges.push_back(edge);
		}
	}
};

void FileParser::readNodes2D(ifstream& infile) {
	string line;
	while (getline(infile, line)) {
		istringstream iss(line);
		int v; double x, y;
		if (!(iss >> v >> x >> y)) break;
		data.vertices[v - 1].coordinate[0] = x;
		data.vertices[v - 1].coordinate[1] = y;
	}
};

void FileParser::readSpecification(ifstream& infile, unordered_map<string, string>& spec) {
	string line;
	while (getline(infile, line)) {
		size_t pos = line.find(":");
		if (pos == string::npos) break;
		line.replace(pos, 1, " ");
		istringstream iss(line);
		string key; string value;
		if (!(iss >> key >> value)) { break; }
		spec[key] = value;
	}
	spec[line] = "true";
};
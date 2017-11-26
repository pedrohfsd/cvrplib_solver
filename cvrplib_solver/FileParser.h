#ifndef FILEPARSER_H_
#define FILEPARSER_H_

#include <fstream>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "Data.h"

class FileParser {
public:
	FileParser(Data& data);
	void parse(const std::string& filepath);

private:
	Data& data;

	int attDistance(double node1[2], double node2[2]);
	void buildDataFromCoord(std::function<int(double[], double[])> dist);
	int euclidianDistance2D(double node1[2], double node2[2]);
	int geoDistance(double node1[2], double node2[2]);
	int nint(double x);
	void readDemands(std::ifstream& infile);
	void readLowDiagEdges2D(std::ifstream& infile, bool includeDiagonal);
	void readNodes2D(std::ifstream& infile);
	void readSpecification(std::ifstream& infile, std::unordered_map<std::string, std::string>& spec);
};

#endif  // FILEPARSER_H_

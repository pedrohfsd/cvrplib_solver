#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "Singleton.h"

#include <string>

class Parameters : public Singleton<Parameters>
{
	friend class Singleton<Parameters>;
public:

	bool read(int argc, char* argv[]);

	enum AlgorithmType
	{
		None,
		Constructive,
		MIPFormulation,
		SimulatedAnnealing,
	};

	enum ObjectiveType
	{
		Cost,
		Makespan,
		Duration,
	};

	enum MIPEmphasis
	{
		Balanced,
		Optimality,
		BestBound,
		Feasibility,
		Hidden,
	};

	std::string file_name;
	AlgorithmType algo;
	ObjectiveType obj;
	std::string obj_name;
	double time_limit;
	bool verbose;
	int bc;

	struct
	{
		int rounding_factor;
		bool wrong_triangle;
	} loader;

	struct
	{
		int reps;
	} cons;

	struct
	{
		bool force_sol;
		bool linear;
		bool linear_only;
		bool cuts;
		bool order;
		MIPEmphasis emphasis;
	} mip;

	struct
	{
		int limit;
	} cut;

private:
	Parameters();

	void setObjectiveName();

	void help(const std::string& command = "");
};

#endif

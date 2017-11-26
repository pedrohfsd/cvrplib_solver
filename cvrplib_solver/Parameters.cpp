#include "Parameters.h"

#include <iostream>

using namespace std;

Parameters::Parameters()
{
	algo = None;
	obj = Cost;
	time_limit = 1e20;
	verbose = false;

	loader.rounding_factor = 0;
	loader.wrong_triangle = false;

	cons.reps = 1;

	mip.linear = false;
	mip.linear_only = false;
	mip.cuts = false;
	mip.order = false;
	mip.force_sol = false;
	mip.emphasis = Balanced;

	cut.limit = 100;
}

bool Parameters::read(int argc, char* argv[])
{
	if (argc < 3)
	{
		help();
		return false;
	}

	file_name = string(argv[1]);
	for (int i = 2; i < argc; i++)
	{
		string command = string(argv[i]);

		if (command == "cons") algo = Constructive;
		else if (command == "mip") algo = MIPFormulation;
		else if (command == "sa") algo = SimulatedAnnealing;
		else if (command == "none") algo = None;

		else if (command == "-oc") obj = Cost;
		else if (command == "-om") obj = Makespan;
		else if (command == "-od") obj = Duration;

		else if (command == "-tl") time_limit = atof(argv[++i]);
		else if (command == "-v") verbose = true;

		else if (command == "-wti") loader.wrong_triangle = true;
		else if (command == "-rf") loader.rounding_factor = atoi(argv[++i]);

		else if (command == "-crep") cons.reps = atoi(argv[++i]);

		else if (command == "-ml") mip.linear = true;
		else if (command == "-mlo") mip.linear = mip.linear_only = true;
		else if (command == "-mcu") mip.cuts = true;
		else if (command == "-mor") mip.order = true;
		else if (command == "-mfs") mip.force_sol = true;
		else if (command == "-mem")
		{
			command = string(argv[++i]);
			if (command == "bal") mip.emphasis = Balanced;
			else if (command == "opt") mip.emphasis = Optimality;
			else if (command == "feas") mip.emphasis = Feasibility;
			else if (command == "bb") mip.emphasis = BestBound;
			else if (command == "hid") mip.emphasis = Hidden;
		}

		else
		{
			help(command);
			return false;
		}
	}

	setObjectiveName();

	return true;
}

void Parameters::setObjectiveName()
{
	switch (obj)
	{
	case Cost: obj_name = "cost"; break;
	case Duration: obj_name = "duration"; break;
	case Makespan: obj_name = "makespan"; break;
	}
}

void Parameters::help(const string& command)
{
	if (!command.empty()) cout << "Invalid command: " << command << endl;
	cout << "Usage: tsptw <instance> <algorithm> <objective> [options]" << endl;
	cout << "	Instance: TSPTW instance file to run." << endl;
	cout << "	Algorithm:" << endl;
	cout << "		cons: Constructive." << endl;
	cout << "		mip: MIP Formulation." << endl;
	cout << "		sa: Simulated Annealing." << endl;
	cout << "	Objective:" << endl;
	cout << "		-oc: Cost minimization (default)." << endl;
	cout << "		-om: Makespan minimization." << endl;
	cout << "		-od: Duration minimization." << endl;
	cout << "	General Options:" << endl;
	cout << "		-v: verbose mode." << endl;
	cout << "		-tl <int>: Time limit." << endl;
	cout << "		-wti: Use wrong triangle inequality calculation." << endl;
	cout << "		-rf <int>: Rounding factor to calculate instance distances." << endl;
	cout << "	Constructive Options:" << endl;
	cout << "		-crep <int>: Repetitions to improve time measurement." << endl;
	cout << "	MIP Formulation:" << endl;
	cout << "		-ml: Run linear relaxation before MIP." << endl;
	cout << "		-mlo: Run linear relaxation only." << endl;
	cout << "		-mcu: Use cuts." << endl;
	cout << "		-mor: Use order subtour elimination." << endl;
	cout << "		-mfs: Force known solution (if exists)." << endl;
	cout << "		-mem <emphasis>: MIP emphasis:" << endl;
	cout << "			bal: Balanced (default)." << endl;
	cout << "			opt: Optimality." << endl;
	cout << "			feas: Feasibility." << endl;
	cout << "			bb: Best Bound." << endl;
	cout << "			hid: Hidden Feasibility." << endl;
}

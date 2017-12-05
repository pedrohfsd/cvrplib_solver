#ifndef CUT_H
#define CUT_H

#include <vector>

class Cut
{
public:
	enum Sense
	{
		greater_equal,
		equal,
		lower_equal,
	};

	Cut();
	Cut(double rhs, Sense sense);


	std::vector<std::pair<int, double>> coeff;
	std::vector<int> vertices;
	Sense sense;
	double rhs;
};

#endif //CUT_H

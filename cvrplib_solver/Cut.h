#ifndef CUT_H
#define CUT_H

#include <vector>

class Cut
{
public:
	Cut();
	Cut(int rhs, int sense);

	enum Sense
	{
		greater_equal,
		equal,
		lower_equal,
	};

	std::vector<std::pair<int, double>> coeff;
	Sense sense;
	double rhs;
};

#endif //CUT_H

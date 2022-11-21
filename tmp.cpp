#include<cstdlib>
#include <algorithm>

bool doubleEq(const double a, const double b) {
	const auto relative_difference_factor = 0.0001;    // 0.01%
	double aa = std::abs(a);
	double ab = std::abs(b);
	const double greater_magnitude = std::max(
		1, 2
	);//std::max(std::abs(a), std::abs(b));

	if (std::abs(a - b) < relative_difference_factor * greater_magnitude)
		return true;
	return false;
};
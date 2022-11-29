#pragma once
#define NOMINMAX //https://stackoverflow.com/a/2789510/488802
#include<cstdlib>
#include<algorithm>


struct Vector {
	double x, y;

	inline bool doubleEq(const double a, const double b) const{
		const auto relative_difference_factor = 0.0001;    // 0.01%
		const auto greater_magnitude = std::max(std::abs(a), std::abs(b));
		if (std::abs(a - b) <= relative_difference_factor * greater_magnitude)
			return true;
		return false;
	};

	double lensq() const { return x * x + y * y; }
	 
	Vector operator-() const {return Vector{ -x, -y }; };
	bool operator>=(const Vector& b) const {
		return lensq() >= b.lensq() ? true : false;
	}

	bool operator==(const Vector& other) const {
		return doubleEq(x, other.x) && doubleEq(y, other.y);
	}
};
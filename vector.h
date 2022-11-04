#pragma once
struct Vector {
	double x, y;

	double lensq() const { return x * x + y * y; }

	bool operator>=(const Vector& b) const {
		return lensq() >= b.lensq() ? true : false;
	}
};
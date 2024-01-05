#include <cmath>
struct ext_double {
	double value;
	const double EPS = 1e-10;
	ext_double(double value) : value(value) {}
	ext_double& operator+=(const ext_double& other) {
		(*this).value += other.value;
		return *this;
	}
	ext_double& operator-=(const ext_double& other) {
		(*this).value -= other.value;
		return *this;
	}
	ext_double& operator*=(const ext_double& other) {
		(*this).value *= other.value;
		return *this;
	}
	ext_double& operator/=(const ext_double& other) {
		(*this).value /= other.value;
		return *this;
	}
	ext_double& operator=(const ext_double& other) {
		(*this).value = other.value;
		return *this;
	}
	ext_double operator+(const ext_double& other) const { return ext_double(*this) += other; }
	ext_double operator-(const ext_double& other) const { return ext_double(*this) -= other; }
	ext_double operator*(const ext_double& other) const { return ext_double(*this) *= other; }
	ext_double operator/(const ext_double& other) const { return ext_double(*this) /= other; }
	bool operator==(ext_double other) { return std::abs(value - other.value) < EPS; }
	bool operator!=(ext_double other) { return std::abs(value - other.value) >= EPS; }
	bool operator<(ext_double other) { return value - EPS <= other.value; }
	bool operator<=(ext_double other) { return ((*this) < other) || ((*this) == other); }
	bool operator>(ext_double other) { return other.value - EPS <= value; }
	bool operator>=(ext_double other) { return ((*this) > other) || ((*this) == other); }
	friend bool operator==(const ext_double&x, const ext_double&y) { return (ext_double)x == y; }
	friend bool operator!=(const ext_double&x, const ext_double&y) { return (ext_double)x != y; }
	friend bool operator<(const ext_double&x, const ext_double&y) { return (ext_double)x < y; }
	friend bool operator<=(const ext_double&x, const ext_double&y) { return (ext_double)x <= y; }
	friend bool operator>(const ext_double&x, const ext_double&y) { return (ext_double)x > y; }
	friend bool operator>=(const ext_double&x, const ext_double&y) { return (ext_double)x >= y; }
};
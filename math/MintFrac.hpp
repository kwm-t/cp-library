#include<iostream>
struct fracmint {
	mint x, y;
	fracmint(long long x, long long y = 1) :x(x), y(y) {}
	fracmint(mint x = 0, mint y = 1) :x(x), y(y) {}
	fracmint& operator+=(const fracmint& other) {
		mint nx = x * other.y + y * other.x;
		mint ny = y * other.y;
		(*this).x = nx;
		(*this).y = ny;
		return *this;
	}
	fracmint& operator-=(const fracmint& other) {
		mint nx = x * other.y - y * other.x;
		mint ny = y * other.y;
		(*this).x = nx;
		(*this).y = ny;
		return *this;
	}
	fracmint& operator*=(const fracmint& other) {
		mint nx = x * other.x;
		mint ny = y * other.y;
		(*this).x = nx;
		(*this).y = ny;
		return *this;
	}
	fracmint& operator/=(const fracmint& other) {
		mint nx = x * other.y;
		mint ny = y * other.x;
		(*this).x = nx;
		(*this).y = ny;
		return *this;
	}
	fracmint& operator=(const fracmint& other) {
		(*this).x = other.x;
		(*this).y = other.y;
		return *this;
	}
	fracmint operator+(const fracmint& other) const { return fracmint(*this) += other; }
	fracmint operator-(const fracmint& other) const { return fracmint(*this) -= other; }
	fracmint operator*(const fracmint& other) const { return fracmint(*this) *= other; }
	fracmint operator/(const fracmint& other) const { return fracmint(*this) /= other; }
	bool operator==(fracmint other) { return x * other.y == other.x*y; }
	bool operator!=(fracmint other) { return x * other.y != other.x*y; }
	fracmint& operator++(int n) { return *this = fracmint(*this) += 1; }
	fracmint& operator--(int n) { return *this = fracmint(*this) -= 1; }
	friend bool operator==(const fracmint&x, const fracmint&y) { return (fracmint)x == y; }
	friend bool operator!=(const fracmint&x, const fracmint&y) { return (fracmint)x != y; }
	mint val() { return x * (y.inv()); }
	void output() { std::cout << (x * (y.inv())).val() << std::endl; }
	void debug() { std::cout << x.val() << "/" << y.val() << std::endl; }
};
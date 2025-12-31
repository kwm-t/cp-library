#include <numeric>
#include<iostream>
struct frac {
	long long nume, deno;
	frac(long long n = 0, long long d = 1) {
		if (d < 0) nume = -n, deno = -d;
		else nume = n, deno = d;
	}
	void reduce() {
		if (deno < 0) {
			deno *= -1;
			nume *= -1;
		}
		long long g = std::gcd(nume, deno);
		nume /= g;
		deno /= g;
	}
	frac& operator+=(const frac& other) {
		long long nnume = nume * other.deno + deno * other.nume;
		long long ndeno = deno * other.deno;
		(*this).nume = nnume;
		(*this).deno = ndeno;
		(*this).reduce();
		return *this;
	}
	frac& operator-=(const frac& other) {
		long long nnume = nume * other.deno - deno * other.nume;
		long long ndeno = deno * other.deno;
		(*this).nume = nnume;
		(*this).deno = ndeno;
		(*this).reduce();
		return *this;
	}
	frac& operator*=(const frac& other) {
		long long nnume = nume * other.nume;
		long long ndeno = deno * other.deno;
		(*this).nume = nnume;
		(*this).deno = ndeno;
		(*this).reduce();
		return *this;
	}
	frac& operator/=(const frac& other) {
		long long nnume = nume * other.deno;
		long long ndeno = deno * other.nume;
		(*this).nume = nnume;
		(*this).deno = ndeno;
		(*this).reduce();
		return *this;
	}
	frac& operator=(const frac& other) {
		(*this).nume = other.nume;
		(*this).deno = other.deno;
		(*this).reduce();
		return *this;
	}
	frac operator+(const frac& other) const { return frac(*this) += other; }
	frac operator-(const frac& other) const { return frac(*this) -= other; }
	frac operator*(const frac& other) const { return frac(*this) *= other; }
	frac operator/(const frac& other) const { return frac(*this) /= other; }
	bool operator==(frac other) { return nume * other.deno == other.nume*deno; }
	bool operator!=(frac other) { return nume * other.deno != other.nume*deno; }
	bool operator<(frac other) { return nume * other.deno < other.nume*deno; }
	bool operator>(frac other) { return nume * other.deno > other.nume*deno; }
	friend bool operator==(const frac&x, const frac&y) { return (frac)x == y; }
	friend bool operator!=(const frac&x, const frac&y) { return (frac)x != y; }
	friend bool operator<(const frac&x, const frac&y) { return (frac)x < y; }
	friend bool operator>(const frac&x, const frac&y) { return (frac)x > y; }
	double getDoulbeValue() { return (double)nume / deno; }
	void output() const {
		if (1 == deno)std::cout << nume << std::endl;
		else std::cout << nume << "/" << deno << std::endl;
	}
};
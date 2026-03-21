#pragma once

#include <vector>
#include "FormalPowerSeries.hpp"

template <typename mint>
std::vector<mint> bernoulli(int n, Factorial<mint> &fc) {
	// B_{0}からB_{n}までを返す
	n++;
	FormalPowerSeries<mint> f((fc.finv).begin() + 1, (fc.finv).end());
	f = f.inv();
	for (int i = 0; i < n; ++i) f[i] *= fc.fac[i];
	return f;
}

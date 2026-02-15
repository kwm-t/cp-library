#pragma once

#include <vector>
#include "FormalPowerSeries.hpp"

std::vector<mint> bernoulli(int n) {
	// B_{0}からB_{n}までを返す
	n++;
	fc = Factorial<mint>(n);
	fps f((fc.finv).begin() + 1, (fc.finv).end());
	f = f.inv();
	for (int i = 0; i < n; ++i) f[i] *= fc.fac[i];
	return f;
}
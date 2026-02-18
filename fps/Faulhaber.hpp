#pragma once

#include <vector>
#include "Bernoulli.hpp"

template <typename mint>
std::vector<mint>faulhaber(int k, int shift, Factorial<mint> &fc) {
	// n乗和の公式の母関数を返す
	auto b = bernoulli<mint>(k + 1, fc);
	std::vector<mint>f(k + 2);
	mint inv = mint(k + 1).inv();
	for (int i = 0; i <= k; ++i) {
		f[k + 1 - i] = fc.binom(k + 1, i) * b[i] * inv *(i % 2 ? -1 : 1);
	}
	if (shift)f = ((FormalPowerSeries<mint>)f).shift_inplace(shift);
	return f;
}
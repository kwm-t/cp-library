#pragma once

#include <vector>
#include "Bernoulli.hpp"

std::vector<mint>faulhaber(int k) {
	// n乗和の公式の母関数を返す
	auto b = bernoulli(k + 1);
	std::vector<mint>f(k + 2);
	mint inv = mint(k + 1).inv();
	for (int i = 0; i <= k; ++i) {
		f[k + 1 - i] = c(k + 1, i) * b[i] * inv *(i % 2 ? -1 : 1);
	}
	return f;
}
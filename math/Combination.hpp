#pragma once

#include <vector>
template<typename T>
std::vector<std::vector<T>> combination(const int n = 6000) {
	std::vector res(n + 1, std::vector<T>(n + 1));
	for (int i = 0; i < n + 1; ++i) {
		for (int j = 0; j < i + 1; ++j) {
			if (j == 0)res[i][j] = 1;
			else {
				res[i][j] = res[i - 1][j - 1] + res[i - 1][j];
			}
		}
	}
	return res;
}
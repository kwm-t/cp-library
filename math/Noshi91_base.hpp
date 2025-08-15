#ifndef NOSHI91_BASE_HPP
#define NOSHI91_BASE_HPP

#include <algorithm>
#include <iostream>
#include <vector>
std::vector<long long> noshi91_base(std:: vector<long long>a) {
	std::vector<long long>base;
	for (int i = 0; i < (int)a.size(); ++i) {
		for (int j = 0; j < (int)base.size(); ++j) {
			a[i] = std::min(a[i], a[i]^base[j]);
		}
		if (a[i] == 0) continue;
		for (int j = 0; j < (int)base.size(); ++j) {
			base[j] = std::min(base[j], a[i]^base[j]);
		}
		base.push_back(a[i]);
	}
	std::sort(base.begin(), base.end());
	return base;
}

// with_index
std::vector<std::pair<long long, int>> noshi91_base(std::vector<long long> a) {
	std::vector<std::pair<long long, int>> base;
	for (int i = 0; i < (int)a.size(); ++i) {
		for (int j = 0; j < (int)base.size(); ++j) {
			a[i] = std::min(a[i], a[i] ^ base[j].first);
		}
		if (a[i] == 0) continue;
		for (int j = 0; j < (int)base.size(); ++j) {
			base[j].first = std::min(base[j].first, a[i] ^ base[j].first);
		}
		base.push_back({ a[i],i });
	}
	std::sort(base.begin(), base.end());
	return base;
}
#endif
#include <algorithm>
#include <iostream>
#include <vector>
void noshi_base(std:: vector<long long>a) {
	std::vector<long long>base;
	for (int i = 0; i < (int)a.size(); ++i) {
		for (int j = 0; j < base.size(); ++j) {
			a[i] = std::min(a[i], a[i]^base[j]);
		}
		if (a[i] == 0) continue;
		for (int j = 0; j < base.size(); ++j) {
			base[j] = std::min(base[j], a[i]^base[j]);
		}
		base.push_back(a[i]);
	}
	std::sort(base.begin(), base.end());
}
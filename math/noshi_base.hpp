#include <algorithm>
#include <iostream>
#include <vector>
void noshi_base(std:: vector<long long>a) {
	std::vector<long long>base;
	for (int i = 0; i < (int)a.size(); ++i) {
		long long a; std::cin >> a;
		for (int j = 0; j < base.size(); ++j) {
			a = std::min(a, a^base[j]);
		}
		if (a == 0) continue;
		for (int j = 0; j < base.size(); ++j) {
			base[j] = std::min(base[j], a^base[j]);
		}
		base.push_back(a);
	}
	std::sort(base.begin(), base.end());
}
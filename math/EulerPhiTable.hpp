#include <vector>
std::vector<long long>euler_phi_table(int n) {
	std::vector<long long>euler(n + 1);
	for (int i = 0; i < n + 1; ++i)euler[i] = i;
	for (int i = 2; i < n + 1; ++i) {
		if (euler[i] == i) {
			for (int j = i; j <= n; j += i) {
				euler[j] = euler[j] / i * (i - 1);
			}
		}
	}
	return euler;
}
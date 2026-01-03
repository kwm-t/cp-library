#include <vector>
template <typename T>
std::vector<std::pair<T, std::pair<T, T>>> enumerate_quotients(T n) {
	T l = 1;
	std::vector<std::pair<T, std::pair<T, T>>> quotients;
	while (l <= n) {
		T q = n / l;
		T r = n / q + 1;
		quotients.push_back({ q, {l, r} });
		l = r;
	}
	std::reverse(quotients.begin(), quotients.end());
	return quotients;
}
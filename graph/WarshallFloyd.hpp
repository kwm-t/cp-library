#include <algorithm>
#include <utility>
#include <vector>
template<typename T>
void warshallFloyd(int n, std::vector<std::vector<T>> &d, T inf) {
	for (int k = 0; k < n; ++k)for (int i = 0; i < n; ++i)for (int j = 0; j < n; ++j) {
		if (inf == d[i][k] || inf == d[k][j])continue;
		d[i][j] = std::min(d[i][j], d[i][k] + d[k][j]);
	}
}
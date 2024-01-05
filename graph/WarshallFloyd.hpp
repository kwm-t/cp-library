#include <algorithm>
#include <utility>
#include <vector>
void WarshallFloyd(int n,std::vector<std::vector<int>> &d) {
	for (int k = 0; k < n; ++k)for (int i = 0; i < n; ++i)for (int j = 0; j < n; ++j) {
		d[i][j] = std::min(d[i][j], d[i][k] + d[k][j]);
	}
}
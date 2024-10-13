#include <vector>
const int INF = 1e9;

// 正20面体でのdp
template<typename T>
struct Icosahedron {
	const int sz = 12;
	std::vector<std::vector<int>>edge;
	std::vector<std::vector<T>> dp;
	Icosahedron() {
		edge.reserve(sz);
		edge[0] = { 1,2,3,4,5 };
		edge[1] = { 0,5,2,6,10 };
		edge[2] = { 0,1,3,10,9 };
		edge[3] = { 0,2,4,9,8 };
		edge[4] = { 0,3,5,8,7 };
		edge[5] = { 0,4,1,7,6 };
		edge[6] = { 11,10,7,1,5 };
		edge[7] = { 11,6,8,5,4 };
		edge[8] = { 11,7,9,4,3 };
		edge[9] = { 11,8,10,3,2 };
		edge[10] = { 11,9,6,2,1 };
		edge[11] = { 6,7,8,9,10 };
	}
	void solve(int n) {
		dp.resize(n + 1);
		for (int i = 0; i < n + 1; ++i)dp[i].resize(sz);
		dp[0][0] = 1;
		for (int i = 1; i < sz; ++i)dp[0][i] = 0;
		// dp
	}
};
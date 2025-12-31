#include <vector>
#include <iostream> 
struct TreeIsomorphic {
public:
	TreeIsomorphic(int n, std::vector<mint> rnum) :sz(n), root(0), rnum(rnum){
		to.resize(sz);
	}
	void init() {
		ReadGraph();
	}
	mint sovle() {
		return dfs(root).first;
	}
private:
	void ReadGraph() {
		for (int i = 0; i < sz - 1; ++i) {
			int a, b; std::cin >> a >> b; a--; b--;
			to[a].push_back(b);
			to[b].push_back(a);
		}
	}
	std::pair<mint, int> dfs(int v, int p = -1) {
		mint res = 1;
		std::vector<mint>vec;
		int sec = 0;
		for (int e : to[v]) {
			if (p == e) continue;
			auto s = dfs(e, v);
			vec.push_back(s.first);
			sec = std::max(sec, s.second + 1);
		}
		mint fir = 1;
		for (int i = 0; i < vec.size(); ++i)fir *= vec[i] + rnum[sec];
		return { fir,sec };
	}
private:
	int sz, root;
	std::vector<std::vector<int>>to;
	std::vector<mint> rnum;
};
//uint_fast64_t seed = 202212022100123456;
//mt19937_64 engine(seed);
//rep(i, n) rnum[i] = engine();
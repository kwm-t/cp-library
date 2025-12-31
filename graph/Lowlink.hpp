class Lowlink {
	typedef std::vector<std::vector<std::pair<int, int>>> Edge;
public:
	// 頂点数
	int sz;
	// 木
	Edge G;
	// 関節点
	std::vector<int>articulation_points;
	// 橋(Key:idx,Value:child)
	// in[親] < low[子]なら、橋となる。
	std::unordered_map<int, int> bridge;
	// roots
	std::unordered_set<int>roots;
private:
	// dfs順
	std::vector<int>in;
	std::vector<int>out;
	std::vector<int>low;
	// bfs木上で子のリスト
	std::vector<std::vector<int>> child;
public:
	// コンストラクタ
	Lowlink(const Edge &G) : G(G) {}
	// 構築
	void build() {
		sz = G.size();
		in.assign(sz, -1);
		out.assign(sz, -1);
		low.assign(sz, -1);
		child.resize(sz);
		int k = 0;
		for (int i = 0; i < sz; ++i) {
			if (in[i] < 0) {
				roots.insert(i);
				dfs(i, -1, k);
			}
		}
	}
	// 頂点vを取り除いたときに連結成分が1->retになる
	int remove(int v) {
		int c = 0;
		for (auto nv : child[v]) {
			if (in[v] <= low[nv])c++;
		}
		if (!roots.count(v))c++;
		return c;
	}
	// G 上において、s-t パスは必ず辺 a を含むかどうか判定
	bool query(int a, int s, int t) {
		if (!bridge.count(a)) return false;
		int l = in[bridge[a]], r = out[bridge[a]];
		s = in[s], t = in[t];
		return (l <= s and s < r) xor (l <= t and t < r);
	}
	void dump() {
		std::cout << "edge" << std::endl;
		for (int i = 0; i < sz; ++i) for (int j = 0; j < (int)G[i].size(); ++j) std::cout << i << " " << G[i][j].first << " " << G[i][j].second << std::endl;
		std::cout << "in" << std::endl;
		for (int i = 0; i < sz; ++i)std::cout << in[i] << " ";
		std::cout << std::endl;
		std::cout << "out" << std::endl;
		for (int i = 0; i < sz; ++i)std::cout << out[i] << " ";
		std::cout << std::endl;
		std::cout << "low" << std::endl;
		for (int i = 0; i < sz; ++i)std::cout << low[i] << " ";
		std::cout << std::endl;
		std::cout << "bridge" << std::endl;
		for (auto[k, v] : bridge)std::cout << k << " " << v << std::endl;
		std::cout << "articulation_points" << std::endl;
		for (int i = 0; i < (int)articulation_points.size(); ++i) {
		std::cout << articulation_points[i] << " ";
		}std::cout << std::endl;
	}
private:
	void dfs(int v, int p, int &k) {
		in[v] = low[v] = k++;
		bool is_art = false;// 関節点判定
		for (auto[nv, id] : G[v]) {
			if (in[nv] < 0) {
				child[v].push_back(nv);
				dfs(nv, id, k);
				low[v] = std::min(low[v], low[nv]);
				// is関節点?
				if (-1 != p && in[v] <= low[nv]) {
					is_art = true;
				}
				// is橋?
				if (in[v] < low[nv]) {
					bridge[id] = nv;
				}
			}
			else if (id != p) {
				low[v] = std::min(low[v], in[nv]);
			}
		}
		// 根の場合、2つ以上の子があるなら、関節点
		if (-1 == p && child[v].size() >= 2) {
			is_art = true;
		}
		if (is_art) {
			articulation_points.push_back(v);
		}
		out[v] = k++;
	}
};
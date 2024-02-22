class BestTheorem {
public:
	BestTheorem(std::vector<std::vector<int>>edge) :n(edge.size()), edge(edge) {}
	// 使用しない頂点があると壊れるので呼ぶ側で座圧が必要
	mint execute(int startPount = -1, bool cycle = true, bool walk = true, bool distinctEdge = false) {
		// startPount:cycleときの開始頂点を決め打つ
		// cycle:閉路許容
		// walk:walk許容
		// distinctEdge:辺の区別をするかどうか
		std::vector<int>in(n), out(n);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				in[j] += edge[i][j];
				out[i] += edge[i][j];
			}
		}
		mint val = 1;
		if (!distinctEdge) {
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j) {
					val *= c.ifact[edge[i][j]];
				}
			}
		}
		int sv = -1, tv = -1, cnt = 0, tot = 0;
		for (int i = 0; i < n; ++i) {
			tot += out[i];
			if (in[i] == out[i])continue;
			int diff = in[i] - out[i];
			if (std::abs(diff) >= 2) return 0;
			if (1 == diff)tv = i, cnt++;
			if (-1 == diff)sv = i;
		}
		// cycle でも walk でもない
		if (cnt >= 2) return 0;
		// cycle
		if (-1 == sv) {
			if (!cycle) return 0;
			if(-1== startPount)val *= tot;// 開始点の数を掛ける
			else val *= out[startPount];
		}
		// walk
		else {
			if (!walk)return 0;
			// tv->svに辺を増やしてcycleにする
			out[tv]++;
			in[sv]++;
			edge[tv][sv]++;
		}
		val *= count_spanning_tree();
		for (int i = 0; i < n; ++i) {
			val *= c.fact[out[i] - 1];
		}
		return val;
	}
private:
	int n;
	std::vector<std::vector<int>>edge;
	mint count_spanning_tree() {
		//行列木定理
		std::vector mat(n, std::vector<mint>(n));
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				mat[i][j] -= edge[i][j];
				mat[i][i] += edge[i][j];
			}
		}
		mint ret = 1;
		for (int i = 0; i < (int)mat.size(); ++i) {
			if (0 == i)continue;
			ret *= mat[i][i];
			if (0 == mat[i][i]) continue;
			mint inv = inv_mod(mat[i][i].val(), mod);
			for (int j = 0; j < (int)mat.size(); ++j) {
				if (i == j) continue;
				mint num = mat[j][i] * inv;
				for (int k = 0; k < (int)mat.size(); ++k) mat[j][k] -= num * mat[i][k];
			}
		}
		return ret;
	}
};
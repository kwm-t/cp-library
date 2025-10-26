#include <vector>
void ParallelBinarySearch() {
	int k, q;
	std::vector<int>ok(q, k), ng(q, -1);
	while (true) {
		bool fin = true;
		std::vector<std::vector<int>>mid(k);
		for (int i = 0; i < q; ++i) {
			if (abs(ok[i] - ng[i]) > 1) {
				fin = false;
				int m = (ok[i] + ng[i]) / 2;
				mid[m].push_back(i);
			}
		}
		if (fin)break;
		//todo	データ構造
		DualSegtree<S, op, e>seg(n);
		for (int i = 0; i < k; ++i) {
			//todo	データ構造の更新
			seg.apply(l[i], r[i] + 1, h[i]);
			for(int j = 0;j < mid[i].size();++j) {
				//todo	データ構造の取得
				long long get = seg.get(x[mid[i][j]]);
				//todo	配列更新
				if (get >= y[mid[i][j]])ok[mid[i][j]] = i;
				else ng[mid[i][j]] = i;
			}
		}
	}
}
// https://atcoder.jp/contests/past20-open/submissions/70477611
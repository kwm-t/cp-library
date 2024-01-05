template <class T, T(*op)(T, T), T(*e)()> struct SparseTable {
	int n, LOG;
	std::vector<std::vector<T>> table;
	std::vector<int> log_table;
	SparseTable() {}
	SparseTable(const std::vector<T> &v) : n(v.size()), LOG(lg(std::max(n, 1)) + 1),
		log_table(n + 1), table(LOG, std::vector<T>(n)) {
		std::copy(v.begin(), v.end(), table[0].begin());
		for (int i = 1; i < LOG; i++) {
			for (int j = 0; j + (1 << i) <= n; j++) {
				table[i][j] = op(table[i - 1][j], table[i - 1][j + (1 << (i - 1))]);
			}
		}
		for (int i = 2; i <= n; i++) {
			log_table[i] = log_table[i >> 1] + 1;
		}
	}
	T prod(int l, int r) {
		if (l == r) return e();
		int b = log_table[r - l];
		return op(table[b][l], table[b][r - (1 << b)]);
	}
	int lg(int n) {
		int ret = 0;
		while (n > 1) {
			n /= 2;
			ret++;
		}
		return ret;
	}
};
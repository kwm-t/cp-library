// monotoneなn * m行列について
// 各行の最小要素のうち最も左に位置するものの列番号を
// O(n+m)で求めるアルゴリズム
template<typename T, typename F>
std::vector<std::pair<int, T>> monotoneMinima(const int n, const int m, const F& f) {
	std::vector<std::pair<int, T>>result(n);
	auto dfs = [&](auto && self, int top, int bottom, int left, int right)->void {
		if (top > bottom)return;
		int line = (top + bottom) / 2;
		T minValue = f(line, left);
		int minIndex = left;
		for (int i = left + 1; i <= right; ++i) {
			T value = f(line, i);
			if (value < minValue) {
				minValue = value;
				minIndex = i;
			}
		}
		result[line] = std::make_pair(minIndex, minValue);
		self(self, top, line - 1, left, minIndex);
		self(self, line + 1, bottom, minIndex, right);
	};
	dfs(dfs, 0, n - 1, 0, m - 1);
	return result;
}
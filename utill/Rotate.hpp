// ‰ñ“]
std::vector<std::string> rotate(const std::vector<std::string> &org) {
	int n = org.size();
	int m = org[0].size();
	std::vector<std::string> ret(m, std::string(n, '.'));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			ret[j][n - 1 - i] = org[i][j];
		}
	}
	return ret;
}
template<typename T> std::vector<std::vector<T>> rotate(const std::vector<std::vector<T>> &org) {
	int n = org.size();
	int m = org[0].size();
	std::vector ret(m, std::vector<T>(n));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			ret[j][n - 1 - i] = org[i][j];
		}
	}
	return ret;
}
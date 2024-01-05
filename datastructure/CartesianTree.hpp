// Cartesian Tree
template<class T> struct CartesianTree {
	int root;  // root
	std::vector<int> par, left, right;

	CartesianTree() {}
	CartesianTree(const std::vector<T>& v) : root(0),
		par(v.size(), -1), left(v.size(), -1), right(v.size(), -1) {
		std::vector<int> st(v.size(), 0);
		int top = 0;
		for (int i = 1; i < v.size(); ++i) {
			if (v[st[top]] > v[i]) {
				while (top >= 1 && v[st[top - 1]] > v[i]) --top;
				left[i] = st[top];
				par[left[i]] = i;
				if (top == 0) {
					root = i;
				}
				else {
					par[i] = st[top - 1];
					right[par[i]] = i;
				}
				st[top] = i;
			}
			else {
				par[i] = st[top];
				right[par[i]] = i;
				st[++top] = i;
			}
		}
	}
};
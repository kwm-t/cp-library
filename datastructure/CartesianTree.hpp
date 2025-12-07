// Cartesian Tree
template<class T, class Compare = std::less<T>>
struct CartesianTree {
	int root;  // root
	std::vector<int> par, left, right;
	Compare comp;

	CartesianTree() {}
	CartesianTree(const std::vector<T>& v) : root(0),
		par(v.size(), -1), left(v.size(), -1), right(v.size(), -1) {
		std::vector<int> st(v.size(), 0);
		int top = 0;
		for (int i = 1; i < (int)v.size(); ++i) {
			if (comp(v[i], v[st[top]])) {
				while (top >= 1 && comp(v[i], v[st[top - 1]])) --top;
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
// 以下でrootを0とn-1で切り替え
// CartesianTree<int, std::greater<int>> ct(a);
// CartesianTree<int, std::greater<int>> ct(a);
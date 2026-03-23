#ifndef KWM_T_TREE_CARTESIAN_TREE_HPP
#define KWM_T_TREE_CARTESIAN_TREE_HPP

#include <vector>
#include <functional>

/**
 * @brief Cartesian Tree (Min / Max Heap + Inorder = original order)
 *
 * 典型用途:
 *  - RMQ
 *  - LCA (Euler Tour)
 *  - 区間最小値構造
 *
 * 計算量:
 *  - 構築: O(N)
 *
 * @tparam T 値型
 * @tparam Compare 比較関数
 *   std::less<T>    → min Cartesian Tree
 *   std::greater<T> → max Cartesian Tree
 * verified:
 *   https://atcoder.jp/contests/arc216/submissions/74357931
 */
namespace kwm_t::tree {

template<class T, class Compare = std::less<T>>
struct CartesianTree {
	int n;
	int root;

	std::vector<int> par, left, right;
	Compare comp;

	CartesianTree() : n(0), root(-1) {}

	CartesianTree(const std::vector<T>& v) {
		build(v);
	}

	void build(const std::vector<T>& v) {
		n = (int)v.size();
		root = -1;

		par.assign(n, -1);
		left.assign(n, -1);
		right.assign(n, -1);

		std::vector<int> st;
		st.reserve(n);

		for (int i = 0; i < n; ++i) {
			int last = -1;

			while (!st.empty() && comp(v[i], v[st.back()])) {
				last = st.back();
				st.pop_back();
			}

			if (!st.empty()) {
				par[i] = st.back();
				right[st.back()] = i;
			}

			if (last != -1) {
				left[i] = last;
				par[last] = i;
			}

			st.push_back(i);
		}

		// root は stack の一番下
		if (!st.empty()) root = st.front();
	}

	int parent(int v) const { return par[v]; }
	int lchild(int v) const { return left[v]; }
	int rchild(int v) const { return right[v]; }
};

} // namespace kwm_t::tree

#endif // KWM_T_TREE_CARTESIAN_TREE_HPP
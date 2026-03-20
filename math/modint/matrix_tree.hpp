#ifndef KMW_T_MATH_MODINT_MATRIX_TREE_HPP
#define KMW_T_MATH_MODINT_MATRIX_TREE_HPP

#include <vector>

/**
 * @brief 行列木定理（spanning tree数）
 *
 * 典型用途:
 *   グラフの全域木の数を求める
 *
 * 計算量:
 *   O(N^3)
 *
 * @tparam Mint modint型
 *
 * 制約 / 注意:
 *   - ラプラシアン行列を渡す
 *   - 0行0列を削除した (N-1)x(N-1) 行列を使う
 *   - mod は素数
 * verified:
 *   https://yukicoder.me/submissions/1153309
 */

namespace kmw_t::math::modint {

	template <class Mint>
	Mint count_spanning_tree(std::vector<std::vector<Mint>> mat) {
		int n = (int)mat.size();
		Mint det = 1;

		for (int i = 1; i < n; ++i) {
			if (mat[i][i] == Mint(0)) {
				// pivot探す
				for (int j = i + 1; j < n; ++j) {
					if (mat[j][i] != Mint(0)) {
						std::swap(mat[i], mat[j]);
						det = -det;
						break;
					}
				}
			}

			if (mat[i][i] == Mint(0)) return 0;

			Mint inv = mat[i][i].inv();
			det *= mat[i][i];

			for (int j = i + 1; j < n; ++j) {
				Mint factor = mat[j][i] * inv;
				for (int k = i; k < n; ++k) {
					mat[j][k] -= factor * mat[i][k];
				}
			}
		}

		return det;
	}

} // namespace kmw_t::math::modint

#endif // KMW_T_MATH_MODINT_MATRIX_TREE_HPP
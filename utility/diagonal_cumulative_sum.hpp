#ifndef KWM_T_UTILITY_DIAGONAL_CUMULATIVE_SUM_HPP
#define KWM_T_UTILITY_DIAGONAL_CUMULATIVE_SUM_HPP

#include <vector>
#include <cassert>

namespace kwm_t::utility {

/**
 * @brief 斜め累積和（Diagonal Prefix Sum）
 *
 * 典型用途:
 *   - 斜め方向（↘ / ↙）の区間和を高速に求める
 *   - 斜め一直線上の部分列の和クエリ
 *   - グリッド問題で「対角方向のみ」の集計
 *
 * 計算量:
 *   - 構築: O(HW)
 *   - クエリ: O(1)
 *
 * @tparam T
 *   加算・減算が定義されている型（int, long long, double など）
 *
 * @param a
 *   元となる H×W の2次元配列
 *
 * 制約 / 注意:
 *   - 0-indexed
 *   - クエリは半開区間:
 *       diag_down : [ (x0,y0) → (x1-1,y1-1) ]
 *       diag_up   : [ (x0,y0) → (x1-1,y1+1) ]
 *   - (x1-x0) == abs(y1-y0) を満たす必要がある（同一対角線）
 *
 * 使用例:
 *   std::vector<std::vector<int>> a = {
 *       {1, 2, 3},
 *       {4, 5, 6},
 *       {7, 8, 9}
 *   };
 *
 *   kwm_t::utility::DiagonalPrefixSum<int> dps(a);
 *
 *   // ↘ 方向 (0,0) → (3,3) の対角和 = 1+5+9 = 15
 *   int x = dps.sum_down(0, 0, 3, 3);
 *
 *   // ↙ 方向 (0,2) → (3,-1) の対角和 = 3+5+7 = 15
 *   int y = dps.sum_up(0, 2, 3, -1);
 *
 * verified:
 *   - https://yukicoder.me/submissions/1157426
 */
template <class T>
struct DiagonalCumulativeSum {
private:
	int h, w;
	std::vector<std::vector<T>> down; // ↘
	std::vector<std::vector<T>> up;   // ↙

public:
	DiagonalCumulativeSum() = default;

	explicit DiagonalCumulativeSum(const std::vector<std::vector<T>>& a) {
		build(a);
	}

	void build(const std::vector<std::vector<T>>& a) {
		h = (int)a.size();
		w = h ? (int)a[0].size() : 0;

		down.assign(h + 1, std::vector<T>(w + 1, T{}));
		up.assign(h + 1, std::vector<T>(w + 1, T{}));

		// 初期値
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				down[i + 1][j + 1] = a[i][j];
				up[i + 1][j] = a[i][j];
			}
		}

		// 累積
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				down[i + 1][j + 1] += down[i][j];
				up[i + 1][j] += up[i][j + 1];
			}
		}
	}

	int height() const { return h; }
	int width() const { return w; }

	// ↘ 方向: (x0,y0) → (x1-1,y1-1)
	T sum_down(int x0, int y0, int x1, int y1) const {
		assert(0 <= x0 && x0 <= x1 && x1 <= h);
		assert(0 <= y0 && y0 <= y1 && y1 <= w);
		assert(x1 - x0 == y1 - y0);

		return down[x1][y1] - down[x0][y0];
	}

	// ↙ 方向: (x0,y0) → (x1-1,y1+1)
	T sum_up(int x0, int y0, int x1, int y1) const {
		assert(0 <= x0 && x0 <= x1 && x1 <= h);
		assert(-1 <= y1 && y0 < w);
		assert(x1 - x0 == y0 - y1);

		return up[x1][y1 + 1] - up[x0][y0 + 1];
	}
};

} // namespace kwm_t::utility

#endif // KWM_T_UTILITY_DIAGONAL_CUMULATIVE_SUM_HPP
#ifndef KWM_T_UTILITY_CUMULATIVE_SUM_2D_HPP
#define KWM_T_UTILITY_CUMULATIVE_SUM_2D_HPP

#include <vector>
#include <cassert>

/**
 * @brief 2次元累積和（2D Prefix Sum）
 *
 * 典型用途:
 *   - グリッド上の長方形領域の総和を高速に求める
 *   - 「部分矩形和クエリ」を大量に処理する問題
 *   - imos法や二次元DPの前処理
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
 *   - 内部的に (H+1)×(W+1) の配列を持つ
 *   - sum(x0, y0, x1, y1) は半開区間 [x0, x1) × [y0, y1)
 *   - 0-indexed を前提とする
 *   - a は長方形（各行のサイズが同じ）であること
 *
 * 使用例:
 *   std::vector<std::vector<int>> a = {
 *       {1, 2, 3},
 *       {4, 5, 6},
 *       {7, 8, 9}
 *   };
 *
 *   kwm_t::utility::CumulativeSum2D<int> cs(a);
 *
 *   // (0,0) から (2,2) の和 → 1+2+4+5 = 12
 *   int res = cs.sum(0, 0, 2, 2);
 *
 * verified:
 *   - https://atcoder.jp/contests/awc0048/submissions/74980601
 */
namespace kwm_t::utility {

template <class T>
struct CumulativeSum2D {
private:
	std::vector<std::vector<T>> s;

public:
	CumulativeSum2D() = default;

	explicit CumulativeSum2D(const std::vector<std::vector<T>>& a) {
		build(a);
	}

	void build(const std::vector<std::vector<T>>& a) {
		int h = a.size();
		int w = h ? a[0].size() : 0;

		s.assign(h + 1, std::vector<T>(w + 1, T{}));

		for (int i = 0; i < h; i++)
			for (int j = 0; j < w; j++)
				s[i + 1][j + 1] = a[i][j];

		for (int i = 0; i <= h; i++)
			for (int j = 0; j < w; j++)
				s[i][j + 1] += s[i][j];

		for (int i = 0; i < h; i++)
			for (int j = 0; j <= w; j++)
				s[i + 1][j] += s[i][j];
	}

	int height() const { return (int)s.size() - 1; }
	int width() const { return s.empty() ? 0 : (int)s[0].size() - 1; }

	// [x0, x1) × [y0, y1)
	T sum(int x0, int y0, int x1, int y1) const {
		assert(0 <= x0 && x0 <= x1 && x1 <= height());
		assert(0 <= y0 && y0 <= y1 && y1 <= width());

		return s[x1][y1]
			- s[x1][y0]
			- s[x0][y1]
			+ s[x0][y0];
	}
};

} // namespace kwm_t::utility

#endif // KWM_T_UTILITY_CUMULATIVE_SUM_2D_HPP
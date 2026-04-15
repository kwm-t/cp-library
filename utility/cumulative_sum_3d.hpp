#ifndef KWM_T_UTILITY_CUMULATIVE_SUM_3D_HPP
#define KWM_T_UTILITY_CUMULATIVE_SUM_3D_HPP

#include <vector>
#include <cassert>

/**
 * @brief 3次元累積和（3D Prefix Sum）
 *
 * 典型用途:
 *   - 3次元グリッド上の直方体領域の総和を高速に求める
 *   - 「部分直方体和クエリ」を大量に処理する問題
 *   - 3次元imos法や多次元DPの前処理
 *
 * 計算量:
 *   - 構築: O(HWD)
 *   - クエリ: O(1)
 *
 * @tparam T
 *   加算・減算が定義されている型（int, long long, double など）
 *
 * @param a
 *   元となる H×W×D の3次元配列
 *
 * 制約 / 注意:
 *   - 内部的に (H+1)×(W+1)×(D+1) の配列を持つ
 *   - sum(x0, y0, z0, x1, y1, z1) は半開区間
 *       [x0, x1) × [y0, y1) × [z0, z1)
 *   - 0-indexed を前提とする
 *   - a は直方体（各次元でサイズが揃っている）であること
 *   - メモリ使用量は O(HWD) と大きいので注意
 *
 * 使用例:
 *   std::vector<std::vector<std::vector<int>>> a = {
 *       {
 *           {1, 2},
 *           {3, 4}
 *       },
 *       {
 *           {5, 6},
 *           {7, 8}
 *       }
 *   };
 *
 *   kwm_t::utility::CumulativeSum3D<int> cs(a);
 *
 *   // (0,0,0) から (2,2,2) の和 → 全要素の総和 = 36
 *   int res = cs.sum(0, 0, 0, 2, 2, 2);
 *
 * verified:
 *   - 多次元累積和の典型問題（AtCoder等）
 */
namespace kwm_t::utility {

template <class T>
struct CumulativeSum3D {
private:
	std::vector<std::vector<std::vector<T>>> s;

public:
	CumulativeSum3D() = default;

	explicit CumulativeSum3D(const std::vector<std::vector<std::vector<T>>>& a) {
		build(a);
	}

	void build(const std::vector<std::vector<std::vector<T>>>& a) {
		int h = a.size();
		int w = h ? a[0].size() : 0;
		int d = (h && w) ? a[0][0].size() : 0;

		s.assign(h + 1,
			std::vector<std::vector<T>>(w + 1,
				std::vector<T>(d + 1, T{})));

		for (int i = 0; i < h; i++)
			for (int j = 0; j < w; j++)
				for (int k = 0; k < d; k++)
					s[i + 1][j + 1][k + 1] = a[i][j][k];

		for (int i = 0; i <= h; i++)
			for (int j = 0; j <= w; j++)
				for (int k = 0; k < d; k++)
					s[i][j][k + 1] += s[i][j][k];

		for (int i = 0; i <= h; i++)
			for (int j = 0; j < w; j++)
				for (int k = 0; k <= d; k++)
					s[i][j + 1][k] += s[i][j][k];

		for (int i = 0; i < h; i++)
			for (int j = 0; j <= w; j++)
				for (int k = 0; k <= d; k++)
					s[i + 1][j][k] += s[i][j][k];
	}

	// [x0,x1) × [y0,y1) × [z0,z1)
	T sum(int x0, int y0, int z0, int x1, int y1, int z1) const {
		return  s[x1][y1][z1]
			- s[x1][y1][z0]
			- s[x1][y0][z1]
			- s[x0][y1][z1]
			+ s[x0][y0][z1]
			+ s[x0][y1][z0]
			+ s[x1][y0][z0]
			- s[x0][y0][z0];
	}
};

} // namespace kwm_t::utility

#endif // KWM_T_UTILITY_CUMULATIVE_SUM_3D_HPP
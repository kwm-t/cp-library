/**
 * @file imos_2d.hpp
 * @brief 二次元 Imos 法
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/utility/imos_2d.hpp
 */
#ifndef KWM_T_UTILITY_IMOS_2D_HPP
#define KWM_T_UTILITY_IMOS_2D_HPP

#include <vector>
#include <cassert>

namespace kwm_t::utility {

/**
 * @brief 2次元いもす法（2D Difference Array / 2D Imos）
 *
 * 典型用途:
 *   - 長方形領域への一括加算
 *   - グリッドへの大量区間更新
 *   - 塗りつぶし・重なり数計算
 *
 * 計算量:
 *   - add: O(1)
 *   - build: O(HW)
 *   - get: O(1)
 *
 * @tparam T
 *   加算・減算が定義されている型
 *
 * 制約 / 注意:
 *   - add(x0, y0, x1, y1, val) は
 *       半開区間 [x0, x1) × [y0, y1)
 *     に val を加算する
 *   - 0-indexed を前提とする
 *   - build() 後に実際の値が得られる
 *
 * 使用例:
 *   // 5×5 グリッド
 *   kwm_t::utility::Imos2D<int> imos(5, 5);
 *
 *   // [1,4) × [2,5) に +3
 *   imos.add(1, 2, 4, 5, 3);
 *
 *   // [0,2) × [0,2) に +1
 *   imos.add(0, 0, 2, 2, 1);
 *
 *   imos.build();
 *
 *   int v = imos.get(1, 2);
 *
 * verified:
 *   - https://atcoder.jp/contests/awc0070/submissions/75804201
 */
template <class T>
struct Imos2D {
private:
	std::vector<std::vector<T>> d;
	bool built = false;

public:
	Imos2D() = default;

	Imos2D(int h, int w) {
		init(h, w);
	}

	void init(int h, int w) {
		assert(h >= 0 && w >= 0);

		d.assign(h + 1, std::vector<T>(w + 1, T{}));
		built = false;
	}

	int height() const {
		return (int)d.size() - 1;
	}

	int width() const {
		return d.empty() ? 0 : (int)d[0].size() - 1;
	}

	/**
	 * @brief 長方形領域 [x0,x1) × [y0,y1) に val を加算
	 */
	void add(int x0, int y0, int x1, int y1, T val) {
		assert(!built);

		assert(0 <= x0 && x0 <= x1 && x1 <= height());
		assert(0 <= y0 && y0 <= y1 && y1 <= width());

		d[x0][y0] += val;
		d[x1][y0] -= val;
		d[x0][y1] -= val;
		d[x1][y1] += val;
	}

	/**
	 * @brief 累積和を取って復元
	 */
	void build() {
		assert(!built);

		int h = height();
		int w = width();

		for (int i = 0; i <= h; i++)
			for (int j = 0; j < w; j++)
				d[i][j + 1] += d[i][j];

		for (int i = 0; i < h; i++)
			for (int j = 0; j <= w; j++)
				d[i + 1][j] += d[i][j];

		built = true;
	}

	/**
	 * @brief build 後の値を取得
	 */
	T get(int x, int y) const {
		assert(built);

		assert(0 <= x && x < height());
		assert(0 <= y && y < width());

		return d[x][y];
	}

	/**
	 * @brief build 後のグリッドを取得
	 */
	const std::vector<std::vector<T>>& data() const {
		assert(built);
		return d;
	}
};

} // namespace kwm_t::utility

#endif // KWM_T_UTILITY_IMOS_2D_HPP
/**
 * @file diagonal_imos.hpp
 * @brief 対角方向の Imos 法
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/utility/diagonal_imos.hpp
 */
#ifndef KWM_T_UTILITY_DIAGONAL_IMOS_HPP
#define KWM_T_UTILITY_DIAGONAL_IMOS_HPP

#include <vector>
#include <cassert>

namespace kwm_t::utility {

/**
 * @brief 斜めいもす法（Diagonal Imos）
 *
 * 典型用途:
 *   - 斜め方向（↘ / ↙）への区間加算
 *   - 対角線上の区間に一括で値を加える
 *   - 「斜めに影響が伝播する」グリッド問題
 *
 * 計算量:
 *   - 更新: O(1)
 *   - 構築: O(HW)
 *   - 取得: O(1)
 *
 * @tparam T
 *   加算・減算が定義されている型
 *
 * 座標:
 *   - 通常は 0-indexed
 *   - offset を指定した場合、呼び出し側の座標に
 *     offset を加えて内部配列に格納する
 *
 * 制約 / 注意:
 *   - add は半開区間:
 *       ↘ : (x0,y0) → (x1-1,y1-1)
 *       ↙ : (x0,y0) → (x1-1,y1+1)
 *   - (x1-x0) == abs(y1-y0) を満たす必要あり
 *   - build() を呼ぶまで値は確定しない
 *
 * 使用例:
 *   DiagonalImos<int> imos(20, 20, 10);
 *
 *   imos.add_down(-3, -3, 5, 5, 1);
 *   imos.add_up(-3, 5, 5, -3, 2);
 *
 *   auto a = imos.build();
 *   int v = imos.get(0, 0);
 *
 * verified:
 *   - https://yukicoder.me/submissions/1157426
 */
template <class T>
struct DiagonalImos {
private:
	int h, w;
	int offset;
	std::vector<std::vector<T>> down; // ↘
	std::vector<std::vector<T>> up;   // ↙

	int index(int x) const {
		return x + offset;
	}

public:
	DiagonalImos() = default;

	DiagonalImos(int h_, int w_, int offset_ = 0)
		: h(h_), w(w_), offset(offset_) {
		init();
	}

	void init() {
		down.assign(h + 1, std::vector<T>(w + 1, T{}));
		up.assign(h + 1, std::vector<T>(w + 1, T{}));
	}

	int height() const {
		return h;
	}

	int width() const {
		return w;
	}

	// ↘ 方向: (x0,y0) → (x1-1,y1-1)
	void add_down(int x0, int y0, int x1, int y1, T val) {
		assert(x1 - x0 == y1 - y0);
		down[index(x0)][index(y0)] += val;
		down[index(x1)][index(y1)] -= val;
	}

	// ↙ 方向: (x0,y0) → (x1-1,y1+1)
	void add_up(int x0, int y0, int x1, int y1, T val) {
		assert(x1 - x0 == y0 - y1);
		up[index(x0)][index(y0) + 1] += val;
		up[index(x1)][index(y1) + 1] -= val;
	}

	/**
	 * @brief いもす法を構築し、全マスの合算値を返す
	 */
	std::vector<std::vector<T>> build() {
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				down[i + 1][j + 1] += down[i][j];
				up[i + 1][j] += up[i][j + 1];
			}
		}

		std::vector<std::vector<T>> ret(h, std::vector<T>(w));

		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				ret[i][j] = down[i][j] + up[i][j + 1];
			}
		}

		return ret;
	}

	// ↘ の値取得
	T get_down(int i, int j) const {
		return down[index(i)][index(j)];
	}

	// ↙ の値取得
	T get_up(int i, int j) const {
		return up[index(i)][index(j) + 1];
	}

	// ↘ + ↙ の合算値取得
	T get(int i, int j) const {
		return down[index(i)][index(j)]
			+ up[index(i)][index(j) + 1];
	}
};

} // namespace kwm_t::utility

#endif // KWM_T_UTILITY_DIAGONAL_IMOS_HPP
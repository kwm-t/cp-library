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
 * 制約 / 注意:
 *   - 0-indexed
 *   - add は半開区間:
 *       ↘ : (x0,y0) → (x1-1,y1-1)
 *       ↙ : (x0,y0) → (x1-1,y1+1)
 *   - (x1-x0) == abs(y1-y0) を満たす必要あり
 *   - build() を呼ぶまで値は確定しない
 *
 * 使用例:
 *   DiagonalImos<int> imos(3, 3);
 *
 *   // ↘ に +1
 *   imos.add_down(0, 0, 3, 3, 1);
 *
 *   // ↙ に +2
 *   imos.add_up(0, 2, 3, -1, 2);
 *
 *   imos.build();
 *
 *   int v = imos.get_down(1, 1); // 取得
 *
 * verified:
 *   - https://yukicoder.me/submissions/1157426
 */
template <class T>
struct DiagonalImos {
private:
	int h, w;
	std::vector<std::vector<T>> down; // ↘
	std::vector<std::vector<T>> up;   // ↙

public:
	DiagonalImos() = default;

	DiagonalImos(int h_, int w_) : h(h_), w(w_) {
		init();
	}

	void init() {
		down.assign(h + 1, std::vector<T>(w + 1, T{}));
		up.assign(h + 1, std::vector<T>(w + 1, T{}));
	}

	int height() const { return h; }
	int width() const { return w; }

	// ↘ 方向: (x0,y0) → (x1-1,y1-1)
	void add_down(int x0, int y0, int x1, int y1, T val) {
		assert(x1 - x0 == y1 - y0);
		down[x0][y0] += val;
		down[x1][y1] -= val;
	}

	// ↙ 方向: (x0,y0) → (x1-1,y1+1)
	void add_up(int x0, int y0, int x1, int y1, T val) {
		assert(x1 - x0 == y0 - y1);
		up[x0][y0 + 1] += val;
		up[x1][y1 + 1] -= val;
	}

	void build() {
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				down[i + 1][j + 1] += down[i][j];
				up[i + 1][j] += up[i][j + 1];
			}
		}
	}

	// ↘ の値取得
	T get_down(int i, int j) const {
		return down[i][j];
	}

	// ↙ の値取得
	T get_up(int i, int j) const {
		return up[i][j + 1];
	}
};

} // namespace kwm_t::utility

#endif // KWM_T_UTILITY_DIAGONAL_IMOS_HPP
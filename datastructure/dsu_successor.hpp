#ifndef KWM_T_DATA_STRUCTURE_DSU_SUCCESSOR_HPP
#define KWM_T_DATA_STRUCTURE_DSU_SUCCESSOR_HPP

#include <numeric>
#include <vector>

namespace kwm_t::data_structure {

/**
 * @brief 削除可能 successor DS
 *
 * 管理対象:
 *   [0, n)
 *
 * erase(x):
 *   x を削除済みにする
 *
 * next(x):
 *   x 以上の最小の未削除要素を返す
 *   存在しない場合は n を返す
 *
 * contains(x):
 *   x が未削除なら true
 *
 * 典型用途:
 *   - 一度しか処理しない区間走査
 *   - 「次の未使用位置」管理
 *   - 削除付き ordered set の軽量代替
 *
 * 計算量:
 *   amortized O(alpha(N))
 *
 * 使用例:
 *
 *   DSUSuccessor ds(n);
 *
 *   for (int x = ds.next(l); x <= r; x = ds.next(x)) {
 *       // x を処理
 *       x = ds.erase(x);
 *   }
 *
 * verified:
 *   - https://atcoder.jp/contests/awc0080/submissions/76204289
 */

class DSUSuccessor {
private:
	int n;
	std::vector<int> nxt;

	int find(int x) {
		if (nxt[x] == x) return x;
		return nxt[x] = find(nxt[x]);
	}

public:
	DSUSuccessor() : n(0) {}

	explicit DSUSuccessor(int size)
		: n(size), nxt(size + 1) {
		std::iota(nxt.begin(), nxt.end(), 0);
	}

	/**
	 * @brief x 以上の最小未削除要素
	 *
	 * @param x
	 * @return int
	 *   存在しない場合は n
	 */
	int next(int x) {
		return find(x);
	}

	/**
	 * @brief x を削除
	 *
	 * @param x
	 * @return int
	 *   削除後の次要素
	 */
	int erase(int x) {
		return nxt[x] = find(x + 1);
	}

	/**
	 * @brief x が未削除か
	 */
	bool contains(int x) {
		return find(x) == x;
	}

	/**
	 * @brief 管理サイズ
	 */
	int size() const {
		return n;
	}
};

} // namespace kwm_t::data_structure

#endif // KWM_T_DATA_STRUCTURE_DSU_SUCCESSOR_HPP
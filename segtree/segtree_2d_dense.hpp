/**
 * @file segtree_2d_dense.hpp
 * @brief 二次元 Dense Segment Tree
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/segtree/segtree_2d_dense.hpp
 */
#ifndef KWM_T_SEGTREE_SEGTREE_2D_DENSE_HPP
#define KWM_T_SEGTREE_SEGTREE_2D_DENSE_HPP

#include <vector>
#include <cassert>

/**
 * @brief 2次元セグメント木（dense版・自動モード切替）
 *
 * build前:
 *   set(i,j,x) → 値をそのまま格納
 *
 * build後:
 *   set(i,j,x) → 点更新（代入）
 *
 * 典型用途:
 *   - グリッドDP
 *   - 2D range max/sum
 *
 * 計算量:
 *   - build: O(HW)
 *   - update: O(log^2 N)
 *   - query: O(log^2 N)
 *
 * @tparam S モノイド型
 * @tparam op 二項演算
 * @tparam e 単位元
 *
 * 注意:
 *   - updateは「代入」で固定（opではない）
 *   - クエリは半開区間 [i1,i2) × [j1,j2)
 *
 * verified:
 *   - https://atcoder.jp/contests/abc347/submissions/74604644
 */
namespace kwm_t::segtree {

template <class S, S(*op)(S, S), S(*e)()>
class segtree_2d_dense {
private:
	int H, W;
	std::vector<S> seg;
	bool built = false;

public:
	segtree_2d_dense() = default;

	segtree_2d_dense(int h, int w) {
		init(h, w);
	}

	segtree_2d_dense(const std::vector<std::vector<S>>& a) {
		int h = (int)a.size();
		int w = (int)a[0].size();
		init(h, w);
		set_all(a);
		build();
	}

	/**
	 * @brief 初期化
	 */
	void init(int h, int w) {
		H = 1;
		W = 1;
		while (H < h) H <<= 1;
		while (W < w) W <<= 1;
		seg.assign(4 * H * W, e());
		built = false;
	}

	/**
	 * @brief 値セット（build前: 代入 / build後: update）
	 */
	void set(int i, int j, const S& x) {
		if (!built) {
			seg[idx(i + H, j + W)] = x;
		}
		else {
			assign(i, j, x);
		}
	}

	/**
	 * @brief 全要素セット
	 */
	void set_all(const std::vector<std::vector<S>>& a) {
		int h = (int)a.size();
		int w = (int)a[0].size();
		for (int i = 0; i < h; ++i) {
			for (int j = 0; j < w; ++j) {
				seg[idx(i + H, j + W)] = a[i][j];
			}
		}
	}

	/**
	 * @brief 構築
	 */
	void build() {
		// 縦
		for (int j = W; j < 2 * W; j++) {
			for (int i = H - 1; i > 0; i--) {
				seg[idx(i, j)] = op(seg[idx(i << 1, j)], seg[idx(i << 1 | 1, j)]);
			}
		}
		// 横
		for (int i = 0; i < 2 * H; i++) {
			for (int j = W - 1; j > 0; j--) {
				seg[idx(i, j)] = op(seg[idx(i, j << 1)], seg[idx(i, j << 1 | 1)]);
			}
		}
		built = true;
	}

	/**
	 * @brief 点取得
	 */
	S get(int i, int j) const {
		return seg[idx(i + H, j + W)];
	}

	/**
	 * @brief 矩形クエリ [i1,i2) × [j1,j2)
	 */
	S prod(int i1, int j1, int i2, int j2) const {
		assert(built && "query() requires build()");
		if (i1 >= i2 || j1 >= j2) return e();

		S res = e();

		int l = i1 + H, r = i2 + H;
		int jl = j1 + W, jr = j2 + W;

		for (; l < r; l >>= 1, r >>= 1) {
			if (l & 1) res = op(res, inner_query(l++, jl, jr));
			if (r & 1) res = op(res, inner_query(--r, jl, jr));
		}
		return res;
	}

private:
	int idx(int i, int j) const {
		return i * 2 * W + j;
	}

	S inner_query(int node, int l, int r) const {
		S res = e();
		for (; l < r; l >>= 1, r >>= 1) {
			if (l & 1) res = op(res, seg[idx(node, l++)]);
			if (r & 1) res = op(res, seg[idx(node, --r)]);
		}
		return res;
	}

	/**
	 * @brief 点更新（代入）
	 */
	void assign(int i, int j, const S& x) {
		assert(built && "assign() requires build()");

		int hi = i + H;
		int hj = j + W;

		seg[idx(hi, hj)] = x;

		// 縦更新
		for (int v = hi >> 1; v > 0; v >>= 1) {
			seg[idx(v, hj)] = op(seg[idx(v << 1, hj)], seg[idx(v << 1 | 1, hj)]);
		}

		// 横更新
		for (; hi > 0; hi >>= 1) {
			for (int u = hj >> 1; u > 0; u >>= 1) {
				seg[idx(hi, u)] = op(seg[idx(hi, u << 1)], seg[idx(hi, u << 1 | 1)]);
			}
		}
	}
};

} // namespace kwm_t::segtree

#endif // KWM_T_SEGTREE_SEGTREE_2D_DENSE_HPP
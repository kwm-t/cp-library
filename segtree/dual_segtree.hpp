#ifndef KWM_T_SEGTREE_DUAL_SEGTREE_HPP
#define KWM_T_SEGTREE_DUAL_SEGTREE_HPP

#include <vector>
#include <cassert>

/**
 * @brief Dual Segment Tree（区間作用・一点取得）
 *
 * 区間に作用を適用し、各点の値を取得するデータ構造
 *
 * 典型用途:
 *   - 区間加算 + 点取得
 *   - 区間更新（後勝ち） + 点取得
 *
 * 計算量:
 *   - apply: O(log N)
 *   - get:   O(log N)
 *
 * @tparam S モノイド
 * @tparam op 作用の合成（左から適用）
 * @tparam e 単位元
 *
 * 制約 / 注意:
 *   - op は「後から適用するものが右」に来るように設計すること
 *   - 初期値は e() が入る
 *
 * 使用例:
 *   DualSegtree<S, op, e> seg(n);
 *   seg.apply(l, r, x);
 *   auto val = seg.get(i);
 *  verified
 *   https://atcoder.jp/contests/awc0047/submissions/74964021
 */

namespace kwm_t::segtree {

template<class S, S(*op)(S, S), S(*e)()>
struct DualSegtree {
private:
	int n = 0;        // 元のサイズ
	int size = 0;     // セグ木サイズ（2冪）
	int log = 0;      // 高さ
	std::vector<S> data;

	// 子に遅延を伝播
	void propagate(int k) {
		if (k >= size) return;
		data[k * 2] = op(data[k * 2], data[k]);
		data[k * 2 + 1] = op(data[k * 2 + 1], data[k]);
		data[k] = e();
	}

	// 根から k まで伝播
	void full_propagate(int k) {
		for (int h = log; h > 0; --h) {
			propagate(k >> h);
		}
	}

public:
	DualSegtree(int n_ = 0) : n(n_) {
		size = 1;
		log = 0;
		while (size < n) {
			size <<= 1;
			log++;
		}
		data.assign(size * 2, e());
	}

	DualSegtree(const std::vector<S>& v) : n((int)v.size()) {
		size = 1;
		log = 0;
		while (size < n) {
			size <<= 1;
			log++;
		}
		data.assign(size * 2, e());

		// 葉に初期値をセット
		for (int i = 0; i < n; i++) {
			data[size + i] = v[i];
		}
	}

	// 点の初期化（値をリセット）
	void point_init(int p) {
		assert(0 <= p && p < n);
		p += size;
		full_propagate(p);
		data[p] = e();
	}

	// 点設定
	void point_set(int p, S x) {
		assert(0 <= p && p < n);
		p += size;
		full_propagate(p);
		data[p] = x;
	}

	// 点取得
	S get(int p) {
		assert(0 <= p && p < n);
		p += size;
		full_propagate(p);
		return data[p];
	}

	// 区間 [l, r) に作用を適用
	void apply(int l, int r, S x) {
		assert(0 <= l && l <= r && r <= n);

		l += size;
		r += size;

		if (l != size) full_propagate(l - 1);
		if (r != size * 2) full_propagate(r);

		while (l < r) {
			if (l & 1) {
				data[l] = op(data[l], x);
				++l;
			}
			if (r & 1) {
				--r;
				data[r] = op(data[r], x);
			}
			l >>= 1;
			r >>= 1;
		}
	}
};

} // namespace kwm_t::segtree

#endif // KWM_T_SEGTREE_DUAL_SEGTREE_HPP
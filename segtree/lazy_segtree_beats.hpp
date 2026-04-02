#ifndef KWM_T_SEGTREE_LAZY_SEGTREE_BEATS_HPP
#define KWM_T_SEGTREE_LAZY_SEGTREE_BEATS_HPP

#include <bits/stdc++.h>

/**
 * @brief Segment Tree Beats (ACL風 lazy_segtree 拡張)
 *
 * mapping が (S, bool) を返し、失敗時に push することで
 * Segment Tree Beats を実現する
 *
 * 典型用途:
 *   - range chmin / chmax / add / sum
 *   - 区間制約付き更新（破壊的更新を伴うもの）
 *
 * 計算量:
 *   - amortized O(log N)
 *
 * @tparam S モノイド（ノード情報）
 * @tparam op S × S → S
 * @tparam e 単位元
 * @tparam F 遅延作用素
 * @tparam mapping (F, S) → (S, bool)
 *           bool = true のとき「このノードでは処理できない」
 * @tparam composition F ∘ F
 * @tparam id 単位作用素
 *
 * 制約 / 注意:
 *   - mapping が失敗するケースを正しく実装すること
 *   - composition は f(g(x)) の順
 *
 * 使用例:
 *   using Seg = lazy_segtree<...>;
 *   Seg seg(n);
 *
 * verified:
 *   - https://atcoder.jp/contests/abc430/submissions/74233474
 */

namespace kwm_t::segtree {

inline unsigned int bit_ceil(unsigned int n) {
	unsigned int x = 1;
	while (x < n) x <<= 1;
	return x;
}

inline int countr_zero(unsigned int n) {
#ifdef _MSC_VER
	unsigned long index;
	_BitScanForward(&index, n);
	return index;
#else
	return __builtin_ctz(n);
#endif
}

#if __cplusplus >= 201703L

template <class S,
	auto op,
	auto e,
	class F,
	auto mapping,
	auto composition,
	auto id>
	struct lazy_segtree

#else

template <class S,
	S(*op)(S, S),
	S(*e)(),
	class F,
	std::pair<S, bool>(*mapping)(F, S),
	F(*composition)(F, F),
	F(*id)()>
	struct lazy_segtree

#endif
{
public:
	lazy_segtree() : lazy_segtree(0) {}
	explicit lazy_segtree(int n) : lazy_segtree(std::vector<S>(n, e())) {}

	explicit lazy_segtree(const std::vector<S>& v) : _n((int)v.size()) {
		size = (int)bit_ceil((unsigned int)_n);
		log = countr_zero((unsigned int)size);
		d.assign(2 * size, e());
		lz.assign(size, id());

		for (int i = 0; i < _n; i++) d[size + i] = v[i];
		for (int i = size - 1; i >= 1; i--) update(i);
	}

	void set(int p, S x) {
		assert(0 <= p && p < _n);
		p += size;
		for (int i = log; i >= 1; i--) push(p >> i);
		d[p] = x;
		for (int i = 1; i <= log; i++) update(p >> i);
	}

	S get(int p) {
		assert(0 <= p && p < _n);
		p += size;
		for (int i = log; i >= 1; i--) push(p >> i);
		return d[p];
	}

	S prod(int l, int r) {
		assert(0 <= l && l <= r && r <= _n);
		if (l == r) return e();

		l += size;
		r += size;

		for (int i = log; i >= 1; i--) {
			if (((l >> i) << i) != l) push(l >> i);
			if (((r >> i) << i) != r) push((r - 1) >> i);
		}

		S sml = e(), smr = e();
		while (l < r) {
			if (l & 1) sml = op(sml, d[l++]);
			if (r & 1) smr = op(d[--r], smr);
			l >>= 1;
			r >>= 1;
		}

		return op(sml, smr);
	}

	S all_prod() { return d[1]; }

	void apply(int p, F f) {
		assert(0 <= p && p < _n);
		p += size;
		for (int i = log; i >= 1; i--) push(p >> i);
		d[p] = mapping(f, d[p]).first;
		for (int i = 1; i <= log; i++) update(p >> i);
	}

	void apply(int l, int r, F f) {
		assert(0 <= l && l <= r && r <= _n);
		if (l == r) return;

		l += size;
		r += size;

		for (int i = log; i >= 1; i--) {
			if (((l >> i) << i) != l) push(l >> i);
			if (((r >> i) << i) != r) push((r - 1) >> i);
		}

		int l2 = l, r2 = r;
		while (l < r) {
			if (l & 1) all_apply(l++, f);
			if (r & 1) all_apply(--r, f);
			l >>= 1;
			r >>= 1;
		}
		l = l2;
		r = r2;

		for (int i = 1; i <= log; i++) {
			if (((l >> i) << i) != l) update(l >> i);
			if (((r >> i) << i) != r) update((r - 1) >> i);
		}
	}

private:
	int _n, size, log;
	std::vector<S> d;
	std::vector<F> lz;

	void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }

	void all_apply(int k, F f) {
		auto[x, fail] = mapping(f, d[k]);
		d[k] = x;
		if (k < size) {
			lz[k] = composition(f, lz[k]);
			if (fail) {
				push(k);
				update(k);
			}
		}
	}

	void push(int k) {
		all_apply(2 * k, lz[k]);
		all_apply(2 * k + 1, lz[k]);
		lz[k] = id();
	}
};

} // namespace kwm_t::segtree::beats

#endif // KWM_T_SEGTREE_LAZY_SEGTREE_BEATS_HPP
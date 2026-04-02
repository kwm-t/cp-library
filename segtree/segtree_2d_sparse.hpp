#ifndef KWM_T_SEGTREE_SEGTREE_2D_SPARSE_HPP
#define KWM_T_SEGTREE_SEGTREE_2D_SPARSE_HPP

#include <vector>
#include <algorithm>

/**
 * @brief 2次元セグメント木（座標圧縮・静的）
 *
 * 縦方向にセグ木を持ち、各ノードに横方向のセグ木を持つ構造
 *
 * 典型用途:
 *   - 点更新 + 矩形クエリ
 *   - 座標圧縮が必要な2D range query
 *
 * 計算量:
 *   - build: O(N log N)
 *   - update: O(log^2 N)
 *   - query: O(log^2 N)
 *
 * @tparam S モノイドの型
 * @tparam op 二項演算 (S, S) -> S
 * @tparam e 単位元 () -> S
 * @tparam C 座標型（int / long long など）
 *
 * 制約 / 注意:
 *   - 事前に add_point() を呼び、build() が必要
 *   - update() は build() 後に使用
 *   - クエリは半開区間 [li, ri) × [lj, rj)
 *
 * 使用例:
 *   SegTree2D<ll, op, e, int> seg;
 *   seg.add_point(i, j);
 *   seg.build();
 *   seg.update(i, j, val);
 *   seg.query(li, lj, ri, rj);
 *
 * verified:
 *   https://atcoder.jp/contests/ABC266/submissions/74604159
 *   https://yukicoder.me/submissions/1156160
 */
namespace kwm_t::segtree {

template <class S, S(*op)(S, S), S(*e)(), class C>
class segtree_2d_sparse {
private:
	int H;                                   // 縦セグ木サイズ（2冪）
	std::vector<C> coord_i;                  // i座標
	std::vector<std::vector<C>> coord_j;     // 各ノードのj座標
	std::vector<std::vector<S>> seg;         // 各ノードの横seg木
	std::vector<int> size_j;                 // 横seg木サイズ

	std::vector<std::pair<C, C>> points;     // (i, j)

public:
	segtree_2d_sparse() = default;

	/**
	 * @brief 点の登録（build前）
	 */
	void add_point(C i, C j) {
		points.emplace_back(i, j);
	}

	/**
	 * @brief 構築
	 */
	void build() {
		// i座標圧縮
		for (auto&[i, j] : points) coord_i.push_back(i);
		std::sort(coord_i.begin(), coord_i.end());
		coord_i.erase(std::unique(coord_i.begin(), coord_i.end()), coord_i.end());

		H = 1 << ceil_pow2((int)coord_i.size());
		coord_j.resize(2 * H);
		seg.resize(2 * H);
		size_j.resize(2 * H);

		// 各ノードにjを配布
		for (auto&[i0, j0] : points) {
			int i = get_i(i0) + H;
			while (i > 0) {
				coord_j[i].push_back(j0);
				i >>= 1;
			}
		}

		// 各ノードでj圧縮＋seg木構築
		for (int node = 0; node < 2 * H; node++) {
			auto& v = coord_j[node];
			std::sort(v.begin(), v.end());
			v.erase(std::unique(v.begin(), v.end()), v.end());

			int sz = (int)v.size();
			int k = ceil_pow2(sz);
			size_j[node] = 1 << k;
			seg[node].assign(2 * size_j[node], e());
		}
	}

	/**
	 * @brief 点更新（モノイド演算）
	 */
	void set(C i0, C j0, S val) {
		int i = get_i(i0) + H;
		while (i > 0) {
			int j_idx = get_j(i, j0);
			int idx = j_idx + size_j[i];

			seg[i][idx] = op(seg[i][idx], val);

			while (idx >>= 1) {
				pull(i, idx);
			}
			i >>= 1;
		}
	}

	/**
	 * @brief 矩形クエリ [li, ri) × [lj, rj)
	 */
	S prod(C li, C lj, C ri, C rj) {
		int l = get_i(li) + H;
		int r = get_i(ri) + H;

		S res_l = e(), res_r = e();

		while (l < r) {
			if (l & 1) res_l = op(res_l, query_j(l++, lj, rj));
			if (r & 1) res_r = op(query_j(--r, lj, rj), res_r);
			l >>= 1;
			r >>= 1;
		}
		return op(res_l, res_r);
	}

private:
	// i座標取得
	int get_i(C i) const {
		return std::lower_bound(coord_i.begin(), coord_i.end(), i) - coord_i.begin();
	}

	// j座標取得
	int get_j(int node, C j) const {
		return std::lower_bound(coord_j[node].begin(), coord_j[node].end(), j) - coord_j[node].begin();
	}

	// 横方向クエリ
	S query_j(int node, C lj, C rj) {
		int l = std::lower_bound(coord_j[node].begin(), coord_j[node].end(), lj) - coord_j[node].begin();
		int r = std::lower_bound(coord_j[node].begin(), coord_j[node].end(), rj) - coord_j[node].begin();

		S res_l = e(), res_r = e();
		l += size_j[node];
		r += size_j[node];

		while (l < r) {
			if (l & 1) res_l = op(res_l, seg[node][l++]);
			if (r & 1) res_r = op(seg[node][--r], res_r);
			l >>= 1;
			r >>= 1;
		}
		return op(res_l, res_r);
	}

	// 親更新
	void pull(int node, int idx) {
		seg[node][idx] = op(seg[node][2 * idx], seg[node][2 * idx + 1]);
	}

	// ceil(log2(n))
	int ceil_pow2(int n) const {
		int x = 0;
		while ((1U << x) < (unsigned int)n) x++;
		return x;
	}
};

} // namespace kwm_t::segtree

#endif // KWM_T_SEGTREE_SEGTREE_2D_SPARSE_HPP
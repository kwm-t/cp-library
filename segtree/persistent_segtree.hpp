/**
 * @file persistent_segtree.hpp
 * @brief 永続 Segment Tree
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/segtree/persistent_segtree.hpp
 */

#ifndef KWM_T_DATA_SEGTREE_PERSISTENT_SEG_TREE_HPP
#define KWM_T_DATA_SEGTREE_PERSISTENT_SEG_TREE_HPP

#include <vector>

/**
 * @brief 永続セグメント木 (Persistent Segment Tree)
 *
 * 各更新ごとに新しいバージョンのrootを生成するセグメント木。
 * 過去の状態を保持したままクエリ可能。
 *
 * 典型用途:
 *   - 過去バージョンの配列に対する区間クエリ
 *   - k-th number（座標圧縮 + 二分探索）
 *   - rollback可能なデータ構造
 *
 * 計算量:
 *   build: O(N)
 *   update(set/apply): O(log N)
 *   prod: O(log N)
 *
 * @tparam S モノイドの型
 * @tparam op 二項演算 (結合律)
 * @tparam e 単位元
 *
 * @param root 操作対象のバージョン
 *
 * 制約 / 注意:
 *   - メモリは O(N + Q log N)
 *   - Nodeはnewされ続ける（deleteしない前提）
 *
 * 使用例:
 *   auto root0 = seg.build(a);
 *   auto root1 = seg.set(root0, 3, x);
 *   auto res = seg.prod(root1, l, r);
 *
 * verified:
 *   - 永続RMQ
 *   - k-th number
 */
namespace kwm_t::segtree {

template <class S, S(*op)(S, S), S(*e)()>
struct PersistentSegTree {
	struct Node {
		S data;
		Node *l, *r;
		Node(S v, Node* l = nullptr, Node* r = nullptr)
			: data(v), l(l), r(r) {}
	};

	int n;

	PersistentSegTree() : n(0) {}

	Node* build(const std::vector<S>& v) {
		n = (int)v.size();
		return build(0, n, v);
	}

	// 代入更新
	Node* set(Node* root, int idx, const S& x) const {
		return set(idx, x, root, 0, n);
	}

	// 作用更新: a[idx] = op(a[idx], x)
	Node* apply(Node* root, int idx, const S& x) const {
		return apply(idx, x, root, 0, n);
	}

	// 取得
	S get(Node* node, int index) const {
		int l = 0, r = n;
		while (l + 1 < r) {
			int m = (l + 1) >> 1;
			if (index < m) {
				node = node->l;
				r = m;
			}
			else {
				node = node->r;
				l = m;
			}
		}
		return node->data;
	}

	// 区間積 [l, r)
	S prod(Node* root, int l, int r) const {
		return prod(root, l, r, 0, n);
	}

	S all_prod(Node* root) const {
		return root->data;
	}

private:
	Node* build(int l, int r, const std::vector<S>& v) const {
		if (l + 1 == r) {
			return new Node(v[l]);
		}
		int m = (l + r) >> 1;
		Node* left = build(l, m, v);
		Node* right = build(m, r, v);
		return new Node(op(left->data, right->data), left, right);
	}

	Node* set(int idx, const S& x, Node* node, int l, int r) const {
		if (l + 1 == r) {
			return new Node(x);
		}
		int m = (l + r) >> 1;
		if (idx < m) {
			Node* nl = set(idx, x, node->l, l, m);
			return new Node(op(nl->data, node->r->data), nl, node->r);
		}
		else {
			Node* nr = set(idx, x, node->r, m, r);
			return new Node(op(node->l->data, nr->data), node->l, nr);
		}
	}

	Node* apply(int idx, const S& x, Node* node, int l, int r) const {
		if (l + 1 == r) {
			return new Node(op(node->data, x));
		}
		int m = (l + r) >> 1;
		if (idx < m) {
			Node* nl = apply(idx, x, node->l, l, m);
			return new Node(op(nl->data, node->r->data), nl, node->r);
		}
		else {
			Node* nr = apply(idx, x, node->r, m, r);
			return new Node(op(node->l->data, nr->data), node->l, nr);
		}
	}

	S prod(Node* node, int ql, int qr, int l, int r) const {
		if (qr <= l || r <= ql) return e();
		if (ql <= l && r <= qr) return node->data;
		int m = (l + r) >> 1;
		return op(
			prod(node->l, ql, qr, l, m),
			prod(node->r, ql, qr, m, r)
		);
	}
};

} // namespace

#endif
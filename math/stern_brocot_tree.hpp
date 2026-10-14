/**
 * @file stern_brocot_tree.hpp
 * @brief Stern-Brocot Tree
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/stern_brocot_tree.hpp
 */

#ifndef KWM_T_MATH_STERN_BROCOT_TREE_HPP
#define KWM_T_MATH_STERN_BROCOT_TREE_HPP

#include <vector>
#include <functional>
#include <algorithm>
#include <cstdlib>
#include "../base/integer.hpp"

namespace kwm_t::math {

/**
 * @brief Stern-Brocot Tree / 有理数木
 *
 * 典型用途:
 *   - 有理数探索
 *   - 分数最適化（ABC408-G系）
 *   - 連分数操作
 *   - 単調性判定付き探索
 *
 * 計算量:
 *   - O(log value) ~ O(depth)
 * verified
 *   - https://atcoder.jp/contests/abc333/submissions/74824200
 *   - https://atcoder.jp/contests/abc408/submissions/74824452
 */
template <typename T>
class SternBrocotTree {
public:
	struct Node {
		T p, q, r, s;

		Node(T p = 0, T q = 1, T r = 1, T s = 0)
			: p(p), q(q), r(r), s(s) {}

		static Node root() {
			return Node();
		}

		// 左 or 右へ d ステップ
		static Node child(Node node, T d, bool left) {
			if (left) {
				node.r += d * node.p;
				node.s += d * node.q;
			}
			else {
				node.p += d * node.r;
				node.q += d * node.s;
			}
			return node;
		}

		static Node parent(Node node) {
			if (node.p == 0 && node.q == 1 && node.r == 1 && node.s == 0) {
				return Node();
			}
			if (node.p < node.r || node.q < node.s) {
				node.r -= node.p;
				node.s -= node.q;
			}
			else {
				node.p -= node.r;
				node.q -= node.s;
			}
			return node;
		}

		T num() const { return p + r; }
		T den() const { return q + s; }

		bool operator<(const Node& o) const {
			return (int128)num() * o.den() < (int128)o.num() * den();
		}
		bool operator>(const Node& o) const { return o < *this; }
		bool operator==(const Node& o) const {
			return (int128)num() * o.den() == (int128)o.num() * den();
		}
		bool operator<=(const Node& o) const { return !(o < *this); }
		bool operator>=(const Node& o) const { return !(*this < o); }
	};

public:

	// p/q → 連分数
	// 1/1 から p/q へのパスが右に a_1 回、左に a_2 回、… としたときの a を返す
	// (a_1, …, a_k + 1) は p/q の連分数展開になっている
	static std::vector<T> encode_path(T p, T q) {
		std::vector<T> a;

		if (p < q) {
			a.push_back(0);
			std::swap(p, q);
		}

		while (p != 1) {
			a.push_back(p / q);
			p %= q;
			std::swap(p, q);
		}

		if (!a.empty()) {
			if (a.back() == 1) a.pop_back();
			else a.back()--;
		}
		return a;
	}

	// 連分数 → node
	// 1/1 から右に a_1 回、左に a_2 回、… 移動した頂点 (p, q, r, s) を返す
	// (a_1, …, a_k + 1) は (p+r)/(q+s) の連分数展開になっている
	static Node decode_path(const std::vector<T>& a) {
		Node node;
		for (int i = 0; i < (int)a.size(); i++) {
			node = Node::child(node, a[i], i & 1);
		}
		return node;
	}

	// LCA(p/q と r/s の LCA (f, g, h, k) を返す)
	static Node lca(T p, T q, T r, T s) {
		auto a = encode_path(p, q);
		auto b = encode_path(r, s);

		Node node;

		int n = std::min(a.size(), b.size());
		for (int i = 0; i < n; i++) {
			T c = std::min(a[i], b[i]);
			node = Node::child(node, c, i & 1);

			if (a[i] != b[i]) break;
		}
		return node;
	}

	// ancestor
	// p/q の祖先であって深さが d のノード (f, g, h, k) を返す
	// 存在しなければ (0, 0, 0, 0)
	static Node ancestor(T d, T p, T q) {
		auto a = encode_path(p, q);
		Node node;

		for (int i = 0; i < (int)a.size(); i++) {
			T c = std::min(d, a[i]);
			node = Node::child(node, c, i & 1);
			d -= c;
			if (d == 0) break;
		}
		return d == 0 ? node : Node(0, 0, 0, 0);
	}

	// range
	// p/q の子孫の下限 f/g と上限 h/k を返す
	// p/q に対応する頂点 (f, g, h, k) でもある
	static Node range(T p, T q) {
		return decode_path(encode_path(p, q));
	}

	// 単調判定探索
	// judge: [0, ∞] → {true, false}
	// judge は単調性を持つ、すなわち、ある実数 α を境に true と false が切り替わる
	// 分母・分子が max_value 以下の有理数のうち、α に最も近いもの (下側: p/q, 上側: r/s) を返す
	static Node search(const std::function<bool(T, T)>& judge, T maxv) {
		Node node;

		bool j01 = judge(0, 1);
		bool j10 = judge(1, 0);

		while (node.p + node.r <= maxv && node.q + node.s <= maxv) {
			bool cur = judge(node.num(), node.den());
			bool left = cur ^ j01;

			T maxd = max_step(node, maxv, left);

			T l = 1, r = 2;

			while (r <= maxd) {
				auto nxt = Node::child(node, r, left);
				if (judge(nxt.num(), nxt.den()) != cur) break;
				l = r;
				r = std::min<T>(r * 2, maxd + 1);
			}

			while (r - l > 1) {
				T m = (l + r) / 2;
				auto nxt = Node::child(node, m, left);
				if (judge(nxt.num(), nxt.den()) == cur) l = m;
				else r = m;
			}

			node = Node::child(node, l, left);
		}

		return node;
	}

private:
	static T max_step(const Node& node, T maxv, bool left) {
		if (left) {
			return node.p == 0 ? (maxv - node.s) / node.q
				: std::min((maxv - node.r) / node.p, (maxv - node.s) / node.q);
		}
		else {
			return node.s == 0 ? (maxv - node.p) / node.r
				: std::min((maxv - node.q) / node.s, (maxv - node.p) / node.r);
		}
	}
};

} // namespace kwm_t::math

#endif
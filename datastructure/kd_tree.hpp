/**
 * @file kd_tree.hpp
 * @brief KD Tree
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/datastructure/kd_tree.hpp
 */
#ifndef KWM_T_DATA_STRUCTURE_KD_TREE_HPP
#define KWM_T_DATA_STRUCTURE_KD_TREE_HPP

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <queue>
#include <utility>
#include <vector>

namespace kwm_t::data_structure {

/**
 * @brief 2次元点集合に対するKD-tree
 *
 * 各点 (x, y) に値 S を持たせ、以下の操作を提供する。
 *
 * - 矩形範囲に対する集約値の取得
 * - 矩形範囲への一括作用
 * - 個別の点の有効化・無効化
 * - 個別の点の値の更新
 * - 円形範囲内の点の列挙
 *
 * 内部ではKD-treeによる空間分割とlazy propagationを組み合わせている。
 *
 * 矩形範囲操作では、部分木のbounding boxがクエリ矩形に完全に
 * 含まれる場合、その部分木全体を一度に処理する。
 *
 * @tparam S 点の値および集約値の型
 * @tparam op 2つの集約値を結合する関数
 * @tparam e 集約値の単位元を返す関数
 * @tparam F lazy operationの型
 * @tparam mapping lazy operationを集約値に適用する関数
 * @tparam composition 2つのlazy operationを合成する関数
 * @tparam id lazy operationの単位元を返す関数
 *
 * @param point 点の座標
 * @param val 各点の初期値
 *
 * 制約 / 注意:
 * - 座標はlong longで管理する。
 * - 点の番号はコンストラクタに渡した順番で0-indexed。
 * - 矩形範囲は [sx, tx] x [sy, ty] の閉区間。
 * - offした点はquery/applyの対象から除外される。
 * - offした点の値は内部に保存され、onすると再び有効になる。
 * - updateした点がoff中の場合、on時に復元される値は更新されない。
 *   off中の値も更新したい場合は、その仕様に合わせて実装を変更する必要がある。
 *
 * 計算量:
 * - 構築: 期待 O(N log N)
 * - 矩形query/apply: KD-treeの空間分割に依存する。
 * - 点のon/off/update: O(log N) 程度
 * - getNearbyPoints: 出力点数およびKD-treeの枝刈り効率に依存する。
 *
 * 使用例:
 * @code
 * using S = long long;
 * using F = long long;
 *
 * S op(S l, S r) {
 *     return l + r;
 * }
 *
 * S e() {
 *     return 0;
 * }
 *
 * S mapping(F f, S x, int size) {
 *     return x + f * size;
 * }
 *
 * F composition(F f, F g) {
 *     return f + g;
 * }
 *
 * F id() {
 *     return 0;
 * }
 *
 * std::vector<std::pair<long long, long long>> point = {
 *     {1, 2},
 *     {3, 4},
 *     {5, 6},
 * };
 *
 * std::vector<long long> value = {10, 20, 30};
 *
 * KdTree<S, op, e, F, mapping, composition, id> kd(point, value);
 *
 * kd.apply(0, 4, 0, 5, 10);
 * long long sum = kd.query(0, 4, 0, 5);
 * @endcode
 *
 * 参考元
 *	https://atcoder.jp/contests/awc0163/submissions/79456118
 * verified:
 *  https://atcoder.jp/contests/awc0163/submissions/79457724
 */
template <
	class S,
	S(*op)(S, S),
	S(*e)(),
	class F,
	S(*mapping)(F, S, int),
	F(*composition)(F, F),
	F(*id)()
>
class KdTree {
private:
	static constexpr long long INF = (1LL << 60);
	static constexpr long long NINF = -INF;

	struct Point {
		long long x;
		long long y;
		S val;

		Point() = default;

		Point(long long x, long long y, S val)
			: x(x), y(y), val(val) {}
	};

	struct Node {
		long long min_x;
		long long max_x;
		long long min_y;
		long long max_y;

		int size;
		int index;

		S sum;
		F lazy;

		Node()
			: min_x(INF),
			max_x(NINF),
			min_y(INF),
			max_y(NINF),
			size(0),
			index(-1),
			sum(e()),
			lazy(id()) {}

		Node(Point p, int index = -1)
			: min_x(p.x),
			max_x(p.x),
			min_y(p.y),
			max_y(p.y),
			size(1),
			index(index),
			sum(p.val),
			lazy(id()) {}
	};

	int _n;
	int size;
	int log;

	std::vector<Node> nodes;

	// info[k].first:
	//   点kがoffされたときに保存しておく値
	//
	// info[k].second:
	//   KD-tree上での葉の位置
	std::vector<std::pair<S, int>> info;

	void pull(int k) {
		if (k == 0) {
			return;
		}

		nodes[k].size =
			nodes[2 * k].size +
			nodes[2 * k + 1].size;

		nodes[k].min_x =
			std::min(nodes[2 * k].min_x, nodes[2 * k + 1].min_x);

		nodes[k].max_x =
			std::max(nodes[2 * k].max_x, nodes[2 * k + 1].max_x);

		nodes[k].min_y =
			std::min(nodes[2 * k].min_y, nodes[2 * k + 1].min_y);

		nodes[k].max_y =
			std::max(nodes[2 * k].max_y, nodes[2 * k + 1].max_y);

		nodes[k].sum =
			op(nodes[2 * k].sum, nodes[2 * k + 1].sum);
	}

	void all_apply(int k, F f) {
		nodes[k].sum =
			mapping(f, nodes[k].sum, nodes[k].size);

		if (k < size) {
			nodes[k].lazy =
				composition(f, nodes[k].lazy);
		}
	}

	void push(int k) {
		all_apply(2 * k, nodes[k].lazy);
		all_apply(2 * k + 1, nodes[k].lazy);
		nodes[k].lazy = id();
	}

	/**
	 * @brief KD-treeを初期化する。
	 *
	 * @param point 点の座標
	 * @param val 各点の初期値
	 */
	void init(
		const std::vector<std::pair<long long, long long>>& point,
		const std::vector<S>& val
	) {
		assert(point.size() == val.size());

		size = 1;
		log = 0;

		while (size < _n) {
			size *= 2;
			++log;
		}

		nodes.resize(size * 2);
		info.resize(size, { e(), -1 });

		std::vector<std::pair<Point, int>> pi(
			size,
			{ Point{}, -1 }
		);

		for (int i = 0; i < _n; ++i) {
			pi[i] = {
				Point{point[i].first, point[i].second, val[i]},
				i
			};
		}

		build(pi, 0, size);

		for (int i = size - 1; i >= 1; --i) {
			pull(i);
		}
	}

	void build(
		std::vector<std::pair<Point, int>>& v,
		int l,
		int r,
		bool div_x = true
	) {
		const int len = r - l;

		if (len == 1) {
			if (v[l].second != -1) {
				nodes[size + l] = Node(v[l].first, v[l].second);
				info[v[l].second].second = l;
			}

			return;
		}

		const int m = (l + r) / 2;

		if (div_x) {
			std::nth_element(
				v.begin() + l,
				v.begin() + m,
				v.begin() + r,
				[](const std::pair<Point, int>& p,
					const std::pair<Point, int>& q) {
				return p.first.x < q.first.x;
			}
			);
		}
		else {
			std::nth_element(
				v.begin() + l,
				v.begin() + m,
				v.begin() + r,
				[](const std::pair<Point, int>& p,
					const std::pair<Point, int>& q) {
				return p.first.y < q.first.y;
			}
			);
		}

		build(v, l, m, !div_x);
		build(v, m, r, !div_x);
	}

public:
	/**
	 * @brief 座標のみを指定して構築する。
	 *
	 * 全点の初期値は e() になる。
	 *
	 * @param point 点の座標
	 */
	explicit KdTree(
		const std::vector<std::pair<long long, long long>>& point
	) : _n(static_cast<int>(point.size())) {
		std::vector<S> val(_n, e());
		init(point, val);
	}

	/**
	 * @brief 座標と初期値を指定して構築する。
	 *
	 * @param point 点の座標
	 * @param val 各点の初期値
	 */
	KdTree(
		const std::vector<std::pair<long long, long long>>& point,
		const std::vector<S>& val
	) : _n(static_cast<int>(point.size())) {
		init(point, val);
	}

	/**
	 * @brief 点kを有効化する。
	 *
	 * 既に有効な場合は何もしない。
	 *
	 * @param k 点の番号
	 */
	void on(int k) {
		assert(0 <= k && k < _n);

		const int p = info[k].second + size;

		for (int i = log; i >= 1; --i) {
			push(p >> i);
		}

		if (nodes[p].size == 1) {
			return;
		}

		nodes[p].size = 1;
		nodes[p].sum = info[k].first;

		for (int i = 1; i <= log; ++i) {
			pull(p >> i);
		}
	}

	/**
	 * @brief 点kを無効化する。
	 *
	 * 既に無効な場合は何もしない。
	 *
	 * @param k 点の番号
	 */
	void off(int k) {
		assert(0 <= k && k < _n);

		const int p = info[k].second + size;

		for (int i = log; i >= 1; --i) {
			push(p >> i);
		}

		if (nodes[p].size == 0) {
			return;
		}

		info[k].first = nodes[p].sum;
		nodes[p].size = 0;
		nodes[p].sum = e();

		for (int i = 1; i <= log; ++i) {
			pull(p >> i);
		}
	}

	/**
	 * @brief 点kの値をxに変更する。
	 *
	 * 点が無効状態でも値自体は変更されるが、
	 * 現在の実装ではoff中に変更した値はon時の復元値には反映されない。
	 *
	 * @param k 点の番号
	 * @param x 新しい値
	 */
	void update(int k, const S& x) {
		assert(0 <= k && k < _n);

		const int p = info[k].second + size;

		for (int i = log; i >= 1; --i) {
			push(p >> i);
		}

		nodes[p].sum = x;

		for (int i = 1; i <= log; ++i) {
			pull(p >> i);
		}
	}

	/**
	 * @brief 矩形範囲に対して作用を適用する。
	 *
	 * [sx, tx] x [sy, ty] に含まれる有効な点すべてに
	 * fを適用する。
	 *
	 * @param sx x座標の下限
	 * @param tx x座標の上限
	 * @param sy y座標の下限
	 * @param ty y座標の上限
	 * @param f 適用する作用
	 */
	void apply(
		long long sx,
		long long tx,
		long long sy,
		long long ty,
		const F& f
	) {
		if (sx > tx || sy > ty) {
			return;
		}

		std::queue<int> que;
		std::vector<int> st;

		que.push(1);

		while (!que.empty()) {
			const int p = que.front();
			que.pop();

			if (
				nodes[p].size == 0 ||
				nodes[p].max_x < sx ||
				tx < nodes[p].min_x ||
				nodes[p].max_y < sy ||
				ty < nodes[p].min_y
				) {
				continue;
			}

			if (
				sx <= nodes[p].min_x &&
				nodes[p].max_x <= tx &&
				sy <= nodes[p].min_y &&
				nodes[p].max_y <= ty
				) {
				all_apply(p, f);
				continue;
			}

			push(p);

			que.push(2 * p);
			que.push(2 * p + 1);
			st.push_back(p);
		}

		while (!st.empty()) {
			pull(st.back());
			st.pop_back();
		}
	}

	/**
	 * @brief 矩形範囲の集約値を取得する。
	 *
	 * [sx, tx] x [sy, ty] に含まれる有効な点の値を
	 * opで集約して返す。
	 *
	 * @param sx x座標の下限
	 * @param tx x座標の上限
	 * @param sy y座標の下限
	 * @param ty y座標の上限
	 * @return 矩形内の集約値
	 */
	S query(
		long long sx,
		long long tx,
		long long sy,
		long long ty
	) {
		if (sx > tx || sy > ty) {
			return e();
		}

		std::queue<int> que;
		que.push(1);

		S res = e();

		while (!que.empty()) {
			const int p = que.front();
			que.pop();

			if (
				nodes[p].size == 0 ||
				nodes[p].max_x < sx ||
				tx < nodes[p].min_x ||
				nodes[p].max_y < sy ||
				ty < nodes[p].min_y
				) {
				continue;
			}

			if (
				sx <= nodes[p].min_x &&
				nodes[p].max_x <= tx &&
				sy <= nodes[p].min_y &&
				nodes[p].max_y <= ty
				) {
				res = op(res, nodes[p].sum);
				continue;
			}

			push(p);

			que.push(2 * p);
			que.push(2 * p + 1);
		}

		return res;
	}

	/**
	 * @brief 点と円の内外判定を行う。
	 *
	 * @param dx x方向の距離
	 * @param dy y方向の距離
	 * @param dist 半径
	 * @return 円の内部または円周上ならtrue
	 */
	static bool inside(
		long long dx,
		long long dy,
		long long dist
	) {
		return dx * dx + dy * dy <= dist * dist;
	}

	/**
	 * @brief 指定した円の内部にある点を列挙する。
	 *
	 * 中心 (x, y)、半径 dist の円
	 *
	 *   (X - x)^2 + (Y - y)^2 <= dist^2
	 *
	 * に含まれる有効な点kについて f(k)を呼び出す。
	 *
	 * @tparam T コールバックの型
	 * @param x 円の中心のx座標
	 * @param y 円の中心のy座標
	 * @param dist 円の半径
	 * @param f 点番号を受け取るコールバック
	 */
	template <typename T>
	void getNearbyPoints(
		long long x,
		long long y,
		long long dist,
		const T& f
	) {
		std::queue<int> que;
		que.push(1);

		while (!que.empty()) {
			const int p = que.front();
			que.pop();

			if (
				nodes[p].size == 0 ||
				!inside(
					std::clamp(x, nodes[p].min_x, nodes[p].max_x) - x,
					std::clamp(y, nodes[p].min_y, nodes[p].max_y) - y,
					dist
				)
				) {
				continue;
			}

			if (p >= size) {
				f(nodes[p].index);
				continue;
			}

			que.push(2 * p);
			que.push(2 * p + 1);
		}
	}
};

} // namespace kwm_t::data_structure

#endif // KWM_T_DATA_STRUCTURE_KD_TREE_HPP
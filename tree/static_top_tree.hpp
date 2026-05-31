#ifndef KWM_T_TREE_STATIC_TOP_TREE_HPP
#define KWM_T_TREE_STATIC_TOP_TREE_HPP

#include <vector>
#include <algorithm>

namespace kwm_t::tree {
/**
 * @brief 静的トップツリー（Static Top Tree）
 *
 * 典型用途:
 *   - 木上のパスクエリ（動的DP風の集約）
 *   - 部分木・パスのマージ構造の高速処理
 *   - HLD + 木DPの抽象化
 *
 * 概要:
 *   本実装は Static Top Tree を用いて木を二分木構造に変換し、
 *   compress / rake / addVertex / addEdge の4種演算でDPを構成する。
 *
 *   各頂点は Path / Point に変換され、
 *   上位ノードで merge されることで木全体の情報を管理する。
 *
 * 計算量:
 *   - 構築: O(N)
 *   - 更新: O(log N)
 *
 * @tparam Path
 *   パス情報（頂点列の集約状態）
 *
 * @tparam Point
 *   サブツリー情報（枝側の集約状態）
 *
 * @tparam vertex
 *   単一頂点から Path を生成
 *
 * @tparam compress
 *   パス同士の結合（heavy path）
 *
 * @tparam rake
 *   サブツリー同士の結合（light edge 側）
 *
 * @tparam addEdge
 *   Path → Point 変換（辺を介した変換）
 *
 * @tparam addVertex
 *   Point → Path 変換（頂点付加）
 *
 * 制約 / 注意:
 *   - 木構造であること（無向・連結・非循環）
 *   - vertex / compress / rake / add 系は純関数的であることが望ましい
 *   - 内部構造は破壊的に再構築されるため外部参照禁止
 *
 * 使用例:
 *   - 木DP（最大値・和・最小値）
 *   - パス上クエリ（合成モノイド）
 *   - 動的木クエリの基礎構造
 *
 * verified:
 *   - https://atcoder.jp/contests/abc351/submissions/74913979
 *   - https://atcoder.jp/contests/abc460/submissions/76299920
 */
template <
	typename Path,
	typename Point,
	Path(*vertex)(int i),
	Path(*compress)(Path&, Path&),
	Point(*rake)(Point&, Point&),
	Point(*addEdge)(Path&),
	Path(*addVertex)(Point&, int)
>
class StaticTopTree {
public:
	StaticTopTree(const std::vector<std::vector<int>>& g)
		: size(g.size()), graph(g), vtoi(g.size(), -1) {}

	void init(int root = 0) {
		rootTree = root;

		hldDfs(rootTree);

		Result r = doCompress(rootTree);
		rootNodes = r.nodeIndex;

		paths.resize(nodes.size());
		points.resize(nodes.size());
		initDfs(rootNodes);
	}

	Point update(int v) {
		int i = vtoi[v];
		while (-1 != i) {
			doUpdate(i);
			i = nodes[i].parent;
		}
		return paths[rootNodes].toPoint();
	}

private:
	enum Type {
		NONE,
		VERTEX,
		COMPRESS,
		RAKE,
		ADD_EDGE,
		ADD_VERTEX
	};

	struct Node {
		Type type;
		int id;
		int left;
		int right;
		int parent;
		int vIndex;

		Node()
			: type(NONE), id(-1), left(-1), right(-1), parent(-1), vIndex(-1) {}

		Node(Type type, int id, int left, int right, int parent = -1, int vIndex = -1)
			: type(type), id(id), left(left), right(right), parent(parent), vIndex(vIndex) {}
	};

	struct Result {
		int nodeIndex, size;
	};

	int size;
	std::vector<std::vector<int>> graph;

	std::vector<Node> nodes;
	std::vector<Point> points;
	std::vector<Path> paths;

	std::vector<int> vtoi; // vertex -> node index

	int rootTree = 0;
	int rootNodes = 0;

	int hldDfs(int v, int p = -1) {
		int res = 1;

		for (int &u : graph[v]) {
			if (p == u) {
				std::swap(u, graph[v].back());
				graph[v].pop_back();
				break;
			}
		}

		int best = 0;

		for (int &u : graph[v]) {
			int r = hldDfs(u, v);
			res += r;

			if (best < r) {
				best = r;
				std::swap(graph[v][0], u);
			}
		}

		return res;
	}

	int addNode(Type type, int left, int right) {
		int id = (int)nodes.size();
		nodes.emplace_back(type, id, left, right);

		if (-1 != left) nodes[left].parent = id;
		if (-1 != right) nodes[right].parent = id;

		return id;
	}

	Result marge(Type type, std::vector<Result> cs) {
		if (1 == cs.size()) return cs[0];

		int size = 0;
		for (const auto &c : cs) size += c.size;

		for (int i = 0; i < (int)cs.size(); ++i) {
			if (size < cs[i].size) {
				auto resL = marge(type, std::vector<Result>(cs.begin(), cs.begin() + i));
				auto resR = marge(type, std::vector<Result>(cs.begin() + i, cs.end()));
				return {
					addNode(type, resL.nodeIndex, resR.nodeIndex),
					resL.size + resR.size
				};
			}
			size -= cs[i].size * 2;
		}

		assert(false);
	}

	Result doCompress(int v) {
		std::vector<Result> cs{ doAddVertex(v) };

		while (graph[v].size()) {
			cs.emplace_back(doAddVertex(v = graph[v][0]));
		}

		return marge(Type::COMPRESS, cs);
	}

	Result doRake(int v) {
		std::vector<Result> cs;

		for (int i = 1; i < (int)graph[v].size(); ++i) {
			cs.emplace_back(doAddEdge(graph[v][i]));
		}

		return marge(Type::RAKE, cs);
	}

	Result doAddEdge(int v) {
		Result r = doCompress(v);
		return { addNode(Type::ADD_EDGE, r.nodeIndex, -1), r.size };
	}

	Result doAddVertex(int v) {
		if (graph[v].size() <= 1) {
			return doVertex(v);
		}

		Result r = doRake(v);
		int ni = addNode(Type::ADD_VERTEX, r.nodeIndex, -1);

		nodes[ni].vIndex = v;
		vtoi[v] = ni;

		return { ni, r.size + 1 };
	}

	Result doVertex(int v) {
		int ni = addNode(Type::VERTEX, -1, -1);

		nodes[ni].vIndex = v;
		vtoi[v] = ni;

		return { ni, 1 };
	}

	void initDfs(int ni) {
		if (-1 != nodes[ni].left) initDfs(nodes[ni].left);
		if (-1 != nodes[ni].right) initDfs(nodes[ni].right);
		doUpdate(ni);
	}

	void doUpdate(int nv) {
		if (Type::COMPRESS == nodes[nv].type) {
			paths[nv] = compress(
				paths[nodes[nv].left],
				paths[nodes[nv].right]
			);
		}
		else if (Type::RAKE == nodes[nv].type) {
			points[nv] = rake(
				points[nodes[nv].left],
				points[nodes[nv].right]
			);
		}
		else if (Type::ADD_EDGE == nodes[nv].type) {
			points[nv] = addEdge(paths[nodes[nv].left]);
		}
		else if (Type::ADD_VERTEX == nodes[nv].type) {
			paths[nv] = addVertex(points[nodes[nv].left], nodes[nv].vIndex);
		}
		else if (Type::VERTEX == nodes[nv].type) {
			paths[nv] = vertex(nodes[nv].vIndex);
		}
	}
};

} // namespace kwm_t::tree

#endif // KWM_T_TREE_STATIC_TOP_TREE_HPP
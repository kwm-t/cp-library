#include <vector>
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
	StaticTopTree(int n) :size(n), graph(n), vtoi(n, -1) {}
	// 辺の追加
	void add(int a, int b) {
		graph[a].push_back(b);
		graph[b].push_back(a);
	}
	// 木の初期化
	void init(int root = 0) {
		rootTree = root;
		// HL分解、子から親に伸びる辺を削除する
		hldDfs(rootTree);

		// 木の生成過程を生成する
		Result r = doCompress(rootTree);
		rootNodes = r.nodeIndex;

		// Path/Pointの初期化
		paths.resize(nodes.size());
		points.resize(nodes.size());
		initDfs(rootNodes);
	}
	// 木の頂点の更新
	Point update(int v) {
		int i = vtoi[v];
		while (-1 != i) {
			doUpdate(i);
			i = nodes[i].parent;
		}
		return paths[rootNodes].toPoint();
	}
private:
	enum Type { NONE, VERTEX, COMPRESS, RAKE, ADD_EDGE, ADD_VERTEX, };
	struct Node {
		Type type;
		int id, left, right, parent, vIndex;
		Node() {}
		Node(Type type = NONE, int id = -1, int left = -1, int right = -1, int parent = -1, int vIndex = -1) :
			type(type), id(id), left(left), right(right), parent(parent), vIndex(vIndex) {}
	};
	struct Result {
		int nodeIndex, size;
	};

	int size;
	std::vector<std::vector<int>>graph;
	std::vector<Node>nodes;
	std::vector<Point> points;
	std::vector<Path> paths;
	std::vector<int> vtoi;	//頂点番号からNode番号への変換
	int rootTree = 0;			// 木の根の頂点番号
	int rootNodes = 0;		// ノードの根の番号

	int hldDfs(int v, int p = -1) {
		int res = 1;
		// 親への辺を取り除く
		for (int &u : graph[v]) if (p == u) {
			std::swap(u, graph[v].back());
			graph[v].pop_back();
			break;
		}
		int best = 0;
		// heavy_edgeを先頭に
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
		int id = nodes.size();
		nodes.emplace_back(type, id, left, right);
		if (-1 != left) nodes[left].parent = id;
		if (-1 != right) nodes[right].parent = id;
		return id;
	}

	Result marge(Type type, std::vector<Result> cs) {
		if (1 == cs.size()) return cs[0];
		int size = 0;
		for (const auto &c : cs) size += c.size;
		for (int i = 0; i < cs.size(); ++i) {
			if (size < cs[i].size) {
				auto resL = marge(type, std::vector<Result>(cs.begin(), cs.begin() + i));
				auto resR = marge(type, std::vector<Result>(cs.begin() + i, cs.end()));
				return { addNode(type,resL.nodeIndex,resR.nodeIndex),resL.size + resR.size };
			}
			size -= cs[i].size * 2;
		}
	}

	Result doCompress(int v) {
		std::vector<Result> cs{ doAddVertex(v) };
		// 葉にたどり着くまで行う
		while (graph[v].size()) cs.emplace_back(doAddVertex(v = graph[v][0]));
		return marge(Type::COMPRESS, cs);
	}
	Result doRake(int v) {
		std::vector<Result>cs;
		for (int i = 1; i < graph[v].size(); ++i) {
			cs.emplace_back(doAddEdge(graph[v][i]));
		}
		return marge(Type::RAKE, cs);
	}
	Result doAddEdge(int v) {
		Result r = doCompress(v);
		return { addNode(Type::ADD_EDGE,r.nodeIndex, -1),r.size };
	}
	Result doAddVertex(int v) {
		if (graph[v].size() <= 1) {
			return doVertex(v);
		}
		Result r = doRake(v);
		int ni = addNode(Type::ADD_VERTEX, r.nodeIndex, -1);
		nodes[ni].vIndex = v;
		vtoi[v] = ni;
		return { ni,r.size + 1 };
	}
	Result doVertex(int v) {
		int ni = addNode(Type::VERTEX, -1, -1);
		nodes[ni].vIndex = v;
		vtoi[v] = ni;
		return { ni,1 };
	}
	void initDfs(int ni) {
		if (-1 != nodes[ni].left)initDfs(nodes[ni].left);
		if (-1 != nodes[ni].right)initDfs(nodes[ni].right);
		doUpdate(ni);
	}
	void doUpdate(int nv) {
		if (Type::COMPRESS == nodes[nv].type) {
			paths[nv] = compress(paths[nodes[nv].left], paths[nodes[nv].right]);
		}
		else if (Type::RAKE == nodes[nv].type) {
			points[nv] = rake(points[nodes[nv].left], points[nodes[nv].right]);
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
//https://github.com/blue-jam/ProconLibrary/blob/4d1c55deed0a1f39862a72b03d4e18519cd2680b/datastructure/statictoptree.hpp
namespace abc351G {
	std::vector<mint>A;
	struct Point;
	struct Path {
		mint a, b;
		Path(mint a = 1, mint b = 0) :a(a), b(b) {}
		Path operator+(const Path&p)const {
			return Path(a * p.a, b + a * p.b);
		}
		Point toPoint() const;
	};
	struct Point {
		mint x;
		Point(mint x = 1) :x(x) {}
		Point operator+(const Point&p)const {
			return Point(x * p.x);
		}
		Path toPath(mint v)const {
			return Path(x, v);
		}
	};
	Point Path::toPoint() const { return Point(b); }

	Path vertex(int i) {
		return Path{ 1, A[i] };
	}
	Path compress(Path &a, Path &b) {
		return a + b;
	}
	Point rake(Point &a, Point &b) {
		return a + b;
	}
	Point addEdge(Path &a) {
		return a.toPoint();
	}
	Path addVertex(Point &a, int i) {
		return a.toPath(A[i]);
	}
};
/*
使用例
https://atcoder.jp/contests/abc351/tasks/abc351_g
//https://github.com/blue-jam/ProconLibrary/blob/4d1c55deed0a1f39862a72b03d4e18519cd2680b/datastructure/statictoptree.hpp
namespace abc351G {
	vector<mint>A;
	struct Point;
	struct Path {
		mint a, b;
		Path(mint a = 1, mint b = 0) :a(a), b(b) {}
		Path operator+(const Path&p)const {
			return Path(a * p.a, b + a * p.b);
		}
		Point toPoint() const;
	};
	struct Point {
		mint x;
		Point(mint x = 1) :x(x) {}
		Point operator+(const Point&p)const {
			return Point(x * p.x);
		}
		Path toPath(mint v)const {
			return Path(x, v);
		}
	};
	Point Path::toPoint() const { return Point(b); }

	Path vertex(int i) {
		return Path{ 1, A[i] };
	}
	Path compress(Path &a, Path &b) {
		return a + b;
	}
	Point rake(Point &a, Point &b) {
		return a + b;
	}
	Point addEdge(Path &a) {
		return a.toPoint();
	}
	Path addVertex(Point &a, int i) {
		return a.toPath(A[i]);
	}
};
using namespace abc351G;
*/
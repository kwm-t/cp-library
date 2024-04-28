#include <vector>
template <typename Vertex, typename Point, typename Path>
class StaticTopTree {
public:
	StaticTopTree(int n) :n(n), to(n), vertexs(n), vtoi(n) {}
	void add(int a, int b) {
		to[a].push_back(b);
		to[b].push_back(a);
	}
	void init(int root = 0) {
		dfs(root);
		compress(root);
		paths.resize(nodes.size());
		points.resize(nodes.size());
		for (int i = 0; i < nodes.size(); ++i) update(i);
	}
	void preSet(int v, Vertex val) {
		vertexs[v] = val;
	}
	Point set(int v, Vertex val) {
		vertexs[v] = val;
		int i = vtoi[v];
		while (-1 != i) i = update(i);
		return paths[nodes.size() - 1].toPoint();
	}
private:
	enum Type : char { Comp, AddV, Rake, AddE, None };
	struct Node {
		Type type;
		int l, r, p;
		Node() {}
		Node(Type type, int l = -1, int r = -1, int p = -1) :
			type(type), l(l), r(r), p(p) {}
	};
	int n;
	std::vector<std::vector<int>>to;
	std::vector<Node>nodes;
	std::vector<int> vtoi;
	std::vector<Vertex> vertexs;
	std::vector<Path> paths;
	std::vector<Point> points;

	int dfs(int v, int p = -1) {
		int res = 1;
		// 親への辺を取り除く
		for (int &u : to[v]) if (p == u) {
			std::swap(u, to[v].back());
			to[v].pop_back();
			break;
		}
		int best = 0;
		// heavy_edgeを先頭に
		for (int &u : to[v]) {
			int r = dfs(u, v);
			res += r;
			if (best < r) {
				best = r;
				std::swap(to[v][0], u);
			}
		}
		return res;
	}
	int addNode(Type type, int l, int r = -1) {
		int v = nodes.size();
		nodes.emplace_back(type, l, r);
		if (-1 != l)nodes[l].p = v;
		if (Type::AddV != type && -1 != r)nodes[r].p = v;
		return v;
	}
	std::pair<int, int> marge(Type type, const std::vector<std::pair<int, int>> &cs) {
		if (1 == cs.size())return cs[0];
		int s = 0;
		for (auto c : cs) s += c.second;
		for (int i = 0; i < cs.size(); ++i) {
			if (s < cs[i].second) {
				auto[l, sl] = marge(type, std::vector<std::pair<int, int>>(cs.begin(), cs.begin() + i));
				auto[r, sr] = marge(type, std::vector<std::pair<int, int>>(cs.begin() + i, cs.end()));
				return { addNode(type,l,r),sl + sr };
			}
			s -= cs[i].second * 2;
		}
	}
	std::pair<int, int> compress(int v) {
		std::vector<std::pair<int, int>> cs{ addVertex(v) };
		// 葉にたどり着くまで行う
		while (to[v].size()) cs.emplace_back(addVertex(v = to[v][0]));
		return marge(Type::Comp, cs);
	}
	std::pair<int, int> addVertex(int v) {
		auto[i, s] = rake(v);
		return { vtoi[v] = addNode(Type::AddV,i,v),s + 1 };
	}
	std::pair<int, int> rake(int v) {
		std::vector<std::pair<int, int>>cs;
		for (int i = 1; i < to[v].size(); ++i) {
			cs.emplace_back(addEdge(to[v][i]));
		}
		if (cs.empty())return { -1,0 };
		return marge(Type::Rake, cs);
	}
	std::pair<int, int> addEdge(int v) {
		auto[i, s] = compress(v);
		return { addNode(Type::AddE,i),s };
	}
	int update(int i) {
		Node& node = nodes[i];
		if (Type::Comp == node.type) {
			paths[i] = paths[node.l] + paths[node.r];
		}
		else if (Type::AddV == node.type) {
			paths[i] = (node.l == -1 ? Point() : points[node.l]).toPath(vertexs[node.r]);
		}
		else if (Type::Rake == node.type) {
			points[i] = points[node.l] + points[node.r];
		}
		else if (Type::AddE == node.type) {
			points[i] = paths[node.l].toPoint();
		}
		return node.p;
	}
};
/*
使用例
https://atcoder.jp/contests/abc351/tasks/abc351_g
using Vertex = mint;
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
	Path toPath(Vertex v)const {
		return Path(x, v);
	}
};
*/
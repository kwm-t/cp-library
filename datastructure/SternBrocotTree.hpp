using int128 = __int128;// 提出用
//using int128 = long long;
template<class T>
class SternBrocotTree {
	// https://miscalc.hatenablog.com/entry/2023/12/22/213007
	// root =(0,1,1,0)
public:
	struct Node {
		T p, q, r, s;
		Node(T p = 0, T q = 1, T r = 1, T s = 0) :p(p), q(q), r(r), s(s) {}
		// (p,q,r,s) から (isleft ? 左 : 右) に d 個進んだ頂点を返す
		Node root() {
			return Node();
		}
		static Node child(Node node, T d, bool isleft) {
			if (isleft) {
				node.r += d * node.p;
				node.s += d * node.q;
			}
			else {
				node.p += d * node.r;
				node.q += d * node.s;
			}
			return node;
		}
		// (p,q,r,s) の親を返す
		// (p,q,r,s) が根なら (0,0,0,0) を返す
		static Node parent(Node node) {
			if (node.p == 0 && node.q == 1 && node.r == 1 && node.s == 0) {
				return Node();
			}
			if (node.p < node.r || node.q < node.s) {
				node.r -= node.p, node.s -= node.q;
			}
			else {
				node.p -= node.r, node.q -= node.s;
			}
			return node;
		}
		bool operator<(const Node& other) const {
			auto num = p + r;
			auto den = q + s;
			auto num2 = other.p + other.r;
			auto den2 = other.q + other.s;
			return (int128)num * den2 < (int128)num2 * den;
		}
		bool operator>(const Node& other) const {
			auto num = p + r;
			auto den = q + s;
			auto num2 = other.p + other.r;
			auto den2 = other.q + other.s;
			return (int128)num * den2 > (int128)num2 * den;
		}
		bool operator==(const Node& other) const {
			auto num = p + r;
			auto den = q + s;
			auto num2 = other.p + other.r;
			auto den2 = other.q + other.s;
			return (int128)num * den2 == (int128)num2 * den;
		}
		bool operator<=(const Node& other) const {
			return !((*this) > other);
		}
		bool operator>=(const Node& other) const {
			return !((*this) < other);
		}
	};
	// 1/1 から p/q へのパスが右に a_1 回、左に a_2 回、… としたときの a を返す
	// (a_1, …, a_k + 1) は p/q の連分数展開になっている
	static std::vector<T> encode_path(T p, T q) {
		std::vector<T> a;
		if (p < q) {
			a.emplace_back(0);
			std::swap(p, q);
		}
		while (p != 1) {
			a.emplace_back(p / q);
			p %= q;
			std::swap(p, q);
		}
		if (!a.empty()) {
			if (a.back() == 1) {
				a.pop_back();
			}
			else {
				a.back()--;
			}
		}
		return a;
	}
	// 1/1 から右に a_1 回、左に a_2 回、… 移動した頂点 (p, q, r, s) を返す
	// (a_1, …, a_k + 1) は (p+r)/(q+s) の連分数展開になっている
	static Node decode_path(const std::vector<T> &a) {
		Node node = Node();
		for (int i = 0; i < (int)a.size(); i++) {
			node = Node::child(node, a[i], i & 1);
		}
		return node;
	}
	// p/q と r/s の LCA (f, g, h, k) を返す
	static Node lca(T p, T q, T r, T s) {
		std::vector<T> a = encode_path(p, q), b = encode_path(r, s);
		int n = std::min((int)a.size(), (int)b.size());
		//T f = 0, g = 1, h = 1, k = 0;
		Node node = Node();
		for (int i = 0; i < n; i++) {
			T c = std::min(a[i], b[i]);
			node = Node::child(node, c, i & 2);
			if (a[i] != b[i]) {
				break;
			}
		}
		return node;
	}
	// p/q の祖先であって深さが d のノード (f, g, h, k) を返す
	// 存在しなければ (0, 0, 0, 0)
	static Node ancestor(T d, T p, T q) {
		std::vector<T> a = encode_path(p, q);
		//T f = 0, g = 1, h = 1, k = 0;
		Node node = Node();
		for (int i = 0; i < (int)a.size(); i++) {
			T c = std::min(d, a[i]);
			node = Node::child(node, c, i & 1);
			d -= c;
			if (d == 0) {
				break;
			}
		}
		return d == 0 ? node : Node(0, 0, 0, 0);
	}
	// p/q の子孫の下限 f/g と上限 h/k を返す
	// p/q に対応する頂点 (f, g, h, k) でもある
	static Node range(T p, T q) {
		return decode_path(encode_path(p, q));
	}
	// judge: [0, ∞] → {true, false}
	// judge は単調性を持つ、すなわち、ある実数 α を境に true と false が切り替わる
	// 分母・分子が max_value 以下の有理数のうち、α に最も近いもの (下側: p/q, 上側: r/s) を返す
	static Node search(const std::function<bool(T, T)> &judge, const T &max_value) {
		//T p = 0, q = 1, r = 1, s = 0;
		Node node = Node();
		const bool judge_01 = judge(0, 1), judge_10 = judge(1, 0);
		while (node.p + node.r <= max_value && node.q + node.s <= max_value) {
			const bool judge_now = judge(node.p + node.r, node.q + node.s);
			const bool isleft = judge_now ^ judge_01;
			T maxd;
			if (isleft) {
				maxd = node.p == 0 ? (max_value - node.s) / node.q : std::min((max_value - node.r) / node.p, (max_value - node.s) / node.q);
			}
			else {
				maxd = node.s == 0 ? (max_value - node.p) / node.r : std::min((max_value - node.q) / node.s, (max_value - node.p) / node.r);
			}
			T dl = 1, dr = 2;
			while (dr <= maxd) {
				auto node2 = Node::child(node, dr, isleft);
				if (judge(node2.p + node2.r, node2.q + node2.s) != judge_now) {
					break;
				}
				dl = dr, dr = std::min(2 * dr, maxd + 1);
			}
			while (dr - dl > 1) {
				T dm = dl + (dr - dl) / 2;
				auto node2 = Node::child(node, dm, isleft);
				if (judge(node2.p + node2.r, node2.q + node2.s) == judge_now) {
					dl = dm;
				}
				else {
					dr = dm;
				}
			}
			node = Node::child(node, dl, isleft);
		}
		return node;
	}

	// (a/b,c/d)の区間内にあるもので最も分母が最小なものを返す
	// https://atcoder.jp/contests/abc408/tasks/abc408_g
	template<class F>
	static T binarySearch(T ok, T ng, F f) {
		while (abs(ok - ng) > 1) {
			T mid = (ok + ng) / 2;
			(f(mid) ? ok : ng) = mid;
		}
		return ok;
	}
	template<class F>
	static T expSearch(F f) {
		long long r = 1;
		while (!f(r)) r <<= 1;
		return binarySearch(r, r >> 1, f);
	}
	static Node abc408g(T a, T b, T c, T d) {
		auto l = Node(a, b, 0, 0);
		auto r = Node(c, d, 0, 0);
		auto node = Node();
		while (true) {
			if (r <= node) {// left
				auto f = [&](const T x)->bool {
					auto node2 = Node::child(node, x, true);
					return node2 < r;
				};
				T d = expSearch(f);
				node = Node::child(node, d, true);
			}
			else if (node <= l) {// right
				auto f = [&](const T x)->bool {
					auto node2 = Node::child(node, x, false);
					return l < node2;
				};
				T d = expSearch(f);
				node = Node::child(node, d, false);
			}
			else {
				break;
			}
		}
		return node;
	}
};
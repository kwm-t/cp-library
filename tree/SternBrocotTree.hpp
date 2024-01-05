class SternBrocotTree {
	// https://miscalc.hatenablog.com/entry/2023/12/22/213007
	// root =(0,1,1,0)
public:
	// (p,q,r,s) から (isleft ? 左 : 右) に d 個進んだ頂点を返す
	static std::tuple<T, T, T, T> child(T p, T q, T r, T s, T d, bool isleft) {
		if (isleft) {
			r += d * p, s += d * q;
		}
		else {
			p += d * r, q += d * s;
		}
		return std::make_tuple(p, q, r, s);
	}
	// (p,q,r,s) の親を返す
	// (p,q,r,s) が根なら (0,0,0,0) を返す
	static std::tuple<T, T, T, T> parent(T p, T q, T r, T s) {
		if (p == 0 && q == 1 && r == 1 && s == 0) {
			return std::make_tuple(0, 0, 0, 0);
		}
		if (p < r || q < s) {
			r -= p, s -= q;
		}
		else {
			p -= r, q -= s;
		}
		return std::make_tuple(p, q, r, s);
	}
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
	static std::tuple<T, T, T, T> decode_path(const std::vector<T> &a) {
		T p = 0, q = 1, r = 1, s = 0;
		for (int i = 0; i < (int)a.size(); i++) {
			std::tie(p, q, r, s) = child(p, q, r, s, a[i], i & 1);
		}
		return std::make_tuple(p, q, r, s);
	}
	// p/q と r/s の LCA (f, g, h, k) を返す
	static std::tuple<T, T, T, T> lca(T p, T q, T r, T s) {
		std::vector<T> a = encode_path(p, q), b = encode_path(r, s);
		int n = std::min((int)a.size(), (int)b.size());
		T f = 0, g = 1, h = 1, k = 0;
		for (int i = 0; i < n; i++) {
			T c = std::min(a[i], b[i]);
			std::tie(f, g, h, k) = child(f, g, h, k, c, i & 1);
			if (a[i] != b[i]) {
				break;
			}
		}
		return std::make_tuple(f, g, h, k);
	}
	// p/q の祖先であって深さが d のノード (f, g, h, k) を返す
	// 存在しなければ (0, 0, 0, 0)
	static std::tuple<T, T, T, T> ancestor(T d, T p, T q) {
		std::vector<T> a = encode_path(p, q);
		T f = 0, g = 1, h = 1, k = 0;
		for (int i = 0; i < (int)a.size(); i++) {
			T c = std::min(d, a[i]);
			std::tie(f, g, h, k) = child(f, g, h, k, c, i & 1);
			d -= c;
			if (d == 0) {
				break;
			}
		}
		return d == 0 ? std::make_tuple(f, g, h, k) : std::make_tuple(0, 0, 0, 0);
	}
	// p/q の子孫の下限 f/g と上限 h/k を返す
	// p/q に対応する頂点 (f, g, h, k) でもある
	static std::tuple<T, T, T, T> range(T p, T q) {
		return decode_path(encode_path(p, q));
	}
	// judge: [0, ∞] → {true, false}
	// judge は単調性を持つ、すなわち、ある実数 α を境に true と false が切り替わる
	// 分母・分子が max_value 以下の有理数のうち、α に最も近いもの (下側: p/q, 上側: r/s) を返す
	static std::tuple<T, T, T, T> search(const std::function<bool(T, T)> &judge, const T &max_value) {
		T p = 0, q = 1, r = 1, s = 0;
		const bool judge_01 = judge(0, 1), judge_10 = judge(1, 0);
		while (p + r <= max_value && q + s <= max_value) {
			const bool judge_now = judge(p + r, q + s);
			const bool isleft = judge_now ^ judge_01;
			T maxd;
			if (isleft) {
				maxd = p == 0 ? (max_value - s) / q : std::min((max_value - r) / p, (max_value - s) / q);
			}
			else {
				maxd = s == 0 ? (max_value - p) / r : std::min((max_value - q) / s, (max_value - p) / r);
			}
			T dl = 1, dr = 2;
			while (dr <= maxd) {
				auto[np, nq, nr, ns] = child(p, q, r, s, dr, isleft);
				if (judge(np + nr, nq + ns) != judge_now) {
					break;
				}
				dl = dr, dr = std::min(2 * dr, maxd + 1);
			}
			while (dr - dl > 1) {
				T dm = dl + (dr - dl) / 2;
				auto[np, nq, nr, ns] = child(p, q, r, s, dm, isleft);
				if (judge(np + nr, nq + ns) == judge_now) {
					dl = dm;
				}
				else {
					dr = dm;
				}
			}
			std::tie(p, q, r, s) = child(p, q, r, s, dl, isleft);
		}
		return std::make_tuple(p, q, r, s);
	}
};
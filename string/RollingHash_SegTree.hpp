namespace RangeSumMint {
	// 区間加算
	mint op(mint a, mint b) { return a + b; }
	mint e() { return 0; }
	using segtree = atcoder::segtree<mint, op, e>;
}
template<typename T>
class Rollinghash {
public:
	Rollinghash(const std::vector<T>&v, int base) :base(base) {
		build(v);
	}
	Rollinghash(const std::string &s, int base = 27) :base(base) {
		std::vector<int>v;
		for (auto c : s)v.push_back(c - 'a' + 1);
		build(v);
	}
	// rは区間内に収まらなくてもいい
	mint get(const long long l, const long long r) {
		if (l >= size) {
			long long nl = l % size;
			long long nr = r + nl - l;
			return get(nl, nr);
		}
		long long width = r - l;
		long long h = std::min(width, (long long)size - l);
		long long b = ((width - h) / size) * size;
		long long t = width - h - b;
		mint hash = 0;
		if (h) {
			hash += seg.prod(l, l + h) / pow[l];
		}
		if (b) {
			hash += seg.prod(0, size) * (atcoder::pow_mod(base.val(), b, mod) - 1) / (pow[size] - 1) * pow[h];
		}
		if (t) {
			hash += atcoder::pow_mod(base.val(), h + b, mod)*seg.prod(0, t);
		}
		return hash;
	}
	void update(int index, T value) {
		seg.set(index, value * pow[index]);
	}
	void update(int index, char c) {
		update(index, c - 'a' + 1);
	}
private:
	int size;
	mint base;
	mint inv;
	RangeSumMint::segtree seg;
	std::vector<mint>pow;

	template<typename U>
	void build(const std::vector<U>&v) {
		inv = base.inv();
		size = v.size();

		pow.resize(size + 1);
		pow[0] = 1;
		for (int i = 0; i < size; ++i)pow[i + 1] = pow[i] * base;

		std::vector<mint>mv(size);
		for (int i = 0; i < size; ++i) {
			mv[i] = pow[i] * v[i];
		}
		seg = RangeSumMint::segtree(mv);
	}
};
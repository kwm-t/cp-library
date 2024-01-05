#include <vector>
mint op(mint a, mint b) { return a + b; }
mint e() { return 0; }
struct Rollinghash {
	Rollinghash(int _base, std::vector<int>v) {
		base = _base + 10;
		inv = base.inv();
		sz = v.size() * 2;
		std::vector<mint>mv(sz);
		mint pw = 1;
		for (int i = (int)v.size() * 2 - 1; i >= 0; --i) {
			mv[i] = (v[i%v.size()] + 1)*pw;
			pw *= base;
		}
		seg = segtree<mint, op, e>(mv);
	}
	mint get(int l, int r) {
		return seg.prod(l, r) * inv.pow(sz - r);
	}
	int sz;
	mint base;
	mint inv;
	segtree<mint, op, e> seg;
};
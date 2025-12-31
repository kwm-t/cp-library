// https://atcoder.jp/contests/fps-24/submissions/70656364
// TOMOO - Super Bal
/*template <class T>
std::vector<T> FPS_inv(std::vector<T> f, int len = -1) {
	if (len == -1) len = f.size();
	assert(f[0] != 0);
	std::vector<T> g = { 1 / f[0] };
	int s = 1;
	while (s < len) {
		// g = 2g_s - f(g_s)^2 (mod x ^ (2 * s))
		// g = g - (fg - 1)g
		// (fg - 1) = 0 (mod x ^ (s))
		std::vector<T> n_g(s * 2, 0);
		std::vector<T> f_s(s * 2, 0);
		g.resize(s * 2);
		for (int i = 0; i < s * 2; i++) {
			if (int(f.size()) > i) f_s[i] = f[i];
			n_g[i] = g[i];
		}
		atcoder::internal::butterfly(g);
		atcoder::internal::butterfly(f_s);
		for (int i = 0; i < s * 2; i++) {
			f_s[i] *= g[i];
		}
		atcoder::internal::butterfly_inv(f_s);
		T iz = 1 / (T)(s * 2);
		for (int i = s; i < s * 2; i++) {
			f_s[i] *= iz;
		}
		for (int i = 0; i < s; i++) {
			f_s[i] = 0;
		}
		atcoder::internal::butterfly(f_s);
		for (int i = 0; i < s * 2; i++) {
			f_s[i] *= g[i];
		}
		atcoder::internal::butterfly_inv(f_s);
		for (int i = s; i < s * 2; i++) {
			n_g[i] -= f_s[i] * iz;
		}
		std::swap(n_g, g);
		s *= 2;
	}
	g.resize(len);
	return g;
}*/
template<class T>
std::vector<T> FPS_Product_Sequence(std::vector<std::vector<T>> f) {
	if (f.empty()) return { 1 };
	auto op = [&](auto self, int l, int r) -> std::vector<T> {
		if (l + 1 == r) return f[l];
		int m = (l + r) / 2;
		return atcoder::convolution(self(self, l, m), self(self, m, r));
	};
	return op(op, 0, f.size());
}
template<class T>
std::vector<T> Multipoint_Evaluation(std::vector<T> f, std::vector<T> p) {
	std::reverse(f.begin(), f.end());
	int M = f.size();
	int N = p.size();
	std::vector<std::vector<T>> g(N);
	for (int i = 0; i < N; i++) {
		g[i] = { 1, p[i] * -1 };
	}
	auto tmp = FPS_Product_Sequence(g);
	tmp = ((fps)tmp).inv(M);
	//tmp = FPS_inv(tmp, M);
	tmp = atcoder::convolution(tmp, f);
	tmp.resize(M);
	int size = 1;
	while (size < N) size *= 2;
	std::vector<std::vector<T>> seg(size * 2, { 1 });
	for (int i = 0; i < N; i++) {
		seg[i + size] = g[i];
	}
	for (int i = size - 1; i > 0; i--) {
		seg[i] = atcoder::convolution(seg[i * 2], seg[i * 2 + 1]);
	}

	std::vector<T> inv = { 1, (T)(1) / (T)(2) };
	// a, b の convolution のうち、l, r の間だけ欲しい
	auto calc = [&](std::vector<T> a, std::vector<T> b, int l, int r) -> std::vector<T> {
		if (l == r) return {};
		if (l < 0) l = 0;
		while ((int)a.size() > r) a.pop_back();
		while ((int)b.size() > r) b.pop_back();
		int lim = (int)a.size() + (int)b.size() - 1;
		lim -= std::min(lim - r, l);
		int z = 0;
		while ((1 << z) < lim) z++;
		a.resize((1 << z), 0);
		b.resize((1 << z), 0);
		atcoder::internal::butterfly(a);
		atcoder::internal::butterfly(b);
		for (int i = 0; i < (1 << z); i++) a[i] *= b[i];
		atcoder::internal::butterfly_inv(a);
		while ((int)inv.size() <= z) {
			inv.push_back(inv.back() * inv[1]);
		}
		std::vector<T> d(r - l);
		for (int i = l; i < r; i++) d[i - l] = a[i] * inv[z];
		return d;
	};

	std::vector<T> res(N);
	auto rec = [&](auto self, int ind, std::vector<T> v) -> void {
		if ((int)seg[ind].size() == 1) {
			return;
		}
		if (size <= ind) {
			res[ind - size] = v.back();
			return;
		}
		self(self, ind * 2, calc(v, seg[ind * 2 + 1], (int)(v.size() - seg[ind * 2].size() + 1), (int)v.size()));
		self(self, ind * 2 + 1, calc(v, seg[ind * 2], (int)(v.size() - seg[ind * 2 + 1].size() + 1), (int)v.size()));
	};
	rec(rec, 1, tmp);
	return res;
}
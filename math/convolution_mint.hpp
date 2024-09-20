std::vector<mint> make_root(long long mod) {
	std::vector<mint> ret(0);
	long long r = pow_mod(3, 119, mod);
	for (long long i = 0; i < 23; i++) {
		ret.push_back(r);
		r = (r * r) % mod;
	}
	reverse(ret.begin(), ret.end());
	return ret;
}

std::vector<mint> make_invroot(std::vector<mint> &root, long long mod) {
	std::vector<mint> ret;
	for (long long i = 0; i < root.size(); i++) {
		ret.push_back(root[i].inv());
	}
	return ret;
}

std::vector<mint> ntt(const std::vector<mint> &a, int depth, const std::vector<mint> &root) {
	int n = (int)a.size();
	if (n == 1) return a;

	std::vector<mint> ret;

	std::vector<mint> even;
	std::vector<mint> odd;
	for (long long i = 0; i < n; ++i) {
		if (0 == i % 2) even.push_back(a[i]);
		else odd.push_back(a[i]);
	}

	auto d_even = ntt(even, depth - 1, root);
	auto d_odd = ntt(odd, depth - 1, root);

	const mint r = root[depth];

	mint now = 1;
	for (long long i = 0; i < n; i++) {
		ret.push_back(d_even[i % (n / 2)] + now * d_odd[i % (n / 2)]);
		now *= r;
	}
	return ret;
}


std::vector<mint> convolution_mint(std::vector<mint> a, std::vector<mint> b) {
	std::vector<mint> root = make_root(mod);
	std::vector<mint> invroot = make_invroot(root, mod);

	int sizeA = (int)a.size();
	int sizeB = (int)b.size();
	int sizeC = sizeA + sizeB - 1;

	int n = 1;
	while (n <= sizeC) n *= 2;

	a.resize(n);
	b.resize(n);

	int log_2n = 1;
	while ((1 << log_2n) < n) log_2n++;

	auto da = ntt(a, log_2n - 1, root);
	auto db = ntt(b, log_2n - 1, root);

	std::vector<mint> dc(n);
	for (int i = 0; i < n; ++i) dc[i] = da[i] * db[i];

	auto c = ntt(dc, log_2n - 1, invroot);

	// INTT の後は最後に n で割ることを忘れずに．
	std::vector<mint> ret(n);
	mint invN = mint::raw(n).inv();
	for (int i = 0; i < n; ++i) ret[i] = c[i] * invN;
	return ret;
}
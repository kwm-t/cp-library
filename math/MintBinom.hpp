// combination mod prime
// https://www.youtube.com/watch?v=8uowVvQ_-Mo&feature=youtu.be&t=1619
struct combination {
	std::vector<mint> fact, ifact;
	combination(int n) :fact(n + 1), ifact(n + 1) {
		assert(n < mod);
		fact[0] = 1;
		for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i;
		ifact[n] = fact[n].inv();
		for (int i = n; i >= 1; --i) ifact[i - 1] = ifact[i] * i;
	}
	mint operator()(int n, int k) { return com(n, k); }
	mint com(int n, int k) {
		//負の二項係数を考慮する場合にコメントアウトを外す
		//if (n < 0) return com(-n, k) * (k % 2 ? -1 : 1);
		if (k < 0 || k > n) return 0;
		return fact[n] * ifact[k] * ifact[n - k];
	}
	mint comsub(long long n, long long k) {
		if (n - k < k) k = n - k;
		assert(k < (int)fact.size());
		mint val = ifact[k];
		for (int i = 0; i < k; ++i) val *= n - i;
		return val;
	}
	template <typename ...Ms, std::enable_if_t<std::conjunction_v<std::is_integral<Ms>...>, std::nullptr_t> = nullptr>
	mint polynom(const int n, const Ms & ...ms) {
		mint res = fact[n];
		int sum = 0;
		for (int m : { ms... }) {
			if (m < 0 or m > n) return 0;
			res *= ifact[m];
			sum += m;
		}
		if (sum > n)return 0;
		res *= ifact[n - sum];
		return res;
	}
	mint div(int x) {
		if (x >= (int)fact.size())return mint(x).inv();
		return fact[x - 1] * ifact[x];
	}
	mint inv(int n, int k) {
		//if (n < 0) return inv(-n, k) * (k % 2 ? -1 : 1);
		if (k < 0 || k > n) return 0;
		return ifact[n] * fact[k] * fact[n - k];
	}
	mint p(int n, int k) { return fact[n] * ifact[n - k]; }
}c(2000006);
// https://qiita.com/sounansya/items/51b39e0d7bf5cc194081
// https://atcoder.jp/contests/abc402/submissions/65538621
template <typename mint>
class GeneralFloorSum {
public:
	GeneralFloorSum(int pqsum) :pqsum(pqsum) { init(); }

	std::vector<std::vector<mint>>solve(long long n, long long m, long long a, long long b) {
		long long a0 = a / m;
		long long b0 = b / m;
		long long a1 = a % m;
		long long b1 = b % m;
		std::vector<std::vector<mint>> sdp(pqsum + 1);
		if (a1 == 0) {
			for (int p = 0; p <= pqsum; ++p) {
				sdp[p].resize(pqsum - p + 1);
				sdp[p][0] = s(p, n);
			}
		}
		else {
			// f_{p}{q}(n,m,a,b)
			// = k^q*S_p(d_{k+1}) -Σ(q,i)ΣS_p(j)f_{i,j}(k,a,m,m-b+a-1)
			long long k = (a1 * (n - 1) + b1) / m;
			auto f = solve(k, a1, m, m - b1 + a1 - 1);
			for (int p = 0; p <= pqsum; ++p) {
				sdp[p].resize(pqsum - p + 1);
				sdp[p][0] = s(p, n);// q = 0;
				for (int q = 1; p + q <= pqsum; ++q) {
					sdp[p][q] = mint(k).pow(q) * s(p, n);
					for (int i = 0; i < q; ++i) {
						mint tmp = 0;
						for (int j = 0; j <= p + 1; ++j) {
							tmp += faul[p][j] * f[i][j];
						}
						sdp[p][q] -= tmp * fc.binom(q, i);
					}
				}
			}
		}
		// a0,b0の処理
		std::vector<mint> apow(pqsum + 1, 1);
		std::vector<mint> bpow(pqsum + 1, 1);
		for (int i = 1; i <= pqsum; i++) {
			apow[i] = apow[i - 1] * a0;
			bpow[i] = bpow[i - 1] * b0;
		}
		std::vector<std::vector<mint>> dp(pqsum + 1);
		for (int p = 0; p <= pqsum; p++) {
			dp[p].resize(pqsum - p + 1);
			dp[p][0] = sdp[p][0];// q = 0
			for (int q = 1; p + q <= pqsum; ++q) {
				// (a1*i+b1+floor())^qの展開
				for (int x = 0; x <= q; ++x) for (int y = 0; x + y <= q; ++y) {
					mint tmp = 1;
					tmp *= fc.binom(q, x) * apow[x];
					tmp *= fc.binom(q - x, y) * bpow[y];
					tmp *= sdp[p + x][q - x - y];
					dp[p][q] += tmp;
				}
			}
		}
		return dp;
	}
private:
	int pqsum;
	std::vector<std::vector<mint>>faul;
	Factorial<mint> fc;

	void init() {
		fc = Factorial<mint>(pqsum + 10);
		faul.resize(pqsum + 1);
		for (int i = 0; i <= pqsum; ++i)faul[i] = faulhaber(i, i == 0 ? 0 : -1, fc);
	}

	mint s(int k, long long n) {
		// S_k(n):=0^k+1^k+.......(n-1)^k
		if (n == 0)return 0;
		if (k == 0)return n;
		mint ret = 0;
		mint powN = 1;
		for (auto e : faul[k]) {
			ret += e * powN;
			powN *= n;
		}
		return ret;
	}
};
// intで収まらない時はmodを2つとって復元してください
// https://atcoder.jp/contests/abc402/submissions/73397856
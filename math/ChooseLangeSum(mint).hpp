#include <vector>
// 二項係数の区間和
struct ChooseLangeSum {
public:
	//pは素数
	ChooseLangeSum(long long n, long long p) :mod(p), n(n) { Init(); }
private:
	//下準備
	void Init() {
		lc = Lucas(mod);
		Create();
	}
	void Create() {
		d = 1;
		while (d * d <= n)d *= mod;
		v0.resize(d), v1.resize(d);
		v0sum.resize(d + 1), v1sum.resize(d + 1);
		for (int i = 0; i < d; ++i) {
			v0[i] = lc.c(n / d, i);
			v1[i] = lc.c(n % d, i);
			v0sum[i + 1] = v0sum[i] + v0[i];
			v1sum[i + 1] = v1sum[i] + v1[i];
		}
	}
public:
	mint f(int r) {
		// choose(n,0)+...+choose(n,r-1)
		mint ret = v0sum[r / d] * v1sum[d] + v0[r / d] * v1sum[r%d];
		return ret;
	}
	mint f(int l, int r) {
		// choose(n,l)+...+choose(n,r-1)
		return f(r) - f(l);
	}
private:
	long long n;
	long long mod;
	long long d;//√nを超える最大のp冪
	std::vector<mint>v0, v0sum;//Σc(n/d,k),0<=k<d
	std::vector<mint>v1, v1sum;//Σc(n%d,k),0<=k<d
	Lucas lc;
};
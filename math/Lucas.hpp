#include <vector>
#include <iostream> 
struct Lucas {
public:
	//pは素数
	Lucas(long long p) :mod(p) { Init(); }
private:
	//下準備
	void Init() {
		CreateFact();
		CreateComb();
	}
	//comb計算
	void CreateComb() {
		comb.resize(mod);
		for (int i = 0; i < mod; ++i) {
			comb[i].resize(mod);
			for (int j = 0; j < mod; ++j) {
				if (i < j) {
					comb[i][j] = 0;
					continue;
				}
				comb[i][j] = (((fact[i] * ifact[j]) % mod) * ifact[i - j]) % mod;
			}
		}
	}
	//階乗計算
	void CreateFact() {
		fact.resize(mod);
		ifact.reserve(mod);
		fact[0] = 1;
		for (int i = 1; i < mod; ++i) fact[i] = (fact[i - 1] * i) % mod;
		ifact[mod - 1] = inv(fact[mod - 1]);
		for (int i = mod - 1; i >= 0; --i) ifact[i - 1] = (ifact[i] * i) % mod;
	}
	//逆元計算(x^(y - 2))
	long long inv(long long x) {
		long long y = mod - 2;
		long long tmp = x;
		long long ret = 1;
		while (y > 0) {
			if (1 == y % 2) {
				ret *= tmp;
				ret %= mod;
			}
			y /= 2;
			tmp *= tmp;
			tmp %= mod;
		}
		return ret;
	}
public:
	//c(i,j)の計算
	int c(long long i, long long j) {
		if (i < j) return 0;
		long long ret = 1;
		while (i > 0) {
			long long x = i % mod;
			long long y = j % mod;
			ret *= comb[x][y];
			ret %= mod;
			i /= mod;
			j /= mod;
		}
		return ret;
	}
	//階乗デバッグ
	void debug() {
		for (int i = 0; i < mod; ++i) std::cout << fact[i] << " ";
		std::cout << std::endl;
		for (int i = 0; i < mod; ++i) std::cout << ifact[i] << " ";
		std::cout << std::endl;
	}
	//combデバッグ
	void debug2() {
		for (int i = 0; i < mod; ++i) {
			for (int j = 0; j < mod; ++j) std::cout << comb[i][j] << " ";
			std::cout << std::endl;
		}
	}
	//パスカルの三角形デバッグ
	void debug3() {
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < i + 1; ++j) std::cout << c(i, j) << " ";
			std::cout << std::endl;
		}
	}
private:
	long long mod;
	std::vector<std::vector<long long>> comb;
	std::vector<long long>fact;
	std::vector<long long>ifact;
};

#include <vector>
#include <random>
#include <time.h> 
#include <map>
#include <numeric>
long long cheker(long long n) {
	if (n < 0)return -1;
	for (long long i = sqrt(n) - 10; i <= sqrt(n) + 10; ++i) {
		if (i * i == n)return i;
	}
	return -1;
}
long long sq(long long n) {
	long long ng = -1, ok = 3 * 1e9;
	while (abs(ng - ok) > 1) {
		long long mid = (ok + ng) / 2;
		if (mid * mid >= n)ok = mid;
		else ng = mid;
	}
	if (ok * ok == n)return ok;
	else return -1;
}

struct Miller {
	const std::vector<long long> v = { 2 , 7 , 61 }; // < 4,759,123,141
	// x^k (mod m)
	long long modpow(long long x, long long k, long long m) {
		long long res = 1;
		while (k) {
			if (k & 1) {
				res = res * x % m;
			}
			k /= 2;
			x = x * x % m;
		}
		return res;
	}
	// check if n is prime
	bool check(long long n) {
		if (n < 2) {
			return false;
		}
		long long d = n - 1;
		long long s = 0;
		while (d % 2 == 0) {
			d /= 2;
			s++;
		}
		for (long long a : v) {
			if (a == n) {
				return true;
			}
			if (modpow(a, d, n) != 1) {
				bool ok = true;
				for (long long r = 0; r < s; r++) {
					if (modpow(a, d * (1LL << r), n) == n - 1) {
						ok = false;
						break;
					}
				}
				if (ok) {
					return false;
				}
			}
		}
		return true;
	}
};

struct Rho {
	std::mt19937 mt;
	Miller miller;
	long long c;
	Rho() {
		mt.seed(clock());
	}
	inline long long f(long long x, long long n) {
		return (x * x + c) % n;
	}
	long long check(long long n) {
		if (n == 4) {
			return 2;
		}
		c = mt() % n;
		long long x = mt() % n;
		long long y = x;
		long long d = 1;
		while (d == 1) {
			x = f(x, n);
			y = f(f(y, n), n);
			d = std::gcd(abs(x - y), n);
		}
		if (d == n) {
			return -1;
		}
		return d;
	}
	std::vector<long long> factor(long long n) {
		if (n <= 1) {
			return {};
		}
		if (miller.check(n)) {
			return { n };
		}
		long long res = -1;
		while (res == -1) {
			res = check(n);
		}
		std::vector<long long> fa = factor(res);
		std::vector<long long> fb = factor(n / res);
		fa.insert(fa.end(), fb.begin(), fb.end());
		return fa;
	}
};

Rho rho;

void dfs(const std::map<long long, long long>& f, long long num, const std::map<long long, long long>::iterator &ite, std::vector<long long> &d) {
	if (ite == f.end()) {
		d.push_back(num);
		return;
	}
	for(int i = 0;i <ite->second + 1;++i) {
		dfs(f, num, next(ite), d);
		num *= ite->first;
	}
}

std::vector<long long> divisor(long long n) {
	std::map<long long, long long> mp;
	for (const auto& aa : rho.factor(n))++mp[aa];
	std::vector<long long>ret;
	dfs(mp, 1, mp.begin(), ret);
	return ret;
}
//std::vector<long long>vx = divisor(r);
#include <unordered_map>
struct BabyStepGiantStep {
private:
	long long g, p;
	long long sqrtp,inv;
	std::unordered_map<long long, int> mp;
	void init(long long g) {
		long long cur = 0;
		for (int i = 0; i < sqrtp; i++) {
			if (!mp.count(cur))mp[cur] = i;
			(cur += g) %= p;
		}
		// inv(g^sqrtp)
		inv = mod(-g * sqrtp);
		return;
	}
	long long mod(long long x) {
		if (x > 0)return x % p;
		else return (p + x % p) % p;
	}

public:
	// g^x(mod p)
	BabyStepGiantStep(long long _g, long long _p) :g(_g), p(_p) {
		g %= p;
		long long ok = p, ng = 0;
		while (ok - ng > 1) {
			long long mid = (ok + ng) / 2;
			if (mid * mid >= p) ok = mid;
			else ng = mid;
		}
		sqrtp = ok;
		init(g);
	}

	// g^x = y mod p → return x 
	// not found -1
	long long solve(long long y) {
		y = mod(y);
		long long cur = y;
		for (int i = 0; i < sqrtp; i++) {
			if (mp.count(cur)) {
				int j = mp[cur];
				return sqrtp * i + j;
			}
			else cur = (cur + inv) % p;
		}
		return -1;
	}
};
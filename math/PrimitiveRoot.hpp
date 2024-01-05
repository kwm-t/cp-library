#include <atcoder/all>
// 原始根を求める
int primitive_root(int p) {
	if (p == 2)  return 1;
	if (p == 167772161)  return 3;
	if (p == 469762049)  return 3;
	if (p == 754974721)  return 11;
	if (p == 998244353)  return 3;
	if (p == 1000000007)  return 5;

	// p-1 の素因数分解
	int divs[20] = {};
	divs[0] = 2;
	int cnt = 1;
	long long x = (p - 1) / 2;
	while (x % 2 == 0)   x /= 2;
	for (long long i = 3; i * i <= x; i += 2) {
		if (x % i == 0) {
			divs[cnt++] = i;
			while (x % i == 0)   x /= i;
		}
	}
	if (x > 1)   divs[cnt++] = x;

	// 原始根であるかの判定のために root^((p-1)/d) != 1 (mod p) を確かめる
	for (int root = 2;; ++root) {
		bool ok = true;
		for (int i = 0; i < cnt; ++i) {
			if (atcoder::pow_mod(root, (p - 1) / divs[i], p) == 1) {
				ok = false;
				break;
			}
		}
		if (ok)  return root;
	}
}
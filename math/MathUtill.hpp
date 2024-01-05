#include <vector>
#include <algorithm>
//約数の個数
long long divisorEnumeration(long long n) {
	std::vector<long long> x;
	for (long long i = 2; i*i <= n; i++) {
		int c = 0;
		while (n%i == 0) { // 素数で割り切れなくなるまで割っていく
			c++;//割った個数を配列に足す
			n /= i;
		}
		if (0 != c) x.push_back(c);
		if (1 == n) break;
	}
	if (1 != n) x.push_back(1);

	long long nAns = 1;
	for (int i = 0; i < x.size(); ++i)nAns *= x[i] + 1;
	return nAns;
}

//約数列挙
std::vector<long long> divisorEnumeration(long long n) {
	std::vector<long long> x;
	for (long long i = 1; i*i <= n; i++) {
		if (0 == n % i) {
			x.push_back(i);
			if (n != i * i) x.push_back(n / i);
		}
	}
	std::sort(x.begin(),x.end());
	return x;
}
// 素因数分解
std::vector<std::pair<long long, int>> primeFactorization(long long n) {
	std::vector<std::pair<long long, int>> ret;
	for (long long i = 2; i*i <= n; i++) {
		int c = 0;
		while (n%i == 0) { // 素数で割り切れなくなるまで割っていく
			c++;//割った個数を配列に足す
			n /= i;
		}
		if (0 != c) ret.push_back({ i,c });
		if (1 == n) break;
	}
	if (1 != n) ret.push_back({ n,1 });
	return ret;
}

//オイラーのφ関数(n以下の互いに素な物の数)
long long phi(long long n) {
	long long ret = n;
	for (long long i = 2; i*i <= n; i++) {
		int c = 0;
		while (n%i == 0) {
			c++;
			n /= i;
		}
		if (0 != c) ret -= ret / i;
		if (1 == n) break;
	}
	if (1 != n) ret -= ret / n;
	return ret;
}
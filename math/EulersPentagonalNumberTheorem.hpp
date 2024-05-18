// オイラーの五角数定理
// f(x^k) := Π(1-x^(i*k))
// ⇔Σ(i=-inf,inf)(-1)^i * x^ k *(i * (3 * i - 1)/2)
// の先頭n+1項目までを返す
// [x^n]Π(i=1,n)(1-x^(i*k))を求めたいときに使える
// ※総積記号が掃く範囲を無限に拡張してもnときまでの係数に影響がないため
std::vector<mint> eulersPentagonalNumberTheorem(int n, int k = 1) {
	std::vector<mint>result(n + 1);
	for (int i = 0; i <= n; ++i) {
		long long j = ((long long)i) * (3 * i - 1) / 2;
		j *= k;
		if (j > n)continue;
		result[j] += 0 == i % 2 ? 1 : -1;
		if (0 == i)continue;
		j = ((long long)-i) * (3 * -i - 1) / 2;
		j *= k;
		if (j > n || j < 0)continue;
		result[j] += 0 == i % 2 ? 1 : -1;
	}
	return result;
}
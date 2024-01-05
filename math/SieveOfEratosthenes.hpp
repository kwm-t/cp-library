std::vector<long long> sieveOfEratosthenes(long long  n = 1000000) {
	std::vector<bool>p(n + 1, true);
	std::vector<long long>ret;
	p[0] = false;
	p[1] = false;
	for (int i = 2; i <= n; ++i) {
		if (!p[i]) continue;
		ret.push_back(i);
		for (int j = 2 * i; j <= n; j += i) p[j] = false;
	}
	return ret;
}
//long long にしているのは p*pとかでこまらないように
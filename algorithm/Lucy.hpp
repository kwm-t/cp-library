// n以下の素数の個数を返す O(N^(3/4)/logN)
long long lucyDp_count(const long long N) {
	const long long sqrtN = std::sqrt(N);
	std::vector<long long>Q;
	for (int i = 1; i <= sqrtN; ++i) Q.push_back(N / i);
	while (1 != Q.back()) Q.push_back(Q.back() - 1);

	std::unordered_map<long long, long long>mp;
	for (auto q : Q)mp[q] = q - 1;
	for (long long p = 2; p <= sqrtN; ++p) {
		// not prime
		if (mp[p] == mp[p - 1])continue;
		auto pre = mp[p - 1];
		for (auto q : Q) {
			if (q < p * p)break;
			mp[q] -= mp[q / p] - pre;
		}
	}
	return mp[N];
}
// n以下の素数の和を返す O(N^(3/4)/logN)
long long lucyDp_sum(const long long N) {
	const long long sqrtN = std::sqrt(N);
	std::vector<long long>Q;
	for (int i = 1; i <= sqrtN; ++i) Q.push_back(N / i);
	while (1 != Q.back()) Q.push_back(Q.back() - 1);

	std::unordered_map<long long, long long>mp;
	for (auto q : Q)mp[q] = q * (q + 1) / 2 - 1;
	for (long long p = 2; p <= sqrtN; ++p) {
		// not prime
		if (mp[p] == mp[p - 1])continue;
		auto pre = mp[p - 1];
		for (auto q : Q) {
			if (q < p * p)break;
			mp[q] -= p * (mp[q / p] - pre);
		}
	}
	return mp[N];
}
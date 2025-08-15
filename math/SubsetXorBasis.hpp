bool subset_xor_basis(const std::vector<long long>& a, long long b, std::vector<int>& used) {
	int n = (int)a.size();
	std::vector<long long> basis;
	std::vector<uint64_t> mask;
	std::vector<int> origin_idx;

	for (int i = 0; i < n; i++) {
		long long x = a[i];
		uint64_t m = 0;
		for (int j = 0; j < (int)basis.size(); j++) {
			if ((x ^ basis[j]) < x) {
				x ^= basis[j];
				m ^= mask[j];
			}
		}
		if (x != 0) {
			m |= (1ULL << basis.size());
			basis.push_back(x);
			mask.push_back(m);
			origin_idx.push_back(i);
		}
	}

	uint64_t comb = 0;
	for (int j = 0; j < (int)basis.size(); j++) {
		if ((b ^ basis[j]) < b) {
			b ^= basis[j];
			comb ^= mask[j];
		}
	}
	if (b != 0) return false;

	used.assign(n, 0);
	for (int j = 0; j < (int)basis.size(); j++) {
		if (comb & (1ULL << j)) {
			used[origin_idx[j]] = 1;
		}
	}
	return true;
}
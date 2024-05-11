class Osa_k {
public:
	Osa_k(int max = 1000006) {
		size = max + 1;
		osa_k.resize(size);
	}
	void init() {
		osa_k[1] = 1;
		for (int i = 2; i < size + 1; ++i) {
			if (0 != osa_k[i])continue;
			osa_k[i] = i;
			for (int j = i * 2; j <= size; j += i) {
				if (0 == osa_k[j]) osa_k[j] = i;
			}
		}
	}
	std::vector<int> osk(int n) {
		std::vector<int>ret;
		while (1 != n) {
			ret.push_back(osa_k[n]);
			n /= osa_k[n];
		}
		return ret;
	}
	std::vector<std::pair<int, int>>oskPair(int n) {
		auto ps = osk(n);
		std::vector<std::pair<int, int>> result;
		for (int i = 0; i < ps.size();) {
			int j = i + 1;
			while (j < ps.size() && ps[i] == ps[j])j++;
			result.emplace_back(ps[i], j - i);
			i = j;
		}
		return result;
	}
private:
	int size;
	std::vector<int>osa_k;
};
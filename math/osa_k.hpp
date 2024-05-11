class Osa_k {
public:
	Osa_k(int max = 1000006) {
		osa_k.resize(max + 1);
	}
	void init() {
		osa_k[1] = 1;
		for (int i = 2; i < osa_k.size(); ++i) {
			if (0 != osa_k[i])continue;
			osa_k[i] = i;
			for (int j = i * 2; j < osa_k.size(); j += i) {
				if (0 == osa_k[j]) osa_k[j] = i;
			}
		}
	}
	std::vector<int> osk(int n) {
		std::vector<int>result;
		while (1 != n) {
			result.push_back(osa_k[n]);
			n /= osa_k[n];
		}
		return result;
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
	std::vector<int>osa_k;
};
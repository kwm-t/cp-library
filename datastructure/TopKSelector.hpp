template <class T, class Cmp>
struct TopKSmall {
	int K;
	std::vector<T> v;
	Cmp cmp;

	TopKSmall(int K, T init, Cmp cmp) : K(K), v(K, init), cmp(cmp) {}

	bool update(const T& x) {
		for (int i = 0; i < K; i++) {
			if (cmp(x, v[i])) {
				for (int j = K - 1; j > i; j--) v[j] = v[j - 1];
				v[i] = x;
				return true;
			}
		}
		return false;
	}
};

template <class T, class Cmp>
struct TopKHeap {
	int K;
	Cmp better;
	std::priority_queue<T, std::vector<T>, Cmp> pq;

	TopKHeap(int K, Cmp better) : K(K), better(better), pq(better) {}

	bool update(const T& x) {
		if ((int)pq.size() < K) {
			pq.push(x);
			return true;
		}
		if (better(x, pq.top())) {
			pq.pop();
			pq.push(x);
			return true;
		}
		return false;
	}
	std::vector<T> dump_sorted() {
		auto tmp = pq;
		std::vector<T> res;
		while (!tmp.empty()) {
			res.push_back(tmp.top());
			tmp.pop();
		}
		std::sort(res.begin(), res.end(), std::greater<T>());
		return res;
	}
};
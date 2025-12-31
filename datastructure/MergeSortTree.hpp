template<typename T>
class MergeSortTree {
public:
	MergeSortTree(const std::vector<T> &v, const T &INF) {
		init(v, INF);
	}
	T query(const int &l, const int &r, const T &threshold) {
		return queryHelper(0, size, 0, l, r, threshold);
	}
private:
	int log = 1;
	int size = 2;
	std::vector<std::vector<T>>node;
	std::vector<std::vector<T>>sumNode;
	void init(std::vector<long long>v, const T &INF) {
		while (size < v.size()) {
			log++;
			size *= 2;
		}
		while (size != v.size()) {
			v.push_back(INF);
		}
		node.resize(size * 2);
		for (int i = 0; i < size; ++i) {
			node[i + size - 1].push_back(v[i]);
		}
		for (int i = size - 2; i >= 0; --i) {
			node[i] = marge(node[i * 2 + 1], node[i * 2 + 2]);
		}
		sumNode.resize(node.size());
		for (int i = 0; i < node.size(); ++i) {
			sumNode[i].resize(node[i].size() + 1);
			for (int j = 0; j < node[i].size(); ++j) {
				sumNode[i][j + 1] = sumNode[i][j] + node[i][j];
			}
		}
	}
	std::vector<T> marge(const std::vector<T>&l, const std::vector<T>&r) {
		std::vector<T>res;
		res.reserve(l.size() + r.size());
		int li = 0, ri = 0;
		while (li < l.size() || ri < r.size()) {
			if (li == l.size()) {
				res.push_back(r[ri]);
				ri++;
			}
			else if (ri == r.size()) {
				res.push_back(l[li]);
				li++;
			}
			else if (l[li] < r[ri]) {
				res.push_back(l[li]);
				li++;
			}
			else {
				res.push_back(r[ri]);
				ri++;
			}
		}
		return res;
	}
	T queryHelper(const int &l, const int &r, const int &index, const int &targetL, const int &targetR, const T &threshold) {
		if (l == r) return 0;
		if (targetR <= l)return 0;
		if (r <= targetL)return 0;
		if (targetL <= l && r <= targetR) {
			int ok = -1;
			int ng = node[index].size();
			while (1 < abs(ok - ng)) {
				int mid = (ng + ok) / 2;
				auto check = [&]() {return node[index][mid] < threshold; };
				if (check()) ok = mid;
				else ng = mid;
			}
			return sumNode[index][ok + 1] - sumNode[index][0];
		}
		long long res = 0;
		res += queryHelper(l, (l + r) / 2, 2 * index + 1, targetL, targetR, threshold);
		res += queryHelper((l + r) / 2, r, 2 * index + 2, targetL, targetR, threshold);
		return res;
	}
};
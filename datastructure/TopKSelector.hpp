template<typename T>
class TopKSelector {
public:
	TopKSelector(int k) :k(k) {}
	void push(T value) {
		if (pq.size() < k) {
			pq.push(value);
		}
		else if (value < pq.top()) {
			pq.pop();
			pq.push(value);
		}
	}
	std::vector<T> get() {
		std::vector<T>ret;
		while (!pq.empty()) {
			ret.push_back(pq.top());
			pq.pop();
		}
		std::reverse(ret.begin(), ret.end());
		return ret;
	}
private:
	int k;
	std::priority_queue<T>pq;
};
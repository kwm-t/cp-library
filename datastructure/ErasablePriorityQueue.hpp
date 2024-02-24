template<typename T>
class ErasablePriorityQueue {
public:
	ErasablePriorityQueue() {}
	void add(const T &val) {
		pq.push(val);
	}
	void erase(const T &val) {
		dpq.push(val);
	}
	T top(const T &empty) {
		while (!dpq.empty() && pq.top() == dpq.top()) {
			pq.pop();
			dpq.pop();
		}
		if (pq.empty())return empty;
		return pq.top();
	}
	size_t size() {
		return pq.size() - dpq.size();
	}
	bool empty() {
		return 0 == (int)this->size();
	}
private:
	std::priority_queue<T>pq, dpq;
};
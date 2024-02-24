template<typename T>
class ErasablePriorityQueue {
public:
	ErasablePriorityQueue() {}
	void add(T val) {
		pq.first.push(val);
	}
	void erase(T val) {
		pq.second.push(val);
	}
	T top(T empty) {
		while (!pq.second.empty() && pq.first.top() == pq.second.top()) {
			pq.first.pop();
			pq.second.pop();
		}
		if (pq.first.empty())return empty;
		return pq.first.top();
	}
private:
	std::pair<std::priority_queue<T>, std::priority_queue<T>>pq;
};
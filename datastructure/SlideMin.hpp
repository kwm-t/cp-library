#include <queue>
struct SlideMin {
	std::deque<std::pair<int,int>> q;
	SlideMin() {}
	void push(int i, int x) {//index,value
		while ((!q.empty()) && q.back().second >= x) {
			q.pop_back();
		}
		q.emplace_back(i, x);
	}
	int get(int l) {
		while ((!q.empty()) && (q.front().first < l)) {
			q.pop_front();
		}
		return q.front().second;
	}
};
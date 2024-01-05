#include <algorithm>
#include <stack>
#include <vector>
long long maximum_rectangle(std::vector<int>&a) {
	// ヒストグラム最大長方形
	std::stack<int> st;
	std::vector<int>l(a.size()), r(a.size());
	for (int i = 0; i < a.size(); ++i) {
		while (!st.empty() && a[st.top()] >= a[i]) st.pop();
		l[i] = st.empty() ? 0 : (st.top() + 1);
		st.push(i);
	}
	while (!st.empty()) st.pop();
	for (int i = (int)a.size() - 1; i >= 0; --i) {
		while (!st.empty() && a[st.top()] >= a[i]) st.pop();
		r[i] = st.empty() ? a.size() : st.top();
		st.push(i);
	}
	long long res = 0;
	for (int i = 0; i < a.size(); ++i) res = std::max(res, (long long)a[i] * (r[i] - l[i]));
	return res;
}
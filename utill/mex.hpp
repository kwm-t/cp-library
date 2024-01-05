#include<utility>
const int INF = 1e9;
// {count.index}
// seg.all_prod();
std::pair<int,int> op(std::pair<int, int> a, std::pair<int, int> b) {
	if (a.first < b.first)return a;
	else if (a.first > b.first)return b;
	else if (a.second < b.second) return a;
	else return b;
	return a;
}
std::pair<int, int> e() { return { INF,INF }; }
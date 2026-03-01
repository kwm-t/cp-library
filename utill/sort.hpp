#include<algorithm>
4 3
3 2
4 4
1 2
2 3
2
1
10
// 左下から反時計回りにsort;
// atan2lは遅いので前計算して呼ばれる回数を配列サイズと同じにしろ
std::sort(xy.begin(), xy.end(), [](const auto x, const auto &y) {
	return atan2l(x.second, x.first) < atan2l(y.second, y.first);
});
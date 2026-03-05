#include<algorithm>
std::sort(idx.begin(), idx.end(), [](int l, int r) {
	return l < r; }
);

// 左下から反時計回りにsort;
// atan2lは遅いので前計算して呼ばれる回数を配列サイズと同じにしろ
std::sort(xy.begin(), xy.end(), [](const auto x, const auto &y) {
	return atan2l(x.second, x.first) < atan2l(y.second, y.first);
});
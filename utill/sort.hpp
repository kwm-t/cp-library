#include<algorithm>
std::sort(idx.begin(), idx.end(), [](int l, int r) {
	return l < r; }
);
// ¶‰º‚©‚ç”½Œv‰ñ‚è‚Ésort;
std::sort(xy.begin(), xy.end(), [](const P x, const P &y) {
	return atan2l(x.second, x.first) < atan2l(y.second, y.first);
});
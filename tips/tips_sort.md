# 競プロ Tips 集

# ラムダ式を用いたソート
```cpp
std::sort(v.begin(), v.end(), [](int l, int r) {
	return l < r; }
);
```

# 左下から反時計回りにsort
atan2lは遅いので前計算するのを推奨
```cpp
// 左下から反時計回りにsort;
std::sort(xy.begin(), xy.end(), [](const auto x, const auto &y) {
	return atan2l(x.second, x.first) < atan2l(y.second, y.first);
});
```

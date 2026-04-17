# 🧠 競プロ Tips 集

## 🔹 出力が遅いとき
```cpp
#define endl "\n"
```

## 🔹 interractive
```cpp
std::fflush(stdout);
```
## 🔹 浮動小数出力

```cpp
// double
printf("%.16f\n", ans);
// long double
printf("%.16Lf\n", ans);

// iostream
cout << fixed << setprecision(16) << ans << endl;
```

---

## 🔹 部分集合DP（部分集合列挙）

### 再帰なし（下から）
```cpp
for (int i = 0; i < (1 << n); i++) {
    for (int j = (i - 1) & i; j > 0; j = (j - 1) & i) {
        chmin(dp[i], dp[j] + dp[i ^ j]);
    }
}
```

---

### 代表元固定パターン
```cpp
int bit = i & -i;
for (int j = (i - 1) & i; j > 0; j = (j - 1) & i) {
    if (j & bit) {
        dp[i] += dp[i ^ j] * h[j] * f[__builtin_popcount(j)];
    }
}
```

---

## 🔹 文字列操作

```cpp
transform(S.begin(), S.end(), S.begin(),
          [](char c) { return tolower(c); });
```

---

## 🔹 順列全列挙

```cpp
do {
    // 処理
} while (next_permutation(v.begin(), v.end()));
```

---

## 🔹 区間共通部分の長さ

```cpp
// [l0, r0] と [l1, r1]
auto len = clamp(r1, l0, r0) - clamp(l1, l0, r0);
```

---

## 🔹 グリッド移動

```cpp
const int dx4[4] = {1, 0, -1, 0};
const int dy4[4] = {0, -1, 0, 1};
const char dc4[4] = { 'D', 'L', 'U', 'R' };

const int dx8[8] = {1,0,-1,0,1,1,-1,-1};
const int dy8[8] = {0,-1,0,1,1,-1,1,-1};
```

##  🔹 dfs
```cpp
auto dfs = [&](auto &&self, int v, int p = -1)->void {
	for (auto nv : to[v]) {
		if (nv == p)continue;
		self(self, nv, v);
	}
}
dfs(dfs, 0);
```

---

## 🔹 int128

```cpp
#ifdef _MSC_VER
using int128 = long long;
#else
using int128 = __int128;
#endif
```

---

## 🔹 浮動小数比較

```cpp
const double EPS = 1e-10;

// 等値
abs(a - b) < EPS

// 大小比較
a - EPS <= b
a <= b + EPS
a + EPS < b
a < b - EPS
```

---

## 🔹 よくある注意

- 部分集合列挙は `j=0` を含まないことに注意
- `next_permutation` は昇順から始める
- 浮動小数は必ず EPS を使う

# 🔎 lower_bound / upper_bound Tips

## 🔹 前提
```cpp
vector<int> v = {1, 4, 4, 7, 7, 8, 8, 11, 13, 19};
int target;
```

---

## 🔹 個数系

```cpp
// 特定の値の個数
int cnt = upper_bound(v.begin(), v.end(), target)
        - lower_bound(v.begin(), v.end(), target);

// 以下 (<= target)
int cnt = upper_bound(v.begin(), v.end(), target) - v.begin();

// 以上 (>= target)
int cnt = v.end() - lower_bound(v.begin(), v.end(), target);

// より小さい (< target)
int cnt = lower_bound(v.begin(), v.end(), target) - v.begin();

// より大きい (> target)
int cnt = v.end() - upper_bound(v.begin(), v.end(), target);

// 区間 [l, r)
int cnt = lower_bound(v.begin(), v.end(), r)
        - lower_bound(v.begin(), v.end(), l);
```

---

## 🔹 set での前後取得

```cpp
set<int> st;

// 必ず番兵（sentinel）を入れること
auto before = [&](long long x) -> long long {
    return *prev(st.lower_bound(x));
};

auto after = [&](long long x) -> long long {
    return *st.upper_bound(x);
};
```

---

## 🔹 汎用ラッパ

```cpp
// false: <= x, true: < x
auto left = [&](const auto& s, auto x, bool strict = false) {
    if (strict) return *prev(s.lower_bound(x));
    else return *prev(s.upper_bound(x));
};

// false: >= x, true: > x
auto right = [&](const auto& s, auto x, bool strict = false) {
    if (strict) return *s.upper_bound(x);
    else return *s.lower_bound(x);
};
```

---

## 🔹 基本定義

```txt
lower_bound: 値以上(>=)の最初
upper_bound: 値より大きい(>)の最初
```

---

## 🔹 例（multiset）

```cpp
// {5,5,6,7,7,7}

// 最初の7
auto it = mst.lower_bound(7);

// 最後の7
auto it = mst.upper_bound(7);
if (it != mst.begin()) {
    --it;
} else {
    // 存在しない
}
```

---

## 🔹 競プロTips

- lower_bound は「挿入位置」を返す
- set / multiset は必ず境界チェック or 番兵
- `upper_bound(x)` は `lower_bound(x+1)` と同義（整数の場合）

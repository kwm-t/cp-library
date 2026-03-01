# sorted_set_safe.hpp

std::set / std::multiset 用の安全なユーティリティ集。

## 設計方針

- 番兵なし前提
- すべての探索系関数は
  見つからない場合 `end()` を返す
- auto 不使用
- set / multiset 両対応

## 重要な注意

返ってきた iterator は必ず end() チェックすること！

```cpp
auto it = lib::stl::sorted_set_safe::it_prev_leq(s, x);
if (it != s.end()) {
    // OK
    int v = *it;
}
```
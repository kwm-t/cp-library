# sorted_set_sentinel.hpp

番兵あり前提の sorted set / multiset ユーティリティライブラリです。  
競技プログラミング向けの「境界チェックを省いた高速・安全な操作」を目的としています。

---

## 前提条件

このライブラリは以下を **前提** とします：

- set / multiset に必ず **2つの番兵** を入れている
- 番兵の順序関係：

  min_sentinel < すべての要素 < max_sentinel

- 常に `size() >= 2` が成り立つ

```cpp
std::set<long long> s;
s.insert(-INF);   // 最小番兵
s.insert(+INF);   // 最大番兵
```
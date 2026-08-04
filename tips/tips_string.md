# 文字列 Tips

## S の suffix と T の prefix の一致最大値

`T + "_" + S` に KMP をかけて、最後の `next` を見る。  
`"_"` は **S, T のどちらにも含まれない文字**を選ぶ。

```cpp
auto ts = t + "_" + s;
kwm_t::string::KMP kmp(ts);
auto val = kmp.get_next().back();
```

`val` = `S` の suffix と `T` の prefix の最大一致長

## 例
https://atcoder.jp/contests/awc0111/submissions/77466007
https://atcoder.jp/contests/awc0127/submissions/78116953
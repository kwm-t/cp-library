# 文字列 Tips

## S の suffix と T の prefix の一致最大値

`S + "_" + T` に KMP をかけて、最後の `next` を見る。  
`"_"` は **S, T のどちらにも含まれない文字**を選ぶ。

```cpp
auto st = s + "_" + t;
kwm_t::string::KMP kmp(st);
auto val = kmp.get_next().back();
```

`val` = `S` の suffix と `T` の prefix の最大一致長

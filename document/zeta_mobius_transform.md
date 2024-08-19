## 高速ゼータ変換
長さが $2^n$ の配列 $A$がある。
以下を満たすような数列 $B$を $\mathcal{O}(n*2^n)$で求める事ができる。

```math
B_i = \sum_{j\subset i} A_j
```

## 愚直解

```cpp
std::vector<int> naive(int n,const std::vector<int>&a){
    std::vector<int>b(1<<n);
    for(int i = 0; i < (1<<n);++i){
        for(int j = 0;j <(1<<n);++j){
            if(j==(i&j))b[i]+=a[j];
        }
    }
    return b;
}
```
これだと、$\mathcal{O}(4^n)$ になる。
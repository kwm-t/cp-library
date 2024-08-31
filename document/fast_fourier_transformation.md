# 高速フーリエ変換
　$n$ 次の多項式 $A(x)$ と、 $n$次の多項式 $B(x)$ をかけ合わせた $C(x)$ を $\mathcal{O}(nlog(n))$ で求める

# 愚直解 $\mathcal{O}(n^2)$ 
```cpp
template <class T>
std::vector<int> naive(const std::vector<T> &a, const std::vector<T> &b) {
	std::vector<T>c((int)a.size() + (int)b.size() - 1);
	for (int i = 0; i < (int)a.size(); ++i) {
		for (int j = 0; j < (int)b.size(); ++j) {
			c[i + j] += a[i] * b[j];
		}
	}
	return c;
}
```
これを高速に求めるというが目標になります。

# 1のn乗根
以下、　$\zeta_n$=$e^{i \frac{\cdot 2\pi}{n}}$ と定める。<br>
 $\zeta_n$には以下の重要な性質がある。<br>

 - $\zeta_n^n=1$
 - $\zeta_n^0$ , $\zeta_n^1$ , $\zeta_n^2$ $\cdots$  $\zeta_n^{n-1}$はすべて異なる
 - $\sum_{k=0}^{N-1} \zeta_n^{kl}$は、$l\equiv0 (modN)$のとき $N$、そうでないときに $0$ である。

 # 離散フーリエ変換(DFT)
  $n$ 次の多項式 $f(x)=\sum_{k=0}^{N-1}C_kx^k $ を考えます。<br>
これに $x=\zeta_n^k$ を代入すると以下となる。

```math
	f(\zeta_n^k) = \sum_{j=0}^{N-1}c_j\zeta_N^{jk}
```
以下を $f(x)$の離散フーリエ変換(Discrete Fourier Transform)として $\hat{f}(x)$を定義する。
```math
　\hat{f}(t)=\sum_{k=0}^{N-1}f(\zeta_N^k)t^k
```
別の形で表現すると以下のようになる。
```math
　\hat{f}(t)=\sum_{k=0}^{N-1}\sum_{j=0}^{N-1}c_j\zeta_N^{jk}t^k
```

# 離散逆フーリエ変換（IDFT）
前章の $\hat{f}(t)$に $t=\zeta_n^{-l}$を代入すると以下のようになります。
```math
	\hat{f}(\zeta_n^{-l})=\sum_{k=0}^{N-1}\sum_{j=0}^{N-1}c_j\zeta_N^{jk}\zeta_n^{-lk}
```
整理すると以下になります。
```math
	\hat{f}(\zeta_n^{-l})=\sum_{j=0}^{N-1}c_j\sum_{k=0}^{N-1}\zeta_N^{k(j-l)}
```
後ろの $Σ$は $j=l$のときに $N$、そうでないときに $0$になるため
```math
	\hat{f}(\zeta_n^{-l})=Nc_l
```
となり
```math
c_l= \frac{1}{N}\hat{f}(\zeta_N^{-l})
```
となります。

# 高速フーリエ変換の手法
ここまでのことより以下の手順を取ればよいことが分かりました。<br>

- 多項式 $A(x)$と $B(x)$のそれぞれにDFTを行い<br>
 $\hat{A(x)}$と $\hat{B(x)}$を得る。
- $\hat{A(x)}$と $\hat{B(x)}$の各項をかけ合わせ $\hat{C(x)}$を得る。
- $\hat{C(x)}$に対してIDFTを行い、$C(x)=A(x)B(x)$を得る。

ですが、これを行うためにはDFTとIDFTを高速化しなくてはなりません。
 
 
# 参考
https://qiita.com/AngrySadEight/items/0dfde26060daaf6a2fda
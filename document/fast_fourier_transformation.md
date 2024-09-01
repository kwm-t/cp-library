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
以下、 $\zeta_n$= $e^{i \frac{\cdot 2\pi}{n}}$ と定める。<br>
 $\zeta_n$には以下の重要な性質がある。<br>

 - $\zeta_n^n=1$
 - $\zeta_n^0$ , $\zeta_n^1$ , $\zeta_n^2$ $\cdots$  $\zeta_n^{n-1}$はすべて異なる
 - $\sum_{k=0}^{N-1} \zeta_n^{kl}$は $l\equiv0 (modN)$のとき $N$、そうでないときに $0$ である。

# 離散フーリエ変換(DFT)
以降、Nは全て2冪の数とします。<br>
  $n$ 次の多項式 $f(x)=\sum_{k=0}^{N-1}c_kx^k$ を考えます。<br>
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
 $\hat{A}(x)$と $\hat{B}(x)$を得る。
- $\hat{A(x)}$と $\hat{B(x)}$の各項をかけ合わせ $\hat{C}(x)$を得る。
- $\hat{C}(x)$に対してIDFTを行い $C(x)=A(x)B(x)$ を得る。

ですが、これを行うためにはDFTとIDFTを高速化しなくてはなりません。

# DFTとIDFTの高速化
多項式 $f(x)$ に対して、偶数次のみを取り出した多項式 $f_e(x)$ と奇数次のみを取り出した多項式 $f_o(x)$ を考えます。
すなわち以下について考えます
```math
f_e(x) = c_0+c_2x+\cdots
```
```math
f_o(x) = c_1+c_3x+\cdots
```
これらは以下を満たします。
```math
f(x)=f_e(x^2)+xf_o(x^2)
```
 $f(x)$ に $x=\zeta_N^k$ を代入した結果を使って、DFTができるわけです。<br>
前述の $f(x)=f_e(x^2)+xf_o(x^2)$ に $x=\zeta_N^k$ を代入した結果を得るためには以下が必要になります。

- $f_e(\zeta_N^{0})$ , $f_e(\zeta_N^{2})$ , $f_e(\zeta_N^{4})$, $\cdots$ $f_e(\zeta_N^{2N-2})$ の $N$ 個の値
- $f_o(\zeta_N^{0})$ , $f_o(\zeta_N^{2})$ , $f_o(\zeta_N^{4})$, $\cdots$ $f_o(\zeta_N^{2N-2})$ の $N$ 個の値

 $\zeta_n^2 = \zeta_{n/2}$が成り立つので上記は以下のようになります。<br>

- $f_e(\zeta_{N/2}^{0})$ , $f_e(\zeta_{N/2}^{1})$ , $f_e(\zeta_{N/2}^{2})$, $\cdots$ $f_e(\zeta_{N/2}^{N-1})$ の $N$ 個の値
- $f_o(\zeta_{N/2}^{0})$ , $f_o(\zeta_{N/2}^{1})$ , $f_o(\zeta_{N/2}^{2})$, $\cdots$ $f_o(\zeta_{N/2}^{N-1})$ の $N$ 個の値

また、 $\zeta_{N/2}^{N/2} = 1$ が成り立つので上記は以下のように纏められます。

- $f_e(\zeta_{N/2}^{0})$ , $f_e(\zeta_{N/2}^{1})$ , $f_e(\zeta_{N/2}^{2})$, $\cdots$ $f_e(\zeta_{N/2}^{N/2-1})$ の $\frac{N}{2}$ 個の値
- $f_o(\zeta_{N/2}^{0})$ , $f_o(\zeta_{N/2}^{1})$ , $f_o(\zeta_{N/2}^{2})$, $\cdots$ $f_o(\zeta_{N/2}^{N/2-1})$ の $\frac{N}{2}$ 個の値

以上より、 $f(x)$ に $x=\zeta_N^k$ を代入した $N$個の結果を求めるためには<br>
 $f_e(x)$ と $f_o(x)$ に $x=\zeta_{N/2}^k$ を代入した $\frac{N}{2}$個の結果が求まれば<br>
 $f(\zeta_N^k) = f_e(\zeta_{N/2}^k) + \zeta_N^kf_o(\zeta_{N/2}^k)$ として求めることができます。<br>
これを $f_e(x)$と $f_o(x)$ に対して再帰的に繰り返すと<br>
再帰が $log(N)$回行われ、 $\mathcal{O}(Nlog(N))$ でFFTを行うことができました。


# 実装
```cpp
const double pi = acos(-1);

std::vector<std::complex<double>> fft(const std::vector<std::complex<double>> &a, double inv) {
	// inv = 1 ならば普通の FFT，inv = -1 ならば IFFT になるようにする．
	int n = (int)a.size();
	if (1 == n)return a;

	std::vector<std::complex<double>> ret;

	std::vector<std::complex<double>> even;
	std::vector<std::complex<double>> odd;
	for (int i = 0; i < n; ++i) {
		if (0 == i % 2) even.push_back(a[i]);
		else odd.push_back(a[i]);
	}

	auto d_even = fft(even, inv);
	auto d_odd = fft(odd, inv);

	const std::complex<double> zeta(cos(2 * pi * inv / (double)n), sin(2 * pi * inv / (double)n));

	std::complex<double> now = 1;
	for (int i = 0; i < n; i++) {
		ret.push_back(d_even[i % (n / 2)] + now * d_odd[i % (n / 2)]);
		now *= zeta;
	}
	return ret;
}


std::vector<double> convolution(std::vector<std::complex<double>> a, std::vector<std::complex<double>> b) {
	int sizeA = (int)a.size();
	int sizeB = (int)b.size();
	int sizeC = sizeA + sizeB - 1;

	int n = 1;
	while (n <= sizeC) n *= 2;

	a.resize(n);
	b.resize(n);

	auto da = fft(a, 1);
	auto db = fft(b, 1);

	std::vector<std::complex<double>> dc(n);
	for (int i = 0; i < n; ++i) dc[i] = da[i] * db[i];

	auto c = fft(dc, -1);

	// IFFT の後は最後に n で割る必要がある
	std::vector<double> ret(n);
	for (int i = 0; i < n; ++i) ret[i] = c[i].real() / (double)n;
	return ret;
}
```

# 課題
上記には課題が残っています。<br>
与えられた多項式 $A(x)$ $B(x)$が整数多項式の時、実装上で複素数を持ち出して少数を扱って計算を行っているため、場合によっては誤差が発生します。<br>
どうすればいいでしょうか？<br>
誤差発生した理由は、少数を扱ったからです。<br>
もし仮に、整数の世界のみで上記と同じことができれば課題は解決されます。<br>

# 数論変換
 $\zeta_N$ の代わりになるものがあればよいです。<br>
 $1のN$乗根の代わりに、 $modP$ の原始根を利用します。<br>
原始根の説明は省略します。<br>

ところで、 $r_N=r^{(p-1)/N}$と取れば、以下を満たします。
 - $r_N^N\equiv1(modP)$
 - $r_N^0$ , $r_N^1$ , $r_N^2$ $\cdots$  $r_N^{n-1}$はmodP上ですべて異なる
 - $\sum_{k=0}^{N-1} r_N^{kl}$は $l\equiv0 (modN)$のとき $N$、そうでないときに $0$ である。

ただし、 $P-1$ を $N$ が割り切れる必要があります。<br>
 $N$を2冪で定義していたことを思い出すと、 $P$ は $P-1$ が2を素因数として多く持つ必要があります。<br>
要するに、FFTをする際に再帰が走る深さより多く2を素因数として保つ必要があるということです。<br>
このような性質を持つ代表的な素数として $P=998244353$などが知られています。<br>
実際に $998244353 = 119*2^{23}+1$ となっています。<br>
また、 $mod998244353$の原始根として $r=3$が知られています。<br>
原始根を求めるアルゴリズムも存在しますが、今回は触れません。

# 実装
modは998244353で決め打ちしています
```cpp
std::vector<mint> make_root(long long mod) {
	std::vector<mint> ret(0);
	long long r = pow_mod(3, 119, mod);
	for (long long i = 0; i < 23; i++) {
		ret.push_back(r);
		r = (r * r) % mod;
	}
	reverse(ret.begin(), ret.end());
	return ret;
}

std::vector<mint> make_invroot(std::vector<mint> &root, long long mod) {
	std::vector<mint> ret;
	for (long long i = 0; i < root.size(); i++) {
		ret.push_back(root[i].inv());
	}
	return ret;
}

std::vector<mint> ntt(const std::vector<mint> &a, int depth, const std::vector<mint> &root) {
	int n = (int)a.size();
	if (n == 1) return a;

	std::vector<mint> ret(0);

	std::vector<mint> even(0);
	std::vector<mint> odd(0);
	for (long long i = 0; i < n; ++i) {
		if (0 == i % 2) even.push_back(a[i]);
		else odd.push_back(a[i]);
	}

	auto d_even = ntt(even, depth - 1, root);
	auto d_odd = ntt(odd, depth - 1, root);

	const mint r = root[depth];

	mint now = 1;
	for (long long i = 0; i < n; i++) {
		ret.push_back(d_even[i % (n / 2)] + now * d_odd[i % (n / 2)]);
		now *= r;
	}
	return ret;
}


std::vector<mint> convolution_mint(std::vector<mint> a, std::vector<mint> b) {
	std::vector<mint> root = make_root(mod);
	std::vector<mint> invroot = make_invroot(root, mod);

	int sizeA = (int)a.size();
	int sizeB = (int)b.size();
	int sizeC = sizeA + sizeB - 1;

	int n = 1;
	while (n <= sizeC) n *= 2;

	a.resize(n);
	b.resize(n);

	int log_2n = 1;
	while ((1 << log_2n) < n) log_2n++;

	auto da = ntt(a, log_2n - 1, root);
	auto db = ntt(b, log_2n - 1, root);

	std::vector<mint> dc(n);
	for (int i = 0; i < n; ++i) dc[i] = da[i] * db[i];

	auto c = ntt(dc, log_2n - 1, invroot);

	// INTT の後は最後に n で割ることを忘れずに．
	std::vector<mint> ret(n);
	mint invN = mint::raw(n).inv();
	for (int i = 0; i < n; ++i) ret[i] = c[i] * invN;
	return ret;
}
```

# 参考
https://qiita.com/AngrySadEight/items/0dfde26060daaf6a2fda
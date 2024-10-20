# Lucy DP
Lucy DP は [Project Euler: Problem 10](https://projecteuler.net/problem=10)のスレッドに投稿された Lucy_Hedgehog 氏の投稿に由来する DP です。<br>

以下のような問題の解を高速に求めることができる。<br>

- 正整数 $N$ が与えられる。 $N$ 以下の素数の個数を求めよ。<br>
- 正整数 $N$ が与えられる。 $N$ 以下の素数の和を求めよ。<br>

## 解説
$1 \leq x \leq \lfloor \sqrt N \rfloor,n \in Q_N$ にて以下のように $S(x,n)$ を定義します。<br>
- $S(n,x)$ : $2$ 以上 $N$ 以下の整数のうち、エラトステネスの篩のアルゴリズムにおいて $x$ 以下の素数では篩われなかった整数の個数<br>

$\pi(n)=S(n,\lfloor \sqrt N \rfloor)$ が成り立つのでこれを求められればよいです。<br>
※ $\pi(n)$ は素数計数関数

$S(n,x)$ のDPの遷移を考えます。<br>
- $x$ が素数でない場合<br>
 $x$ によって篩われるものは存在しないため $S(n,x) = S(n,x-1)$

- $x$ が素数かつ、$n < x^2 $の場合<br>
$n$ 以下の $x$ の倍数は、すでに $x$ 未満の素数によって篩われているため $S(n,x) = S(n,x-1)$

- $x$ が素数かつ、 $n \geqq x^2 $ の場合<br>
$x$ によって篩われる数 $mx$の条件は、「$x$ 未満の素数で篩われなかった数の集合」から「$x$ 未満の素数の集合」を除いたものであるため以下のようになります。<br>
$S(n,x-1)-S(n,x)=S(\lfloor n/x \rfloor,x-1)-\pi(x-1)$<br>
すなわち以下であり<br>
$S(n,x)=S(n,x-1) -S(\lfloor n/x \rfloor,x-1) + \pi(x-1)$<br>
また、$\pi(x-1)=S(x-1,x-1)$であるため<br>
$S(n,x)=S(n,x-1) -S(\lfloor n/x \rfloor,x-1) + S(x-1,x-1)$<br>

## 実装
```cpp
long long lucyDP(const long long N) {
	const long long sqrtN = std::sqrtl(N);
	std::vector<long long>Q;
	for (int i = 1; i <= sqrtN; ++i) Q.push_back(N / i);
	while (1 != Q.back()) Q.push_back(Q.back() - 1);

	std::unordered_map<long long, long long>mp;
	for (auto q : Q)mp[q] = q - 1;
	for (int p = 2; p <= sqrtN; ++p) {
		// not prime
		if (mp[p] == mp[p - 1])continue;
		auto pre = mp[p - 1];
		for (auto q : Q) {
			if (q < p * p)break;
			mp[q] -= mp[q / p] - pre;
		}
	}
	return mp[N];
}
```
in-placeに更新する実装になっていますが、２次元配列で書くとわかりやすい<br>
std::unordered_mapを使用しているが、適切に配列管理すれば不要になる<br>
## 計算量
$1 \leq x \leq \lfloor \sqrt N \rfloor,n \in Q_N$ であるため$\mathcal{O}(N \sqrt N)$ であるように思えます。<br>
しかし$S(N,\lfloor \sqrt N \rfloor)$ を求める過程で登場する$S(n,x)$ における$n$ の候補として現れる数の集合は$\mathcal{O}(\sqrt N)$個であることがわかります。<br>
また、素数定理より$N$ 以下の素数の数は$\mathcal{O}(\frac{N^{\frac{1}{4}}}{log_N})$ のため<br>
計算量は$\mathcal{O}(\frac{N^{\frac{3}{4}}}{log_N})$ となりある程度高速です。
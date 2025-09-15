## 高速ゼータ変換
長さが $2^n$ の配列 $A$がある。
以下を満たすような数列 $B$を $\mathcal{O}(n*2^n)$で求める事ができる。

```math
B_i = \sum_{j\subset i} A_j
```

## 愚直解 $\mathcal{O}(4^n)$ 

```cpp
template<typename T>
std::vector<T> naive(int n,const std::vector<T>& a){
    std::vector<T>b(1<<n);
    for(int i = 0; i < (1<<n);++i){
        for(int j = 0;j <(1<<n);++j){
            if(j==(i&j))b[i]+=a[j];
        }
    }
    return b;
}
```
これだと、 $\mathcal{O}(4^n)$ になってしまう。

## 高速ゼータ変換 $\mathcal{O}(n *2^n)$ 
やっていることは多次元の累積和と同じであることに気がつけば理解が容易である。
```cpp
template<typename T>
std::vector<T> zeta(int n, const std::vector<T>&a) {
	std::vector<std::vector<T>>b(n + 1, std::vector<T>(1 << n));
	b[0] = a;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < (1 << n); ++j) {
			if (1 & (j >> i)) {
				b[i + 1][j] = b[i][j & ~(1 << i)] + b[i][j];
			}
			else {
				b[i + 1][j] = b[i][j];
			}
		}
	}
	return b[n];
}
```

## 高速ゼータ変換(メモリ節約) $\mathcal{O}(n *2^n)$ 
上記の関数内の以下の遷移部分を見る<br>
```cpp
if (1 & (j >> i)) {
	b[i + 1][j] = b[i][j & ~(1 << i)] + b[i][j];
}
else {
	b[i + 1][j] = b[i][j];
}
```
これは $j$の $i$ bit目が立っていなければ何もせず、立っていれば $b[j-(1<<i)]$を加算してると解釈できるので以下のように書き改められる。
```cpp
template<typename T>
std::vector<T> zeta(int n, std::vector<T>z) {
	for (int j = 0; j < n; ++j) {
		for (int i = 0; i < (1 << n); ++i) {
			if (1 & (i >> j)) {
				z[i] += z[i & ~(1 << j)];
			}
		}
	}
	return z;
}
```
## 高速メビウス変換
ゼータ変換の逆操作をすることでもとに戻す事ができる
```cpp
template<typename T>
std::vector<T> mobius(int n, std::vector<T>z) {
	for (int j = n - 1; j >= 0; --j) {
		for (int i = (1 << n) - 1; i >= 0; --i) {
			if (1 & (i >> j)) {
				z[i] -= z[i & ~(1 << j)];
			}
		}
	}
	return z;
}
```

## 亜種

### $n進数$
```cpp
template<typename T>
std::vector<T> zetaP(const int p, const int n, std::vector<T> z) {
	// size|a| = pow(p,n)
	int pow = 1;
	for (int j = 0; j < n; ++j) {
		for (int i = 0; i < (int)z.size(); ++i) {
			if ((i / pow) % p)z[i] += z[i - pow];
		}
		pow *= p;
	}
	return z;
}
```

### 包含されている
```cpp
template<typename T>
std::vector<T> zeta(int n, std::vector<T> z) {
	for (int j = 0; j < n; ++j) {
		for (int i = 0; i < (1 << n); ++i) {
			if (1 & (i >> j)) {
				z[i - (1 << j)] += z[i];
			}
		}
	}
	return z;
}
```
### 倍数
```cpp
template<typename T>
std::vector<T> zeta(int n, std::vector<T> z) {
	auto plist = sieve(1 << n);
	for (int i = 0; i < plist.size(); ++i) {
		int p = plist[i];
		for (int j = ((1 << n) / p)*p; j > 0; j -= p) {
			z[j / p] += z[j];
		}
	}
	return z;
}
```
### 約数
```cpp
template<typename T>
std::vector<T> zeta(int n, std::vector<T> z) {
	auto plist = sieve(n);
	for (int i = 0; i < plist.size(); ++i) {
		int p = plist[i];
		for (int j = 1; j*p < n; ++j) {
			z[j * p] += z[j];
		}
	}
	return z;
}
```

## XOR畳み込み
長さ $2^n$ の数列 $X,Y$ に対して、XOR畳み込みを以下を満たす長さ  $2^n$ の数列 $Z$ として定める。<br>
$$ Z_k = \sum_{k=i\oplus j} X_iY_j$$
以降、$Z = X*Y$と書く<br>

to $(ax^2 + bx + c = 0)$ and they are 


## アダマール変換
アダマール変換 $H_n$ は $2^n$ × $2^n$ の行列であり、以下のように再帰的に定義される。<br><br>
$H_0=\left[\begin{array}{cc}1\end{array}\right]$<br><br>
$
  H_1=\frac{1}{\sqrt{2}}
  \left[
    \begin{array}{cc}
      1 & 1 \cr
      1 & -1
    \end{array}
  \right]
$<br><br>
$
  H_{i+1}=\frac{1}{\sqrt{2}}
  \left[
    \begin{array}{cc}
      H_{i} & H_{i} \cr
      H_{i} & -H_{i}
    \end{array}
  \right]
$<br><br>

$H_i*H_i = I$が成り立つ(帰納法より明らか)<br>
重要な性質として次が成り立つ。<br>
$$
    (H_{n})_{i,j}=\frac{1}{2^{n/2}}(-1)^{popcount(i\&j)}
$$
※正式な定義は上記だが、整数で扱うために定数倍を無視することもある(実装都合)<br>
$$
    \tilde{H_{i}} = 2^{n/2}H_{n}
$$

## XOR畳み込みとアダマール変換
長さ$2^n$の数列$a,b,c$が$a*b=c$を満たす時、以下の性質が成り立つ。<br>
$$
\begin{aligned} 
        2^{n/2}  H_{n}a \odot H_{n}b
        &=
        H_{n}c
\end{aligned}
$$

## 証明
$
    2^{n/2}  H_{n}(H_{n}u \odot H_{n}v)
    =
    u*v
$

を示せば良い。これは帰納法より示せる。
$$
    \begin{aligned} 
        2^{({k+1})/2}  H_{k+1}(H_{k+1}u \odot H_{k+1}v)
        &=
        2^{({k+1})/2}
        H_{k+1}
        \left(
            \frac{1}{\sqrt{2}}
            \left[
                \begin{array}{cc}
                    H_{k}u_{0} + H_{k}u_{1} \cr
                    H_{k}u_{0} - H_{k}u_{1}
                \end{array}
            \right]
            \odot
            \frac{1}{\sqrt{2}}
            \left[
                \begin{array}{cc}
                    H_{k}v_{0} + H_{k}v_{1} \cr
                    H_{k}v_{0} - H_{k}v_{1}
                \end{array}
            \right]
        \right)
        \\\\
        &=
        2^{({k+1})/2} 
        \frac{1}{\sqrt{2}}
        \left[
            \begin{array}{cc}
            H_{k} & H_{k} \cr
            H_{k} & -H_{k}
            \end{array}
        \right]
        \left(
            \frac{1}{\sqrt{2}}
            \left[
                \begin{array}{cc}
                    H_{k}u_{0} + H_{k}u_{1} \cr
                    H_{k}u_{0} - H_{k}u_{1}
                \end{array}
            \right]
            \odot
            \frac{1}{\sqrt{2}}
            \left[
                \begin{array}{cc}
                    H_{k}v_{0} + H_{k}v_{1} \cr
                    H_{k}v_{0} - H_{k}v_{1}
                \end{array}
            \right]
        \right)
        \\\\
        &=
        \frac{2^{{k}/2}}{2} 
        \left[
            \begin{array}{cc}
            H_{k} & H_{k} \cr
            H_{k} & -H_{k}
            \end{array}
        \right]
        \left(
            \begin{array}{cc}
            (H_{k}u_{0} + H_{k}u_{1})\odot(H_{k}v_{0} + H_{k}v_{1}) \cr
            (H_{k}u_{0} - H_{k}u_{1})\odot(H_{k}v_{0} - H_{k}v_{1})
            \end{array}
        \right)
        \\\\
        &=
        2^{{k}/2} 
        \left(
            \begin{array}{cc}
                H_{k}(H_{k}u_{0}\odot H_{k}v_{0}) + H_{k}(H_{k}u_{1}\odot H_{k}v_{1}) \cr
                H_{k}(H_{k}u_{0}\odot H_{k}v_{1}) + H_{k}(H_{k}u_{1}\odot H_{k}v_{0})
            \end{array}
        \right)
        \\\\
        &=
        \left(
                \begin{array}{cc}
                u_{0}*v_{0}+u_{1}*v_{1} \cr
                u_{0}*v_{1}+u_{1}*v_{0}
            \end{array}
        \right)
        \\\\
        &=
        u*v
    \end{aligned}
$$

## 高速Walsh-Hadamard変換
上記アダマール変換を愚直に行うと$\mathcal{O}(4^n)$になる。<br>
これを$\mathcal{O}(n2^n)$で行うアルゴリズムがあり高速Walsh-Hadamard変換という。

## アルゴリズム
$H_{k}u$を求めるのを目標とします($u$は長さ$2^n$の数列)<br>
アダマール変換は再帰的に構成されていることを思い出すと以下が成立してます。
$$
    \begin{aligned} 
    H_{k}u
        &=
        \frac{1}{\sqrt{2}}
        \left(
            \begin{array}{cc}
                E_{k-1}&E_{k-1}\cr
                E_{k-1}&-E_{k-1}
            \end{array}
        \right)
        \left(
                \begin{array}{cc}
                H_{k-1}&0\cr
                0&-H_{k-1}
            \end{array}
        \right)
        u
        \\\\
        &=
        \frac{1}{2}
        \left(
                \begin{array}{cc}
                E_{k-1}&E_{k-1}\cr
                E_{k-1}&-E_{k-1}
            \end{array}
        \right)
        \left(
                \begin{array}{cc}
                E_{k-2}&E_{k-2}&0&0\cr
                E_{k-2}&-E_{k-2}&0&0\cr
                0&0&E_{k-2}&E_{k-2}\cr
                0&0&E_{k-2}&-E_{k-2}
            \end{array}
        \right)
        \left(
                \begin{array}{cc}
                H_{k-2}&0&0&0\cr
                0&H_{k-2}&0&0\cr
                0&0&H_{k-2}&0\cr
                0&0&0&H_{k-2}
            \end{array}
        \right)
        u

    \end{aligned}
$$
この変形を繰り返すと最終的に$H_{0}$が対角線上に並びます。<br>
また重要な点として、分解に現れる行列は各業に非零な要素を２つしか持たないのでベクトルと行列の積がそれぞれ$\mathcal{O}(2^n)$で行えます。<br>
そしてその掛け算が$n$回行われるので計算量が$\mathcal{O}(n2^n)$になります。

## 実装例
```cpp
template <typename T>
void walsh_hadamard_transform(std::vector<T>& f) {
	int n = f.size();
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; j++) {
			if ((j & i) == 0) {
				T x = f[j], y = f[j | i];
				f[j] = x + y, f[j | i] = x - y;
			}
		}
	}
}
```
$f$の添字に当たる部分は上記に記載したアルゴリズムで分解した部分をぐっと睨みつけると<br>
$f[p]$を$f[p]+f[q]$で、$f[q]をf[p]-f[q]$で同時に更新するような操作が何回も行われていること<br>
pとqの関係性を考えれば良い。<br>
※二冪の部分は省略しているのでinverseする際に補正するなどの対応が必要


## その他の畳み込み
行列の形を変えxorのときと同様に再帰的に構成するとandやorへの畳み込みにも対応が出来る

andの場合

$
  H_{1}=
  \left[
    \begin{array}{cc}
      1 & 1 \cr
      0 & 1
    \end{array}
  \right],H_{1}^{-1}=
  \left[
    \begin{array}{cc}
      1 & -1 \cr
      0 & 1
    \end{array}
  \right]
$
$$
    \begin{aligned} 
        H_{k+1}^{-1}(H_{k+1}u \odot H_{k+1}v)
        &=
        H_{k+1}^{-1}
        \left(
            \left[
                \begin{array}{cc}
                    H_{k}u_{0} + H_{k}u_{1} \cr
                    H_{k}u_{1}
                \end{array}
            \right]
            \odot
            \left[
                \begin{array}{cc}
                    H_{k}v_{0} + H_{k}v_{1} \cr
                    H_{k}v_{1}
                \end{array}
            \right]
        \right)
        \\\\
        &=
        \left[
            \begin{array}{cc}
            H_{k}^{-1} & -H_{k}^{-1} \cr
            0 & H_{k}^{-1}
            \end{array}
        \right]
        \left(
            \left[
                \begin{array}{cc}
                    H_{k}u_{0} + H_{k}u_{1} \cr
                    H_{k}u_{1}
                \end{array}
            \right]
            \odot
            \left[
                \begin{array}{cc}
                    H_{k}v_{0} + H_{k}v_{1} \cr
                    H_{k}v_{1}
                \end{array}
            \right]
        \right)
        \\\\
        &=
        \left[
            \begin{array}{cc}
            H_{k}^{-1} & -H_{k}^{-1} \cr
            0 & H_{k}^{-1}
            \end{array}
        \right]
        \left(
            \begin{array}{cc}
            (H_{k}u_{0} + H_{k}u_{1})\odot(H_{k}v_{0} + H_{k}v_{1}) \cr
            H_{k}u_{1}\odot H_{k}v_{1}
            \end{array}
        \right)
        \\\\
        &=
        \left(
            \begin{array}{cc}
                H_{k}^{-1}(H_{k}u_{0}\odot H_{k}v_{0}) + H_{k}^{-1}(H_{k}u_{0}\odot H_{k}v_{1}) + H_{k}^{-1}(H_{k}u_{1}\odot H_{k}v_{0}) \cr
                H_{k}^{-1}(H_{k}u_{1}\odot H_{k}v_{1})
            \end{array}
        \right)
        \\\\
        &=
        \left(
                \begin{array}{cc}
                u_{0}*v_{0}+ u_{0}*v_{1}+u_{1}*v_{0}\cr
                u_{1}*v_{1}
            \end{array}
        \right)
        \\\\
        &=
        u*v
    \end{aligned}
$$
orの場合

$
  H_{1}=
  \left[
    \begin{array}{cc}
      1 & 0 \cr
      1 & 1
    \end{array}
  \right],H_{1}^{-1}=
  \left[
    \begin{array}{cc}
      1 & 0 \cr
      -1 & 1
    \end{array}
  \right]
$
$$
    \begin{aligned} 
        H_{k+1}^{-1}(H_{k+1}u \odot H_{k+1}v)
        &=
        H_{k+1}^{-1}
        \left(
            \left[
                \begin{array}{cc}
                    H_{k}u_{0}  \cr
                    H_{k}u_{0} + H_{k}u_{1}
                \end{array}
            \right]
            \odot
            \left[
                \begin{array}{cc}
                    H_{k}v_{0} \cr
                    H_{k}v_{0} + H_{k}v_{1}
                \end{array}
            \right]
        \right)
        \\\\
        &=
        \left[
            \begin{array}{cc}
            H_{k}^{-1} & 0 \cr
            -H_{k}^{-1}  & H_{k}^{-1}
            \end{array}
        \right]
        \left(
            \left[
                \begin{array}{cc}
                    H_{k}u_{0}  \cr
                    H_{k}u_{0} + H_{k}u_{1}
                \end{array}
            \right]
            \odot
            \left[
                \begin{array}{cc}
                    H_{k}v_{0} \cr
                    H_{k}v_{0} + H_{k}v_{1}
                \end{array}
            \right]
        \right)
        \\\\
        &=
        \left[
            \begin{array}{cc}
            H_{k}^{-1} & 0 \cr
            -H_{k}^{-1}  & H_{k}^{-1}
            \end{array}
        \right]
        \left(
            \begin{array}{cc}
            H_{k}u_{0}\odot H_{k}v_{0} \cr
            (H_{k}u_{0} + H_{k}u_{1})\odot(H_{k}v_{0} + H_{k}v_{1})
            \end{array}
        \right)
        \\\\
        &=
        \left(
            \begin{array}{cc}
                H_{k}^{-1}(H_{k}u_{0}\odot H_{k}v_{0})\cr
                H_{k}^{-1}(H_{k}u_{0}\odot H_{k}v_{1}) + H_{k}^{-1}(H_{k}u_{1}\odot H_{k}v_{0}) + H_{k}^{-1}(H_{k}u_{1}\odot H_{k}v_{1}) 
            \end{array}
        \right)
        \\\\
        &=
        \left(
                \begin{array}{cc}
                u_{0}*v_{0}\cr
                u_{0}*v_{1}+ u_{1}*v_{0}+u_{1}*v_{1}
                
            \end{array}
        \right)
        \\\\
        &=
        u*v
    \end{aligned}
$$

## 実装例(再掲)
```cpp
#include <vector>
// 添字xor畳み込み(アダマール変換)
template <typename T>
void walsh_hadamard_transform(std::vector<T>& f) {
	int n = f.size();
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; j++) {
			if ((j & i) == 0) {
				// 添字 xor
				T x = f[j], y = f[j | i];
				f[j] = x + y, f[j | i] = x - y;
				/*
					// 添字 or
					 f[j | i] += f[j];
				*/
				/*
					// 添字 and
					f[j] += f[j | i];
				*/
			}
		}
	}
}
template <typename T>
void walsh_hadamard_transform_inv(std::vector<T>& f) {
	int n = f.size();
	T inv2 = T(2).inv();
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; j++) {
			if ((j & i) == 0) {
				// 添字 xor
				T x = f[j], y = f[j | i];
				f[j] = (x + y) * inv2, f[j | i] = (x - y) * inv2;
				/*
					// 添字 or
					f[j | i] -= f[j];
				*/
				/*
					// 添字 and
					f[j] -= f[j | i];
				*/
			}
		}
	}
}
template <typename T>
std::vector<T> operator*(std::vector<T> a, std::vector<T> b) {
	walsh_hadamard_transform(a);
	walsh_hadamard_transform(b);
	for (int i = 0; i < a.size(); ++i) a[i] *= b[i];
	walsh_hadamard_transform_inv(a);
	return a;
}
```

## 参考
https://qiita.com/izu_nori/items/7810fc2bc0423c8d407f<br>
https://sapphire15.hatenablog.com/entry/2021/09/13/114900
# z_Algorithm
長さ $N$文字列 $S$に対し、長さ $N$の配列を返す<br>
 $i$番目の要素は $S[0..n)$と $S[i..n)$のLCP(Longest Common Prefix)の長さ

# 例
 $S$=abcababcが入力として与えられた時<br>
 $Z$=[8,0,0,2,0,3,0,0]を返す

# 利用例
z_algorithmを用いると文字列Tが文字列Sに含まれるかという判定問題を$\mathcal{O}(|S| + |T|)$で行うことが出来る<br>

## 例
 $S$ = abcba, $T$ = abcの時<br>
  $T$と $S$をこの順に連結した文字列の $T+S$=abcbaabcに対してz_Algorithmを用いれば良い

# アルゴリズム

## 手順
以下の手順を行う<br>
以下、与えられた文字列を $S$、結果配列を $Z$とする<br>
<br>
初期化<br>
 $l$ = 0, $r$ = 0, $Z$ = [n,0,0,0,,,,,0]で初期化を行う

$i\in[1,|S|)$に対して以下の処理を繰り返す<br>
- $Z[i-l] < r-i$ の時(Case.1)
    - $Z[i] = Z[i-l]$を代入
- $Z[i-l] \geq r-i$  の時(Case.2)
    - $r=max(r,i)$を代入
    - $r < n$かつ $S[r] = S[r-i]$を満たす間$r$をインクリメントする
    - $Z[i] = r-i$, $l=i$を代入

## 正当性
### 前提
変数 $l,r$は上記の手順内のCase1,Case2の終了後に常に以下の性質を満たす。<br>
```math
l\leq i\leq r
```
```math
S[l..r) = S[0,r-l)
```
これは以下と同値<br>
変数 $l,r$は上記の手順内のCase1,Case2の開始前に常に以下の性質を満たす。<br>
```math
l\leq i-1 \leq r
```
```math
S[l..r) = S[0,r-l)
```

 $i = 1$の時 $0 \leq  0 \leq  0$かつ $S[0..0) = S[0..0)$より成り立つことが確認できる<br>
$1 < i$に関しては以降で帰納法にて証明する

### アルゴリズムの解説
 $l\leq i \leq r$を満たす時、以下が成り立つ<br>
 $S[l..r) = S[0..r-l)より、S[i..r) = S[i-l..r-l)$<br>
 $S[i-l..i-l + Z[i-l]) = S[0..Z[i-l])かつS[i-l + Z[i-l]] ≠ S[Z[i-l]]を満たす$<br>
 後者はz_algorithmの定義より明らか


- $i-l + Z[i-l] < r-l  ⇔ Z[i-l] < r-i$　の場合
    - $S[i..i+Z[i-l]) = S[i-l..i-l+Z[i-l]) = S[0..Z[i-l])$かつ<br>
 $S[i+Z[i-l]] = S[i-l + Z[i-l]] ≠ S[Z[i-l]]$より<br>
 $Z[i] = Z[i-l]$を満たす。<br>
 これは手順内のCase1に相当する
- $Z[i-l] \geq r-i$　の場合
    - $i>r$の場合( $i=r+1$)
        - $l = i,r = i$を代入
		- $S[r-i] = S[r]$を満たす間$r$++を行う
        - $Z[i] = r-i$を代入
    - $i \leq r$の場合
        - todo
        - $S[r-i] = S[r]$を満たす間$r$++を行う
        - $Z[i] = r-i$を代入




### 計算量解析
Case2の2. increment内にて $r$をインクリメントする回数に着目します<br>
$r$は単調増加であり、上限が存在することより $\mathcal{O}(|S|)$となることがわかる。

## 実装
```cpp
#include <vector>
#include <string>
#include <algorithm>
template <class T>
std::vector<int> z_algorithm(const std::vector<T>& s) {
	int n = (int)s.size();
	if (0 == n)return {};
	std::vector<int>z(n);
	z[0] = n;
	int l = 0, r = 0;
	for (int i = 1; i < n; ++i) {
		if (z[i - l] < r - i) {
			z[i] = z[i - l];
		}
		else {
			r = std::max(r, i);
			while (r < n && s[r] == s[r - i]) {
				r++;
			}
			z[i] = r - i;
			l = i;
		}
	}
	return z;
}
std::vector<int> z_algorithm(const std::string& s) {
	int n = (int)s.size();
	std::vector<int> subS(n);
	for (int i = 0; i < n; i++) {
		subS[i] = s[i];
	}
	return z_algorithm(subS);
}
```
## 参考
https://qiita.com/t_fuki/items/e682238dda6ad832ce05




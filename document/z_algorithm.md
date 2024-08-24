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

$i\in[i,|S|)$に対して以下の処理を繰り返す<br>
#### Case1: $Z[i-l] < r-i$
1. update
    - $Z[i] = Z[i-l]$を代入

#### Case2: $Z[i-l] \geq r-i$
 1. update
    - $r=max(r,i)$を代入
 2. increment
    - $r < n$かつ $S[r] = S[r-i]$を満たす間$r$をインクリメントする
 3. update
    - $Z[i] = r-i$, $l=i$を代入

### 正当性
変数 $l,r$は上記の手順内のCase1,Case2の終了後に常に以下の性質を満たす。<br>
```math
l\leq i\leq r
```
```math
S[l..r) = S[0,r-l)
```
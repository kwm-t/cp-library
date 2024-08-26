# Suffix Array(SA-IS)
文字列 $S$のSuffix $( S[i..|S|))$の集合を辞書順にソートしたときの
index列をSuffix Arrayという<br>
SA-IS(Suffix Array Induced Sorting)を用いることで $\mathcal{O}(|S|)$での構築が可能

# 例
 $S =mississippi $の場合<br>
```
10:i
 7:ippi
 4:issippi
 1:ississippi
 0:mississippi
 9:pi
 8:ppi
 6:sippi
 5:ssippi
 3:sissippi
 2:ssissippi
```
となるため<br>
Suffix Array(以下 $SA$及び $SA$配列)は、<br>
 $SA=[10,7,4,1,0,9,8,6,5,3,2]$となる

# 使用例
文字列 $S$から文字列 $T$を検索する<br>
 $S=missisippi$<br>
 $T=is$<br>
の場合 $m"is"s"is"ippi$の2個所マッチする。<br>
この検索を $SA$配列を $\mathcal{O}(|S|)$で構築し、検索を $\mathcal{O}(|T|*log|S|)$で行える<br>
<br>
 $S$を固定して様々な $T$に対して処理を行いたいときに有用


# アルゴリズム
## 前提と用語の説明
### 番兵
文字列の最後に $\$$ や $*$などの、文字列に含まれるどの文字よりも辞書順で先にくる仮の文字を付与する。<br>
実装都合の含め、以下の説明では $\$$を採用する<br>
この文字を番兵と呼ぶ<br>
例<br>
```
before:missisippi
after :missisippi$
```

### L型/S型
文字列内のすべての文字を以下のルールでL型/S型に分類する<br>
- 番兵はS型
- $S[i..|S|) > S[i+1..|S|)$ならL型
- $S[i..|S|) < S[i+1..|S|)$ならS型

※注<br>
$S[i..|S|) = S[i+1..|S|)$にはならない(文字列長が違うため自明)<br>
<br>
例<br>
```
mississippi$
LSLLSLLSLLLS
```
<br>
これを求めるには文字列を逆順に走査していけばいい( $\mathcal{O}(|S|)$)<br>
以降、この配列を $SL$配列と呼ぶ。

### LMS(Left-most S)/LMS部分文字列(LMS-substring)
上記の手順でL型/S型を求めたのち<br>
 $S[i-1]$がL型、 $S[i]$がS型になっている時、 $i$をLMSという。<br>
<br>
 $i$と $j$がLMSかつ、全ての $i<k<j$となる $k$がLMSでないとき<br>
 $S[i..j+1)$をLMS部分文字列と呼ぶ<br>
また、例外的に最後の$\$$のみもLMS部分文字列とする。<br>

例<br>
```
mississippi$のLMS部分文字列は以下
issi,issi,ippi$,$
```
## アルゴリズム概要
以下の手順でSuffix Arrayを求める
- Step1
    - LMS-substringのソート
    - induced_sortを行う(後述)
- Step2
    - ソートされたLMS-substringを元に<br>
	新たに作り出した文字列のSuffix Arrayを求める<br>
- Step3
    - 再度induced_sortを行い、Suffix Arrayを構築する

## アルゴリズム説明
### 最終結果(具体例)
以下の説明では全て $S=missisippi\$$とする<br>
Suffix Arrayを構築したあとの最終結果を掲載する。
```
str:mississippi$
idx:012345678901
L/S:LSLLSLLSLLLS
index,SA[i],S[SA[i]],SL[SA[i]],S[SA[i]..|S|)
00,11,$,S,$
01,10,i,L,i$
02,07,i,S,ippi$
03,04,i,S,issippi$
04,01,i,S,ississippi$
05,00,m,L,mississippi$
06,09,p,L,pi$
07,08,p,L,ppi$
08,06,s,L,sippi$
09,03,s,L,ssippi$
10,05,s,L,sissippi$
11,02,s,L,ssissippi$
```

### bucket sort
アルゴリズムの最終結果として得られるSA配列から<br>
index, $S[sa[i]]$, $SL[sa[i]]$を抽出したものは以下
```
index,S[SA[i]],SL[SA[i]]
00,$,S
01,i,L
02,i,S
03,i,S
04,i,S
05,m,L
06,p,L
07,p,L
08,s,L
09,s,L
11,s,L
10,s,L
```
これには以下の特徴があることがわかる<br>
- $S[SA[i]]$がソートされている
- $S[SA[i]]$が等しいものに着目するとLがSよりあとに来ることがない<br>
([1..5)=[L,S,S,S])

これが常に成り立つことを確認する。<br>
前半は、Suffix Arrayの定義より明らか<br>
後半の証明は以下<br>
#### 証明
L型のindexを $i0$,S型のindexを $i1$としたときに<br>
L型は定義より $S[i0] \geq S[i0+1]$であり<br>
S型は定義より $S[i1] \leq S[i1+1]$であるため<br>
 $S[i0+1]$と $S[i1+1]$が異なるなら明らか<br>
 $S[i0+1]=S[i1+1]$なら $S[i0+2]とS[i1+2]$で同様に考える。<br>


上記より、累積和などの前処理を適切に行えば<br>
頭文字とS型/L型が分かれば、最終結果がどの区間に入るかがわかる。<br>

以下頭文字が同じものを同じバケットと呼ぶ<br>
例：[1,5)や[8,12)は同じバケット

## induced sort
与えられたLMSのindex列に対して以下の処理を行う<br>
- Step1
    - LMSのindex列を後ろから走査し、バケットへ後ろから詰めていく
- Step2
	- Step1で作ったSA配列を前から走査し、以下の処理を行う<br>
	配列に入っている要素の一つ前の文字がL型なら、バケットへ前から詰めていく<br>
	※一つ前の要素がSであったり、存在しないときは何も行わない
- Step3
    - Step2で作った配列SAを後から走査し、以下の処理を行う<br>
	配列に入っている要素の一つ前の文字がS型なら、バケットへ後ろから詰めていく<br>
	※一つ前の要素がLであったり、存在しないときは何も行わない

### induced sort(例1)
与えられたLMSのindex列が $[1,4,7,11]$の時<br>
- Step1終了後
    ```
    index,SA[i],S[SA[i]],SL[SA[i]],S[SA[i]..|S|)
    00,11,$,S,$
    01,??,i,L,??
    02,01,i,S,ississippi$
    03,04,i,S,issippi$
    04,07,i,S,ippi$
    05,??,m,L,??
    06,??,p,L,??
    07,??,p,L,??
    08,??,s,L,??
    09,??,s,L,??
    10,??,s,L,??
    11,??,s,L,??
    ```
- Step2終了後
    ```
    index,SA[i],S[SA[i]],SL[SA[i]],S[SA[i]..|S|)
	00,11,$,S,$
	01,10,i,L,i$
	02,01,i,S,ississippi$
	03,04,i,S,issippi$
	04,07,i,S,ippi$
	05,00,m,L,mississippi$
	06,09,p,L,pi$
	07,08,p,L,ppi$
	08,03,s,L,sissippi$
	09,06,s,L,sippi$
	10,02,s,L,ssissippi$
	11,05,s,L,ssippi$
    ```
- Step3終了後
    ```
    index,SA[i],S[SA[i]],SL[SA[i]],S[SA[i]..|S|)
	00,11,$,S,$
	01,10,i,L,i$
	02,07,i,S,ippi$
	03,01,i,S,ississippi$
	04,04,i,S,issippi$
	05,00,m,L,mississippi$
	06,09,p,L,pi$
	07,08,p,L,ppi$
	08,03,s,L,sissippi$
	09,06,s,L,sippi$
	10,02,s,L,ssissippi$
	11,05,s,L,ssippi$
    ```

(この例では全てのS型がLMSなので例としてイマイチです)

### induced sort(例1)で得られたもの
この処理によって得られたSAは、正しいものではありません。<br>
求めたい最終結果と見比べると異なる点が何点かあることがわかります。<br>
<br>
なぜでしょうか？<br>
これはStep1で詰めたものが、最初の文字でしかソートされていないからです。<br>
もし、Step1で詰めたものが、完全にソートされていれば完全なSuffix Arrayが求められます<br>

#### 証明(簡略)
Step2で埋められるものに注目します<br>
同じバケットに含まれる $SA[j]$と $SA[k]$がStep2で埋められたとします $(j<k)$<br>
もし、 $S[SA[j]..|S|) > S[SA[k]..|S|)$であったらならば<br>
 $S[SA[j]+1..|S|) > S[SA[k]+1..|S|)$が成り立ちます。<br>
また、<br>
 $S[SA[j]+1..|S|) = S[SA[p]..|S|)$<br>
 $S[SA[k]+1..|S|) = S[SA[q]..|S|)$を満たす $p,q$が存在し、<br>
 $S[SA[p]..|S|) > S[SA[q]..|S|)$が成り立ちます<br>
Step2ではルール上バケットの上から詰めるため $p<q$を満たすはずですが<br>
これはStep1.で詰めたものが完全にソートされているという前提に反します<br>

Step3.はStep2の逆なので、Step2.と同等の理由で正当です。<br>
※ここめちゃめちゃにごまかしているのでざっくりで読んでください。<br>

### ここまでの纏め(induced sort)
induced sortを行う際にLMSのsuffixをソートされたindex順( $mississippi\$$の時 $[11,7,4,1]$)を与えれば<br>
- 1 LMSのindex列を元にL型をソート
- 2 ソートされたL型を元にS型をソート
<br>

を行うことでSuffix Arrayが求まることがわかりました。<br>
では、ソートされたLMSのsuffixのindex列はどのようにして得ればよいでしょうか。

### 改：induced sort(例1)で得られたもの
induced sortに戻ります。<br>
<br>
前述の通り、induced sort(例1)では<br>
Step1で詰めたものは最初の文字でしかソートされていません。<br>
そのことに注意して、ソートされている箇所のみに着目した上で<br>
もう一度induced sortの流れを追ってみます。<br>
- Step1終了後
    ```
    index,SA[i],S[SA[i]],SL[SA[i]],S[SA[i]..|S|)
	00,11,$,S,$
	01,??,i,L,??
	02,01,i,S,i
	03,04,i,S,i
	04,07,i,S,i
	05,??,m,L,??
	06,??,p,L,??
	07,??,p,L,??
	08,??,s,L,??
	09,??,s,L,??
	10,??,s,L,??
	11,??,s,L,??
    ```
- Step2終了後
    ```
    index,SA[i],S[SA[i]],SL[SA[i]],S[SA[i]..|S|)
	00,11,$,S,$
	01,10,i,L,i$
	02,01,i,S,i
	03,04,i,S,i
	04,07,i,S,i
	05,00,m,L,mi
	06,09,p,L,pi$
	07,08,p,L,ppi$
	08,03,s,L,si
	09,06,s,L,si
	10,02,s,L,ssi
	11,05,s,L,ssi
    ```
- Step3終了後
    ```
    index,SA[i],S[SA[i]],SL[SA[i]],S[SA[i]..|S|)
	00,11,$,S,$
	01,10,i,L,i$
	02,07,i,S,ippi$
	03,01,i,S,issi
	04,04,i,S,issi
	05,00,m,L,mi
	06,09,p,L,pi$
	07,08,p,L,ppi$
	08,03,s,L,si
	09,06,s,L,si
	10,02,s,L,ssi
	11,05,s,L,ssi
    ```

<br>
ソートされていない箇所を無視したため、得られた結果が短くなっています。<br>
ここで重要なのは、ソートされていない箇所を無視したことでソートされていること<br>
及び、LMSの箇所に着目するとLMS-substringの辞書順が求まっていることです<br>
<br>
どういうことでしょうか？<br>
序盤に以下のような例を示しました<br>

```
mississippi$の場合のLMS部分文字列は以下
issi,issi,ippi$,$
```
これが以下の部分と対応しています。
```
index,SA[i],S[SA[i]],SL[SA[i]],S[SA[i]..|S|)
00,11,$,S,$
02,07,i,S,ippi$
03,01,i,S,issi
04,04,i,S,issi
```
よって、LMS部分文字列の辞書順indexが分かります $[11,7,1,4]$

### ソートされたLMSのsuffixのindex列の取得
必要としているindex列なにかを思い出します。
```
SA[i],S[SA[i]..|S|)
01,ississippi$
04,issippi$
07,ippi$
11,$
```
上記の辞書順である。 $[11,7,4,1]$です。<br>
これを求める方法を考えます。<br>
これらの文字列は全て、LMS部分文字列を結合したものであることがわかります
(厳密には結合部分の重複あり)
```
01,ississippi$="issi"+"issi"+"ippi$"+"$"
04,issippi$="issi"+"ippi$"+"$"
07,ippi$="ippi$"+"$"
11,$="$"
```
また、すでにLMS部分文字列の辞書順indexがわかっていることより<br>
以下のような置換を行います
```
$→A
ippi$→B
issi→C
```
すると
```
01,CCBA
04,CBA
07,BA
11,A
```
になり、これの辞書順である $[11,7,4,1]$を求めればよいことになりました。<br>
これは $CCBA$のSuffix Arrayに他なりません<br>
```
03,A
02,BA
01,CBA
00,CCBA
```
以上より、文字列 $S$のSuffix Arrayを求めるためには<br>
新たな文字列 $S'$ のSuffix Arrayが分かれば良く<br>
本アルゴリズム(SA-IS法)を再帰的に適用することで求められることが分かりました。<br>
## 計算量
再帰的にSA-IS法を適当する部分の計算量を考えます<br>
文字列から新たな文字列を作り出し、それに対して再起を行っています。<br>
新たに作られる文字列長さはどうなるでしょうか？<br>
LMS部分文字列を一つの文字として扱っていることより<br>
元の文字列の半分以下となります。<br>
よって $\mathcal{O}(n+n/2+n/4+....)= \mathcal{O}(n)$となるため線形での構築が行えます<br>

# 実装
```cpp

class SuffixArray {
public:
	SuffixArray(std::string s) :S(s) {
		build();
	}
	std::vector<int> operator()() {
		return SA;
	}
private:
	std::string S;
	std::vector<int>SA;
	const char SENTINEL = '$';
	const int KIND = 128;
	void build() {
		// 番兵追加
		S += SENTINEL;

		// char->int
		std::vector<int>str(S.size());
		for (int i = 0; i < (int)S.size(); ++i) str[i] = S[i] - SENTINEL;

		// sa-isでSuffixArrayを求める
		SA = sa_is(str, KIND);

		// 番兵を取り除く
		SA.erase(SA.begin());
	}

	// sa-isでSuffixArrayを求める
	std::vector<int> sa_is(const std::vector<int> &str, const int &kind) {
		const int n = str.size();
		// Step0
		// S/L,LMSを前計算しておく
		auto is_s = calcS(str);
		auto is_lms = calcLMS(is_s);
		std::vector<int>lms_pos;
		for (int i = 0; i < n; ++i)if (is_lms[i])lms_pos.push_back(i);

		// Step1
		// induced sortを行い疑似SAを求める
		// LMS部分文字列の辞書順を取得する
		auto pseudo_sa = induced_sort(str, lms_pos, is_s, kind);

		// Step2
		// LMS の suffixの辞書順を求める
		std::vector<int> lms_order;
		lms_order.reserve((int)lms_pos.size());
		for (auto e : pseudo_sa)if (is_lms[e]) lms_order.push_back(e);

		// 連続するLMS部分文字列の比較を行う
		int rank = 0;
		pseudo_sa[lms_order[0]] = rank;
		if ((int)lms_order.size() > 1) pseudo_sa[lms_order[1]] = ++rank;
		for (int i = 1; i < (int)lms_order.size() - 1; ++i) {
			bool diff = false;
			for (int j = 0; j < n; ++j) {
				int idx0 = lms_order[i + 0] + j;
				int idx1 = lms_order[i + 1] + j;
				if (str[idx0] != str[idx1] || is_lms[idx0] != is_lms[idx1]) {
					diff = true;
					break;
				}
				if (0 != j && is_lms[idx0])break;
			}
			pseudo_sa[lms_order[i + 1]] = diff ? ++rank : rank;
		}

		// LMS部分文字列を一つのcharとしてみなしたn_Strを作る
		std::vector<int> n_str;
		n_str.reserve((int)lms_pos.size());
		int index = 0;
		for (int i = 0; i < n; ++i) if (is_lms[i]) {
			n_str.push_back(pseudo_sa[i]);
		}

		// sa_isを再帰的に行いLMSのSuffixArrayを求める
		std::vector<int>lms_sa;
		if (rank + 1 == lms_pos.size()) {
			// LMS部分文字列に重複がなければ辞書順が正しく求まっている
			lms_sa = lms_order;
		}
		else {
			// 再帰的にSA-IS法を適当
			lms_sa = sa_is(n_str, rank + 1);
			// indexをorgのものに戻す
			for (auto &e : lms_sa)e = lms_pos[e];
		}

		// 再度induced_sortをlms_saに沿って行うとSuffixArrayが求まる
		return induced_sort(str, lms_sa, is_s, kind);
	}

	std::vector<bool> calcS(const std::vector<int> &str) {
		const int n = str.size();
		std::vector<bool>isS(n);
		isS[n - 1] = true;
		for (int i = n - 2; i >= 0; --i) {
			if (str[i] < str[i + 1])isS[i] = true;
			else if (str[i] == str[i + 1] && isS[i + 1])isS[i] = true;
		}
		return isS;
	}

	std::vector<bool> calcLMS(const std::vector<bool> &isS) {
		const int n = isS.size();
		std::vector<bool>isLMS(n);
		for (int i = 0; i < n; ++i) {
			if (isS[i] && (0 == i || !isS[i - 1])) {
				isLMS[i] = true;
			}
		}
		return isLMS;
	}

	std::vector<int> induced_sort(const std::vector<int> &str, const std::vector<int> &lms_pos, const std::vector<bool>&is_s, const int kind) {
		// dump(lms_pos);
		const int n = str.size();
		std::vector<int>buckets(n);

		// 出現する文字に対しての累積和
		std::vector<int>cum(kind + 1);
		for (auto e : str)cum[e + 1]++;
		for (int i = 0; i < kind; ++i)cum[i + 1] += cum[i];

		// 種類ごとの詰めた数
		std::vector<int>count(kind);

		// Step1
		// LMSを後ろから
		for (int i = lms_pos.size() - 1; i >= 0; --i) {
			int c = str[lms_pos[i]];
			int index = cum[c + 1] - count[c] - 1;
			buckets[index] = lms_pos[i];
			count[c]++;
		}
		// dump(buckets);

		// Step2
		// S[i-1]がLなものを前から
		count = std::vector<int>(kind);
		for (int i = 0; i < n; ++i) {
			// 未処理or番兵
			if (0 == buckets[i])continue;
			// notL
			if (is_s[buckets[i] - 1])continue;
			int c = str[buckets[i] - 1];
			int index = cum[c] + count[c];
			buckets[index] = buckets[i] - 1;
			count[c]++;
		}
		// dump(buckets);

		// Step3
		// S[i-1]がSなものを後ろから
		count = std::vector<int>(kind);
		for (int i = n - 1; i >= 0; --i) {
			// 未処理or番兵
			if (0 == buckets[i])continue;
			// notS
			if (!is_s[buckets[i] - 1])continue;
			int c = str[buckets[i] - 1];
			int index = cum[c + 1] - count[c] - 1;
			buckets[index] = buckets[i] - 1;
			count[c]++;
		}
		// dump(buckets);

		return buckets;
	}

	/*void dump(std::vector<int>vec) {
		for (auto e : vec)std::cout << e << " ";
		std::cout << std::endl;
	}*/
};
std::vector<int> suffixArray(const std::string &s) {
	SuffixArray sa = SuffixArray(s);
	return sa();
}
```
# 参考
https://shogo82148.github.io/homepage/memo/algorithm/suffix-array/
https://mametter.hatenablog.com/entry/20180130/p1
https://blog.knshnb.com/posts/sa-is/
# 🧩 modint Tips（AtCoder Library）

## 🔹 任意 mod（動的 modint）

```cpp
#include <atcoder/modint>
using mint = atcoder::modint;

int mod = 123456;

int main() {
    mint::set_mod(mod);

    // 使用例
    mint a = 10, b = 20;
    mint c = a * b;

    // コンビネーション等が必要なら事前構築
    // combination<mint> comb(200005);
}
```

### ⚠️ 注意
- `set_mod()` を最初に必ず呼ぶ
- 実行中に mod を変えない

---

## 🔹 固定 mod（static_modint）

```cpp
#include <atcoder/modint>

using mint = atcoder::modint998244353;
// または
using mint = atcoder::modint1000000007;
```

### ✔ 特徴
- 高速（コンパイル時に mod 確定）
- 一番よく使う

---

## 🔹 複数 mod 同時使用

```cpp
#include <atcoder/modint>

using mint1 = atcoder::modint998244353;
using mint2 = atcoder::modint1000000007;

mint1 a = 10;
mint2 b = 20;
```

### ✔ 用途
- CRT
- ダブルハッシュ
- 誤差対策

---

## 🔹 自分で mod を定義（static）

```cpp
#include <atcoder/modint>

const int mod1 = 1107296257;
using mint1 = atcoder::static_modint<mod1>;

const int mod2 = 1224736769;
using mint2 = atcoder::static_modint<mod2>;
```

---

## 🔹 よく使う操作

```cpp
mint a = 10;

// 加減乗除
a += 5;
a -= 3;
a *= 2;
a /= 4;

// 逆元
mint inv = a.inv();

// 累乗
mint p = a.pow(10);

// 値取得
int x = a.val();
```

---

## 🔹 どれ使うべき？

```
✔ 通常 → modint998244353
✔ mod 指定 → modint1000000007
✔ 任意 mod → modint（set_mod）
✔ 高度（CRTなど） → 複数 mod
```

---

## 🔹 競プロTips

- 迷ったら `998244353`
- NTT使うなら `998244353` 一択
- 任意mod畳み込み → 複数mod + CRT

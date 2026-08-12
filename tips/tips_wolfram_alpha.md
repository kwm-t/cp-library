# 🔍 WolframAlpha Tips（競プロ用）

## 🔹 Σ（総和）

### 基本
```
sum i=0 to n i^2
sum i=1 to n i
sum i=1 to n i^3
```

### 無限和
```
sum n=1 to infinity 1/(sqrt(n+1)+sqrt(n))
```

---

## 🔹 コンビネーション

```
binom(n, k)
binomial(n, k)
```

例：
```
binom(10,3)
```

---

## 🔹 極限

```
limit (expression) as n -> infinity
```

例：
```
limit (n/(n+1)) as n->infinity
```

---

## 🔹 積分

```
integrate x^2 dx
integrate x^2 from 0 to 1
```

---

## 🔹 微分

```
derivative of x^2
d/dx x^2
```

---

## 🔹 方程式

```
solve x^2 + 3x + 2 = 0
```

---

## 🔹 連立方程式

```
solve {x+y=3, x-y=1}
```

---

## 🔹 展開・因数分解

```
expand (x+1)^3
factor x^2+3x+2
```

---

## 🔹 数列

```
sum k=0 to n a^k
```

---

## 🔹 競プロでよく使う

- sum i=1 to n i = n(n+1)/2
- sum i=1 to n i^2 = n(n+1)(2n+1)/6
- telescoping sum を試す
- sqrt 系は有理化してみる

---

## 🔹 Tips

- 英語で書く（日本語NG）
- 曖昧でもだいたい通る
- 分からなければ自然文でもOK

例：
```
sum of squares from 1 to n
```

## expand fps
```
Series[x/(1-x) * x^2/(1-x^2) * 1/(1-x^3), {x, 0, 36}]
```

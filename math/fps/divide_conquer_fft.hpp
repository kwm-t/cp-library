#ifndef KWM_T_MATH_FPS_DIVIDE_CONQUER_FFT_HPP
#define KWM_T_MATH_FPS_DIVIDE_CONQUER_FFT_HPP

#include <vector>
#include <functional>
#include <atcoder/convolution.hpp>

namespace kwm_t::math::fps {

// Π(1 + A_i x) の多項式を分割統治で求める（FFT利用）
// a[l..r) に対して再帰的に計算
// a の要素型は汎用 T1
template <typename T, typename T1>
std::vector<T> divideConquerFFT(const std::vector<T1> &a, int l, int r) {
    if (r == l) return { T(1) };
    if (r - l == 1) return { T(1), T(a[l]) };
    int m = (l + r) / 2;
    return atcoder::convolution(
        divideConquerFFT<T, T1>(a, l, m),
        divideConquerFFT<T, T1>(a, m, r)
    );
}

// O(N log(N)^2) の一般版
// 区間 [l, r) に対して、p(x)/q(x) 形式で分割統治的に計算
// e: lambda で index を渡すと {p, q} を返す
// https://atcoder.jp/contests/fps-24/submissions/71555028
template <typename T>
std::pair<std::vector<T>, std::vector<T>> divideConquerFFT(
    int l, int r,
    const std::function<std::pair<std::vector<T>, std::vector<T>>(int)> &e
) {
    if (r == l) return e(-1);
    if (r - l == 1) return e(l);
    int m = (l + r) / 2;

    auto [pl, ql] = divideConquerFFT<T>(l, m, e);
    auto [pr, qr] = divideConquerFFT<T>(m, r, e);

    auto p0 = atcoder::convolution(pl, qr);
    auto p1 = atcoder::convolution(pr, ql);
    auto q  = atcoder::convolution(ql, qr);

    if (p0.size() < p1.size()) std::swap(p0, p1);
    for (int i = 0; i < p1.size(); ++i) p0[i] += p1[i];

    return { p0, q };
}

} // namespace kwm_t::math::fps

#endif // KWM_T_MATH_FPS_DIVIDE_CONQUER_FFT_HPP
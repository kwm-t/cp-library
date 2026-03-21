#ifndef KWM_T_MATH_FPS_BERLEKAMP_MASSEY_HPP
#define KWM_T_MATH_FPS_BERLEKAMP_MASSEY_HPP

#include <vector>
#include <algorithm>

namespace kwm_t::math::fps {

// 線形漸化式の最小多項式を求める（Berlekamp-Massey法）
// A: 数列の先頭項, k次なら O(k^2)
// a_i = Σ(c_j * a_{i-j}) で表される数列に対して、{1,-c[1],-c[2],...} を返す
// 例: a_i = a[i-1]*3 + a[i-2]*1 -> {1,-3,-1}
template <typename T>
std::vector<T> BerlekampMassey(const std::vector<T> &A) {
    int M = A.size();
    T y = 1; // 直前の係数
    std::vector<T> B, C; // C は現在の多項式、B は直前の多項式
    B.reserve(M + 1);
    C.reserve(M + 1);
    B.push_back(T(1));
    C.push_back(T(1));

    for (int i = 1; i <= M; ++i) {
        int lc = C.size(), lb = B.size();
        T x = 0;
        for (int k = 0; k < lc; ++k) x += C[k] * A[i - lc + k];

        B.push_back(T(0));
        lb++;

        if (x == 0) continue;

        T Multi = x / y;

        if (lc < lb) {
            auto memo = C;
            if (lb > lc) C.insert(C.begin(), lb - lc, T(0));
            for (int k = 0; k < lb; ++k) C[k] -= Multi * B[k];
            B = memo;
            y = x;
        } else {
            for (int k = 0; k < lb; ++k)
                C[lc - 1 - k] -= Multi * B[lb - 1 - k];
        }
    }

    std::reverse(C.begin(), C.end());
    return C;
}

} // namespace kwm_t::math::fps

#endif // KWM_T_MATH_FPS_BERLEKAMP_MASSEY_HPP
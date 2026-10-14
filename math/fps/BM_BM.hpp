/**
 * @file bm_bm.hpp
 * @brief Bostan-Mori 法関連の処理
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/fps/bm_bm.hpp
 */
#ifndef KWM_T_MATH_FPS_BM_PREPROCESS_HPP
#define KWM_T_MATH_FPS_BM_PREPROCESS_HPP

#include <vector>
#include <atcoder/convolution.hpp>
#include "bostan_mori.hpp"
#include "berlekamp_massey.hpp"

namespace kwm_t::math::fps {

// 線形漸化式の初めの d 項から n 項目を求める Bostan-Mori 用前処理
// d 次漸化式なら O(d^2 + d log d)
// a: 数列の先頭項（十分な長さ = 2*d程度）
template <typename T>
Bostan_mori<T> BM_BM(std::vector<T> a) {
    // 1. 最小多項式 q(x) を求める (O(d^2))
    auto q = BerlekampMassey(a);

    // 2. 多項式 p(x) を計算 (O(d log d))
    a.resize(q.size() - 1);
    auto p = atcoder::convolution(a, q);
    p.resize(q.size() - 1);

    // 3. Bostan-Mori 構造体を作成
    Bostan_mori<T> bm(p, q);
    return bm; // O(d log d log N) で任意項を取得可能
}

} // namespace kwm_t::math::fps

#endif // KWM_T_MATH_FPS_BM_PREPROCESS_HPP
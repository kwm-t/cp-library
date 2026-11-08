/**
 * @file random_nonzero.hpp
 * @brief 非ゼロの modint 乱数生成
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/random_nonzero.hpp
 */
#ifndef KWM_T_MATH_MODINT_RANDOM_NONZERO_HPP
#define KWM_T_MATH_MODINT_RANDOM_NONZERO_HPP

namespace kwm_t::math::modint {

/**
 * @brief 非ゼロの modint 乱数を生成する
 *
 * 典型用途:
 *  乱択アルゴリズムで、有限体上の非ゼロ要素をランダムに選ぶ。
 *
 * 計算量:
 *  期待 O(1)
 *
 * @tparam Mint modint 型
 * @param rnd 乱数生成器
 * @return 0 でないランダムな Mint
 *
 * 制約 / 注意:
 *  - Mint は整数から構築できること。
 *  - rnd() は整数値を返すこと。
 *  - 乱数生成器の出力を Mint に変換して使用する。
 *
 * 使用例:
 *  mint x = kwm_t::math::modint::random_nonzero<mint>(rnd);
 *
 * verified:
 *  https://atcoder.jp/contests/abc412/submissions/80705679
 */
template <class Mint, class Random>
Mint random_nonzero(Random& rnd) {
    Mint x = Mint(rnd());
    while (x == Mint(0)) {
        x = Mint(rnd());
    }
    return x;
}

} // namespace kwm_t::math::modint

#endif // KWM_T_MATH_MODINT_RANDOM_NONZERO_HPP
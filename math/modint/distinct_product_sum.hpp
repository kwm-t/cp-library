/**
 * @file distinct_product_sum.hpp
 * @brief 相異なる要素の積の和
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/modint/distinct_product_sum.hpp
 */
#ifndef KWM_T_MATH_MODINT_DISTINCT_PRODUCT_SUM_HPP
#define KWM_T_MATH_MODINT_DISTINCT_PRODUCT_SUM_HPP

#include <array>
#include <vector>
#include <cstddef>

/**
 * @brief 異なる要素を選んだ積の総和（基本対称式）
 *
 * 数列 A に対して、
 *   e_k = Σ_{i1 < i2 < ... < ik} A[i1]A[i2]...A[ik]
 * を求める。
 *
 * 典型用途:
 *   - modint 上での組合せ的な積和計算
 *   - 多項式係数の構築
 *
 * 計算量:
 *   power_sum_k: O(N)
 *   elementary_from_power_sum: O(K^2)
 *
 * 制約 / 注意:
 *   - Mint は四則演算と整数からの構築に対応していること
 *   - 入力型 T は Mint にキャスト可能であること
 *
 * verified:
 *   - https://atcoder.jp/contests/arc217/submissions/75199217
 */
namespace kwm_t::math::modint {

    // =========================================================
    // power sum
    // p_i = Σ A[j]^i
    // =========================================================

    template<std::size_t K, class Mint, class T>
    std::array<Mint, K + 1> power_sum_k(const std::vector<T>& A) {
        std::array<Mint, K + 1> p{};
        p.fill(Mint(0));

        for (const auto& v : A) {
            Mint x = Mint(v);
            Mint cur = x;
            for (std::size_t i = 1; i <= K; i++) {
                p[i] += cur;
                cur *= x;
            }
        }

        return p;
    }

    // =========================================================
    // Newton's identities
    // p -> e
    // =========================================================

    template<std::size_t K, class Mint>
    std::array<Mint, K + 1> elementary_from_power_sum(
        const std::array<Mint, K + 1>& p
    ) {
        std::array<Mint, K + 1> e{};
        e.fill(Mint(0));
        e[0] = Mint(1);

        for (std::size_t k = 1; k <= K; k++) {
            Mint sum = Mint(0);
            for (std::size_t i = 1; i <= k; i++) {
                if (i & 1) sum += e[k - i] * p[i];
                else       sum -= e[k - i] * p[i];
            }
            e[k] = sum / Mint(static_cast<int>(k));
        }

        return e;
    }

    // =========================================================
    // prefix
    // =========================================================

    template<std::size_t K, class Mint, class T>
    std::array<Mint, K + 1> distinct_product_sum_prefix(
        const std::vector<T>& A
    ) {
        return elementary_from_power_sum<K, Mint>(power_sum_k<K, Mint>(A));
    }

    // =========================================================
    // small K optimized
    // =========================================================
    template<class Mint, class T>
    Mint distinct_product_sum_1(const std::vector<T>& A) {
        Mint p1 = 0;

        for (const auto& v : A) {
            Mint x = Mint(v);
            p1 += x;
        }

        return p1;
    }

    template<class Mint, class T>
    Mint distinct_product_sum_2(const std::vector<T>& A) {
        Mint p1 = 0, p2 = 0;

        for (const auto& v : A) {
            Mint x = Mint(v);
            p1 += x;
            p2 += x * x;
        }

        return (p1 * p1 - p2) / Mint(2);
    }

    template<class Mint, class T>
    Mint distinct_product_sum_3(const std::vector<T>& A) {
        Mint p1 = 0, p2 = 0, p3 = 0;

        for (const auto& v : A) {
            Mint x = Mint(v);
            Mint x2 = x * x;

            p1 += x;
            p2 += x2;
            p3 += x2 * x;
        }

        return (p1 * p1 * p1 - Mint(3) * p1 * p2 + Mint(2) * p3) / Mint(6);
    }

    template<class Mint, class T>
    Mint distinct_product_sum_4(const std::vector<T>& A) {
        Mint p1 = 0, p2 = 0, p3 = 0, p4 = 0;

        for (const auto& v : A) {
            Mint x = Mint(v);
            Mint x2 = x * x;
            Mint x3 = x2 * x;

            p1 += x;
            p2 += x2;
            p3 += x3;
            p4 += x3 * x;
        }

        return (
            p1 * p1 * p1 * p1
            - Mint(6) * p1 * p1 * p2
            + Mint(3) * p2 * p2
            + Mint(8) * p1 * p3
            - Mint(6) * p4
        ) / Mint(24);
    }

    template<class Mint, class T>
    Mint distinct_product_sum_5(const std::vector<T>& A) {
        Mint p1 = 0, p2 = 0, p3 = 0, p4 = 0, p5 = 0;

        for (const auto& v : A) {
            Mint x = Mint(v);
            Mint x2 = x * x;
            Mint x3 = x2 * x;
            Mint x4 = x3 * x;

            p1 += x;
            p2 += x2;
            p3 += x3;
            p4 += x4;
            p5 += x4 * x;
        }

        return (
            p1 * p1 * p1 * p1 * p1
            - Mint(10) * p1 * p1 * p1 * p2
            + Mint(15) * p1 * p2 * p2
            + Mint(20) * p1 * p1 * p3
            - Mint(20) * p2 * p3
            - Mint(30) * p1 * p4
            + Mint(24) * p5
        ) / Mint(120);
    }

} // namespace kwm_t::math::modint

#endif // KWM_T_MATH_MODINT_DISTINCT_PRODUCT_SUM_HPP
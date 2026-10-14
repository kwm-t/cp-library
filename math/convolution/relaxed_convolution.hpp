/**
 * @file relaxed_convolution.hpp
 * @brief Relaxed Convolution
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/convolution/relaxed_convolution.hpp
 */
#ifndef KWM_T_MATH_CONVOLUTION_RELAXED_CONVOLUTION_HPP
#define KWM_T_MATH_CONVOLUTION_RELAXED_CONVOLUTION_HPP

#include <vector>
#include <cassert>
#include <atcoder/convolution.hpp>

/**
 * @brief オンライン畳み込み（Relaxed Convolution）
 *
 * 配列 a, b の要素が前から順に追加されるとき、
 * c[k] = Σ_{i=0}^k a[i] * b[k-i] を逐次計算する
 * https://qiita.com/Kiri8128/items/1738d5403764a0e26b4c
 *
 * 典型用途:
 *   - FPS の逐次計算
 *   - DP の高速化
 *
 * 計算量:
 *   全体 O(n log^2 n)
 *
 * @tparam T modint 想定
 *
 * 制約 / 注意:
 *   - atcoder::convolution が使える型
 *
 * 使用例:
 *   RelaxedConvolution<mint> rc(n);
 *   for (...) {
 *       auto val = rc(a_i, b_i);
 *   }
 *
 * verified:
 *   https://atcoder.jp/contests/abc315/submissions/74629993
 */
namespace kwm_t::math::convolution {

    template <typename T>
    class RelaxedConvolution {
    public:
        explicit RelaxedConvolution(int n)
            : a(n), b(n), c(n), k(0), n(n) {}

        /**
         * @brief 次の値を追加して c[k] を返す
         */
        T operator()(T na, T nb) {
            assert(k < n);

            a[k] = na;
            b[k] = nb;

            int s = k + 2;

            for (int w = 1; s % w == 0 && w < s; w <<= 1) {
                for (int t = 0; t < 2; ++t) {
                    if (t == 0 || w * 2 != s) {
                        std::vector<T> f(w), g(w);

                        for (int i = 0; i < w; ++i) {
                            f[i] = a[w - 1 + i];
                            g[i] = b[k - w + 1 + i];
                        }

                        auto h = atcoder::convolution(f, g);

                        for (int i = 0; i < (int)h.size(); ++i) {
                            int j = k + i;
                            if (j < n) c[j] += h[i];
                        }
                    }
                    std::swap(a, b);
                }
            }

            return c[k++];
        }

        /**
         * @brief 現在までの結果を取得
         */
        const std::vector<T>& result() const {
            return c;
        }

    private:
        int n;
        int k;
        std::vector<T> a, b, c;
    };

}

#endif // KWM_T_MATH_CONVOLUTION_RELAXED_CONVOLUTION_HPP
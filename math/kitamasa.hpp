#ifndef KWM_T_MATH_KITAMASA_HPP
#define KWM_T_MATH_KITAMASA_HPP

#include <vector>
#include <cassert>
#include <algorithm>

/**
 * @brief Kitamasa法（線形漸化式の高速計算）
 *
 * k項線形漸化式:
 *
 *   a_n = d[0]*a_{n-k} + d[1]*a_{n-k+1} + ... + d[k-1]*a_{n-1}
 *
 * を満たす数列の a_n を O(k^2 log n) で求める。
 *
 * -------------------------------
 * 🔷 重要: このクラスの設計
 * -------------------------------
 *
 * ・この実装では「係数ベクトル」を直接持つ
 *
 *   a_n = Σ res[i] * a_i   (0 <= i < k)
 *
 * という形で管理し、
 * 「n → 2n」「n → n+1」の遷移を繰り返すことで高速化している。
 *
 * -------------------------------
 * 🔷 d の意味（超重要）
 * -------------------------------
 *
 * d は「漸化式の係数」であり、
 *
 *   a_k = d[0]*a_0 + d[1]*a_1 + ... + d[k-1]*a_{k-1}
 *
 * を意味する。
 *
 * 例: フィボナッチ
 *   a_n = a_{n-1} + a_{n-2}
 *
 * → k = 2
 * → d = {1, 1}
 *
 * -------------------------------
 * 🔷 内部状態 v の意味
 * -------------------------------
 *
 * v[i] は
 *
 *   a_n = Σ v[i] * a_i
 *
 * を表す係数ベクトル
 *
 * 初期状態:
 *   n = 0 → v = {1,0,0,...}
 *
 * -------------------------------
 * 🔷 操作
 * -------------------------------
 *
 * inc(v):
 *   n → n+1 に対応
 *
 * dbl(v):
 *   n → 2n に対応
 *
 * -------------------------------
 * 🔷 計算量
 * -------------------------------
 *
 * 前処理なし
 * クエリ: O(k^2 log n)
 *
 * -------------------------------
 * 🔷 使用例
 * -------------------------------
 *
 * // フィボナッチ
 * // a0=0, a1=1
 * // a_n = a_{n-1} + a_{n-2}
 *
 * Kitamasa<mint> kit({1,1});
 * mint ans = kit.calc(n); // a_n を取得（初項は外で掛ける）
 *
 * ※初期値 a[0..k-1] を掛けるのは利用側
 *
 * -------------------------------
 * 🔷 注意
 * -------------------------------
 *
 * ・戻り値は「係数を d に掛けた値」になっている
 *   → 実際の a_n を得るには初期値と合わせる必要あり
 *
 * ・mint は加減乗算ができる型である必要がある
 *
 * verified:
 *   - https://atcoder.jp/contests/tdpc/submissions/74800517
 */
namespace kwm_t::math {

    template <class T>
    class Kitamasa {
    public:
        /**
         * @param d 漸化式係数（サイズ k）
         */
        explicit Kitamasa(const std::vector<T>& d_)
            : k((int)d_.size()), d(d_) {
            assert(k > 0);
        }

        /**
         * @brief n → n+1 の遷移
         *
         * v を「a_n の係数」→「a_{n+1} の係数」に変換する
         *
         * 計算量: O(k)
         */
        void inc(std::vector<T>& v) const {
            // 末尾を前に持ってくる（シフト）
            std::rotate(v.begin(), v.begin() + (k - 1), v.end());

            T last = v[0];
            v[0] = T(0);

            // 漸化式を適用
            for (int i = 0; i < k; ++i) {
                v[i] += d[i] * last;
            }
        }

        /**
         * @brief n → 2n の遷移
         *
         * v を「a_n の係数」→「a_{2n} の係数」に変換する
         *
         * 計算量: O(k^2)
         */
        void dbl(std::vector<T>& v) const {
            auto base = v;
            std::vector<T> res(k, T(0));

            for (int i = 0; i < k; ++i) {
                for (int j = 0; j < k; ++j) {
                    res[j] += base[j] * v[i];
                }
                inc(base);
            }

            v = std::move(res);
        }

        /**
         * @brief a_n を求める（係数 d に対する結果）
         *
         * ⚠ 注意:
         *   これは「d に対する線形結合の結果」であり、
         *   初期値 a[0..k-1] を掛ける処理は含まない
         *
         * @param n 求めたい index
         */
        T calc(long long n) const {
            assert(n >= 0);

            // a_0 の係数
            std::vector<T> v(k, T(0));
            v[0] = T(1);

            // MSB から処理
            for (int i = 63; i >= 0; --i) {
                dbl(v);
                if ((n >> i) & 1LL) {
                    inc(v);
                }
            }

            // 最後に d と合成
            T res = T(0);
            for (int i = 0; i < k; ++i) {
                res += v[i] * d[i];
            }
            return res;
        }

    private:
        int k;
        std::vector<T> d;
    };

} // namespace kwm_t::math

#endif // KWM_T_MATH_KITAMASA_HPP
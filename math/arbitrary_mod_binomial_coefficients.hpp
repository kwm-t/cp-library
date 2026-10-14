/**
 * @file arbitrary_mod_binomial_coefficients.hpp
 * @brief 任意の法における二項係数
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/arbitrary_mod_binomial_coefficients.hpp
 */
#ifndef KWM_T_MATH_ARBITRARY_MOD_BINOMIAL_COEFFICIENTS_HPP
#define KWM_T_MATH_ARBITRARY_MOD_BINOMIAL_COEFFICIENTS_HPP

#include <vector>
#include <algorithm>

#include "linear_sieve.hpp"

namespace kwm_t::math {

/**
 * @brief 任意modにおける二項係数列 nCk (k=0..n)
 *
 * mod が素数でない場合にも対応。
 * 全ての k に対して nCk mod m を O(n) で計算する。
 *
 * 典型用途:
 *   - mod が合成数のときの組み合わせ計算
 *   - n 固定で全列挙したい場合
 *
 * 計算量:
 *   - 構築: O(n)
 *
 * 制約 / 注意:
 *   - mint は mod を静的に持ち inv() が定義されている必要あり
 *   - mod に含まれる素因数に依存した処理を行う
 *
 * 使用例:
 *   ArbitraryModBinomialCoefficients<mint> bc(n);
 *   auto val = bc[k];
 *
 * verified:
 *   - competitive programming
 */
template <typename mint>
class ArbitraryModBinomialCoefficients {
public:
    explicit ArbitraryModBinomialCoefficients(unsigned int n)
        : n_(n),
          mod_(mint::mod()),
          sieve_(n),
          binom_(n + 1) {
        build();
    }

    /**
     * @brief nCk を取得
     */
    mint operator[](unsigned int k) const {
        if (k > n_) return mint(0);
        return binom_[k];
    }

    /**
     * @brief 全係数を取得
     */
    const std::vector<mint>& coeffs() const {
        return binom_;
    }

    /**
     * @brief 内部で使用した線形篩
     */
    const LinearSieve& sieve() const {
        return sieve_;
    }

private:
    unsigned int n_;
    unsigned int mod_;
    LinearSieve sieve_;
    std::vector<mint> binom_;

    void build_inverses(std::vector<mint>& invs) const {
        const auto& mpf = sieve_.min_prime_factor();

        if (n_ >= 1) invs[1] = mint(1);

        for (unsigned int i = 2; i <= n_; ++i) {
            unsigned int pf = mpf[i];
            if (pf == i) {
                if (mod_ % pf != 0) {
                    invs[i] = mint(i).inv();
                }
            } else {
                invs[i] = invs[pf] * invs[i / pf];
            }
        }
    }

    void build() {
        const auto& primes = sieve_.primes();

        std::vector<unsigned int> d(n_ + 1, 0);
        std::vector<unsigned int> p;

        // mod の素因数のみ抽出
        for (unsigned int prime : primes) {
            if (mod_ % prime != 0) continue;
            p.push_back(prime);

            unsigned int id = p.size();
            for (unsigned int v = prime; v <= n_; v += prime) {
                d[v] = id;
            }
        }

        unsigned int L = p.size();
        p.insert(p.begin(), 0); // 1-index

        std::vector<mint> invs(n_ + 1);
        build_inverses(invs);

        // p^k を前計算
        std::vector<std::vector<mint>> powers(L + 1);
        for (unsigned int i = 1; i <= L; ++i) {
            unsigned int max_index = n_ / (p[i] - 1);
            powers[i].resize(max_index + 1);

            mint pi = mint(p[i]);
            powers[i][0] = mint(1);

            for (unsigned int j = 0; j < max_index; ++j) {
                powers[i][j + 1] = powers[i][j] * pi;
            }
        }

        unsigned int half = (n_ + 1) / 2;

        mint S = mint(1);
        std::vector<unsigned int> T(L + 1, 0);

        binom_[0] = mint(1);

        for (unsigned int k = 1; k <= half; ++k) {
            unsigned int num = n_ - k + 1;
            unsigned int den = k;

            while (d[num]) {
                ++T[d[num]];
                num /= p[d[num]];
            }
            while (d[den]) {
                --T[d[den]];
                den /= p[d[den]];
            }

            S *= mint(num) * invs[den];
            binom_[k] = S;

            for (unsigned int i = 1; i <= L; ++i) {
                binom_[k] *= powers[i][T[i]];
            }
        }

        // 対称性
        for (unsigned int k = half + 1; k <= n_; ++k) {
            binom_[k] = binom_[n_ - k];
        }
    }
};

} // namespace kwm_t::math

#endif // KWM_T_MATH_ARBITRARY_MOD_BINOMIAL_COEFFICIENTS_HPP
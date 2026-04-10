#ifndef KWM_T_MATH_OSA_K_HPP
#define KWM_T_MATH_OSA_K_HPP

#include <vector>
#include <utility>
#include <cassert>

/**
 * @brief 最小素因数テーブル（Osa-K 法）
 *
 * 各整数 n に対して「最小の素因数 (spf: smallest prime factor)」を前計算することで、
 * 高速に素因数分解を行う。
 *
 * 典型用途:
 *   - 多数クエリの素因数分解
 *   - 約数列挙の前処理
 *   - 素数判定の補助
 *
 * 計算量:
 *   前処理: O(N log log N)
 *   分解:   O(log N)
 *
 * @param max_n 最大値（この範囲まで前計算）
 *
 * 制約 / 注意:
 *   - build() を呼び忘れないこと
 *   - n <= max_n のみ対応
 *
 * 使用例:
 *   OsaK osa(1'000'000);
 *   osa.build();
 *   auto f = osa.factorize(84); // {2,2,3,7}
 *
 * verified:
 *   - 競プロ用途
 */
namespace kwm_t::math {

    class OsaK {
    public:
        explicit OsaK(int max_n = 1'000'000)
            : max_n_(max_n), spf_(max_n + 1, 0), built_(false) {}

        /**
         * @brief 前処理
         */
        void build() {
            spf_[0] = 0;
            if (max_n_ >= 1) spf_[1] = 1;

            for (int i = 2; i <= max_n_; ++i) {
                if (spf_[i] != 0) continue;
                spf_[i] = i;
                if ((long long)i * i > max_n_) continue;
                for (long long j = (long long)i * i; j <= max_n_; j += i) {
                    if (spf_[j] == 0) spf_[j] = i;
                }
            }

            built_ = true;
        }

        /**
         * @brief 素因数分解（重複あり）
         * @return {p1, p2, ...}
         */
        std::vector<int> factorize(int n) const {
            assert(built_ && "Call build() before using factorize()");
            assert(1 <= n && n <= max_n_);

            std::vector<int> res;
            while (n > 1) {
                res.push_back(spf_[n]);
                n /= spf_[n];
            }
            return res;
        }

        /**
         * @brief 素因数分解（(素数, 個数)）
         * @return {(p, cnt), ...}
         */
        std::vector<std::pair<int,int>> factorize_pair(int n) const {
            auto v = factorize(n);
            std::vector<std::pair<int,int>> res;

            for (int i = 0; i < (int)v.size();) {
                int j = i + 1;
                while (j < (int)v.size() && v[i] == v[j]) ++j;
                res.emplace_back(v[i], j - i);
                i = j;
            }
            return res;
        }

        /**
         * @brief 素数判定
         */
        bool is_prime(int n) const {
            assert(built_);
            assert(1 <= n && n <= max_n_);
            return n >= 2 && spf_[n] == n;
        }

        /**
         * @brief 最小素因数取得
         */
        int spf(int n) const {
            assert(built_);
            assert(1 <= n && n <= max_n_);
            return spf_[n];
        }

    private:
        int max_n_;
        std::vector<int> spf_;
        bool built_;
    };

} // namespace kwm_t::math

#endif // KWM_T_MATH_OSA_K_HPP
#ifndef KWM_T_MATH_MILLER_RABIN_HPP
#define KWM_T_MATH_MILLER_RABIN_HPP

#include <vector>
#include "base/integer.hpp"

namespace kwm_t::math {

/**
 * @brief Miller-Rabin 素数判定（64bit対応・確定的）
 *
 * 計算量:
 *   O(log n)
 *
 * 制約:
 *   - n < 2^64
 *
 * verified:
 *   https://judge.yosupo.jp/submission/367299
 */
struct MillerRabin {
private:
    static int128 modmul(int128 a, int128 b, int128 mod) {
        return (a * b) % mod;
    }

    static long long modpow(long long a, long long e, long long mod) {
        int128 res = 1;
        int128 base = a % mod;

        while (e) {
            if (e & 1) res = modmul(res, base, mod);
            base = modmul(base, base, mod);
            e >>= 1;
        }
        return (long long)res;
    }

public:
    static bool is_prime(long long n) {
        if (n < 2) return false;

        // 小さい素数で弾く
        for (long long p : {2, 3, 5, 7, 11, 13, 17, 19, 23}) {
            if (n == p) return true;
            if (n % p == 0 && n != p) return false;
        }

        long long d = n - 1;
        int s = 0;
        while ((d & 1) == 0) {
            d >>= 1;
            ++s;
        }

        // 64bitで確定的な基底
        static const long long bases[] = {
            2, 325, 9375, 28178, 450775, 9780504, 1795265022
        };

        for (long long a : bases) {
            if (a % n == 0) continue;

            long long x = modpow(a, d, n);
            if (x == 1 || x == n - 1) continue;

            bool composite = true;
            for (int r = 1; r < s; ++r) {
                x = modmul(x, x, n);
                if (x == n - 1) {
                    composite = false;
                    break;
                }
            }

            if (composite) return false;
        }

        return true;
    }
};

} // namespace kwm_t::math

#endif // KWM_T_MATH_MILLER_RABIN_HPP
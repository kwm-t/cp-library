/**
 * @file random.hpp
 * @brief 乱数生成の補助機能
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/utility/random.hpp
 */
#ifndef KWM_T_UTILITY_RANDOM_HPP
#define KWM_T_UTILITY_RANDOM_HPP

#include <random>
#include <cmath>
#include <cassert>
#include <limits>

namespace kwm_t::utility {

/**
 * @brief 乱数ユーティリティ（mt19937 ラッパ）
 *
 * 典型用途:
 *   - 競プロの乱択
 *   - 焼きなまし
 *
 * 注意:
 *   - next_log() は log(0) を避けるため微小値でクリップ
 */
class Random {
public:
    explicit Random(uint32_t seed = 0) : mt_(seed), dist01_(0.0, 1.0) {}

    /**
     * @brief [0, m) の整数
     */
    int next_int(int m) {
        assert(m > 0);
        std::uniform_int_distribution<int> dist(0, m - 1);
        return dist(mt_);
    }

    /**
     * @brief [l, r) の整数
     */
    int next_int(int l, int r) {
        assert(l < r);
        std::uniform_int_distribution<int> dist(l, r - 1);
        return dist(mt_);
    }

    /**
     * @brief [0, 1) の実数
     */
    double next_double() {
        return dist01_(mt_);
    }

    /**
     * @brief log(U)（U ~ Uniform(0,1)）
     *
     * 焼きなましなどで使う（指数分布的）
     */
    double next_log() {
        double u = dist01_(mt_);
        if (u <= 0.0) u = std::numeric_limits<double>::min();
        return std::log(u);
    }

private:
    std::mt19937 mt_;
    std::uniform_real_distribution<double> dist01_;
};

} // namespace kwm_t::utility

#endif // KWM_T_UTILITY_RANDOM_HPP
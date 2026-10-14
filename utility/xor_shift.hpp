/**
 * @file xor_shift.hpp
 * @brief Xorshift 乱数生成器
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/utility/xor_shift.hpp
 */
#ifndef KWM_T_UTILITY_XORSHIFT_HPP
#define KWM_T_UTILITY_XORSHIFT_HPP

#include <cstdint>
#include <limits>

namespace kwm_t::utility {

/**
 * @brief Xorshift 乱数生成器（軽量・高速）
 *
 * 典型用途:
 *   - 競プロの乱択
 *   - テストケース生成
 *
 * 計算量:
 *   O(1)
 *
 * 注意:
 *   - 暗号用途には不適
 *   - seed=0 は避ける
 *
 * verified:
 *   - 競技プログラミング用途
 */
class Xorshift {
public:
    using result_type = uint32_t;

    explicit Xorshift(uint64_t seed = 1) : x(seed ? seed : 1) {}

    static constexpr result_type min() {
        return std::numeric_limits<result_type>::min();
    }

    static constexpr result_type max() {
        return std::numeric_limits<result_type>::max();
    }

    /**
     * @brief [0, 2^32) の乱数
     */
    result_type operator()() {
        x ^= x << 9;
        x ^= x >> 7;
        return static_cast<result_type>(x);
    }

    /**
     * @brief [0, m) の整数乱数
     */
    int64_t operator()(int64_t m) {
        return (static_cast<uint64_t>((*this)()) * m) >> 32;
    }

    /**
     * @brief [0, 1) の実数乱数
     */
    double uniform() {
        return static_cast<double>((*this)()) / (static_cast<uint64_t>(1) << 32);
    }

private:
    uint64_t x;
};

} // namespace kwm_t::utility

#endif // KWM_T_UTILITY_XORSHIFT_HPP
#ifndef KWM_T_UTILITY_TIME_KEEPER_HPP
#define KWM_T_UTILITY_TIME_KEEPER_HPP

#include <chrono>

namespace kwm_t::utility {

/**
 * @brief 時間計測クラス（ミリ秒単位）
 *
 * 典型用途:
 *   - 焼きなまし
 *   - 制限時間付きループ
 *
 * 計算量:
 *   O(1)
 *
 * 注意:
 *   - 時間単位はミリ秒（ms）
 *
 * verified:
 *   - 競プロヒューリスティック
 */
class TimeKeeper {
public:
    explicit TimeKeeper(double time_limit_ms)
        : start_time_(Clock::now()),
          time_limit_(time_limit_ms) {}

    /**
     * @brief 経過時間（ms）
     */
    double elapsed() const {
        auto now = Clock::now();
        auto diff = now - start_time_;
        return std::chrono::duration_cast<std::chrono::microseconds>(diff).count() * 1e-3;
    }

    /**
     * @brief 時間制限を超えたか
     */
    bool is_over() const {
        return elapsed() >= time_limit_;
    }

private:
    using Clock = std::chrono::steady_clock;

    Clock::time_point start_time_;
    double time_limit_;
};

} // namespace kwm_t::utility

#endif // KWM_T_UTILITY_TIME_KEEPER_HPP
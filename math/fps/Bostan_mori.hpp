/**
 * @file bostan_mori.hpp
 * @brief Bostan-Mori 法
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/fps/bostan_mori.hpp
 */
#ifndef KWM_T_MATH_FPS_BOSTAN_MORI_HPP
#define KWM_T_MATH_FPS_BOSTAN_MORI_HPP

#include <vector>
#include <atcoder/convolution.hpp> // ACL の畳み込み

namespace kwm_t::math::fps {

// 二項生成関数の n 項目を高速に求める Bostan-Mori 法
template <class T>
struct Bostan_mori {
    std::vector<T> p, q;

    Bostan_mori(std::vector<T> &_p, std::vector<T> &_q) : p(_p), q(_q) {}

private:
    // f(x) -> f(-x) の符号反転（奇数項だけ反転）
    void rev(std::vector<T> &f) const {
        int d = f.size();
        for (int i = 0; i < d; ++i) if (i & 1) f[i] = -f[i];
    }

    // 偶数項抽出
    void even(std::vector<T> &f) const {
        int d = (f.size() + 1) >> 1;
        for (int i = 0; i < d; ++i) f[i] = f[i << 1];
        f.resize(d);
    }

    // 奇数項抽出
    void odd(std::vector<T> &f) const {
        int d = f.size() >> 1;
        for (int i = 0; i < d; ++i) f[i] = f[i << 1 | 1];
        f.resize(d);
    }

public:
    // n 項目を返す
    T operator[](long long n) const {
        std::vector<T> _p(p), _q(q), _q_rev(q);
        rev(_q_rev);

        for (; n; n >>= 1) {
            _p = atcoder::convolution(std::move(_p), _q_rev);
            if (n & 1) odd(_p);
            else       even(_p);

            _q = atcoder::convolution(std::move(_q), std::move(_q_rev));
            even(_q);
            _q_rev = _q;
            rev(_q_rev);
        }

        return _p[0] / _q[0];
    }
};

} // namespace kwm_t::math::fps

#endif // KWM_T_MATH_FPS_BOSTAN_MORI_HPP
/**
 * @file min_plus_convolution_convex.hpp
 * @brief 凸性を利用した min-plus convolution
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/algorithm/min_plus_convolution_convex.hpp
 */
#ifndef KWM_T_ALGORITHM_MIN_PLUS_CONVOLUTION_CONVEX_HPP
#define KWM_T_ALGORITHM_MIN_PLUS_CONVOLUTION_CONVEX_HPP

#include <algorithm>
#include <vector>

namespace kwm_t::algorithm {

/**
 * @brief Convex Min-Plus Convolution
 *
 * 2つの凸列 a,b に対し
 *
 *   c[k] = min_{i+j=k}(a[i]+b[j])
 *
 * を O((N+M) log(N+M)) で計算する。
 *
 * 凸列とは
 *
 *   a[i+2]-a[i+1] >= a[i+1]-a[i]
 *
 * を満たす列である。
 *
 * 計算量:
 *   O((N+M) log(N+M))
 *
 * @tparam T
 *   加算・比較可能な数値型
 *
 * @param a
 *   凸列
 *
 * @param b
 *   凸列
 *
 * @return
 *   min-plus convolution
 *
 * 制約:
 *   a,b は空でないこと。
 *
 * 使用例:
 * @code
 * std::vector<long long> c =
 *     kwm_t::algorithm::min_plus_convolution_convex(a, b);
 * @endcode
 *
 * verified:
 *   https://judge.yosupo.jp/submission/382683
 */
template <class T>
std::vector<T> min_plus_convolution_convex(
    const std::vector<T>& a,
    const std::vector<T>& b) {

    std::vector<T> diff;
    diff.reserve(a.size() + b.size() - 2);

    for (std::size_t i = 1; i < a.size(); ++i) {
        diff.push_back(a[i] - a[i - 1]);
    }

    for (std::size_t i = 1; i < b.size(); ++i) {
        diff.push_back(b[i] - b[i - 1]);
    }

    std::sort(diff.begin(), diff.end());

    std::vector<T> res;
    res.reserve(a.size() + b.size() - 1);

    res.push_back(a.front() + b.front());

    for (const T& d : diff) {
        res.push_back(res.back() + d);
    }

    return res;
}

} // namespace kwm_t::algorithm

#endif
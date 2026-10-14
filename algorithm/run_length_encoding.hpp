/**
 * @file run_length_encoding.hpp
 * @brief ランレングス圧縮
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/algorithm/run_length_encoding.hpp
 */
#ifndef KWM_T_ALGORITHM_RUN_LENGTH_ENCODING_HPP
#define KWM_T_ALGORITHM_RUN_LENGTH_ENCODING_HPP

#include <vector>
#include <utility>
#include <iterator>

namespace kwm_t::algorithm {
/**
 * @brief Run Length Encoding（連長圧縮）
 *
 * 任意の forward iterator に対して適用可能
 *
 * 計算量:
 *   O(N)
 *
 * 制約:
 *   - 要素型に == が定義されていること
 * verified
 *  https://atcoder.jp/contests/abc452/submissions/74695137
 *  https://atcoder.jp/contests/awc0051/submissions/75170743
 */
template<class It>
std::vector<std::pair<typename std::iterator_traits<It>::value_type, int>>
run_length_encoding(It first, It last) {
    using T = typename std::iterator_traits<It>::value_type;
    std::vector<std::pair<T, int>> ret;

    if (first == last) return ret;

    It it = first;
    while (it != last) {
        It jt = it;
        int cnt = 0;
        while (jt != last && *jt == *it) {
            ++jt;
            ++cnt;
        }
        ret.emplace_back(*it, cnt);
        it = jt;
    }
    return ret;
}

// コンテナ版
template<class Container>
std::vector<std::pair<typename Container::value_type, int>>
run_length_encoding(const Container& c) {
    return run_length_encoding(c.begin(), c.end());
}

} // namespace kwm_t::algorithm

#endif // KWM_T_ALGORITHM_RUN_LENGTH_ENCODING_HPP
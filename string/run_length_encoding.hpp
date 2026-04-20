#ifndef KWM_T_STRING_RUN_LENGTH_ENCODING_HPP
#define KWM_T_STRING_RUN_LENGTH_ENCODING_HPP

#include <vector>
#include <utility>

/**
 * @brief ランレングス圧縮 (Run Length Encoding)
 *
 * 連続する同一要素を (値, 個数) に圧縮する
 *
 * 典型用途:
 *   - 文字列圧縮
 *   - 連続区間の処理
 *
 * 計算量:
 *   O(N)
 *
 * @tparam T
 *   - operator== が定義されている型
 *
 * @param s 入力列
 *
 * @return vector<pair<T, int>>
 *
 * 使用例:
 *   auto v = kwm_t::string::run_length_encoding("aaabb");
 *   // {{'a',3},{'b',2}}
 *
 * verified:
 *   - https://atcoder.jp/contests/awc0051/submissions/75131286
 */
namespace kwm_t::string {

template <typename T>
std::vector<std::pair<T, int>> run_length_encoding(const std::vector<T>& s) {
    std::vector<std::pair<T, int>> res;
    const int n = (int)s.size();

    for (int i = 0; i < n;) {
        int j = i + 1;
        while (j < n && s[j] == s[i]) ++j;
        res.emplace_back(s[i], j - i);
        i = j;
    }

    return res;
}

// string用のラッパー
inline std::vector<std::pair<char, int>> run_length_encoding(const std::string& s) {
    std::vector<char> v(s.begin(), s.end());
    return run_length_encoding(v);
}

} // namespace kwm_t::string

#endif // KWM_T_STRING_RUN_LENGTH_ENCODING_HPP
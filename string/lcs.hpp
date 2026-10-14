/**
 * @file lcs.hpp
 * @brief 最長共通部分列（LCS）
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/string/lcs.hpp
 */
#ifndef KWM_T_STRING_LCS_HPP
#define KWM_T_STRING_LCS_HPP

#include <vector>
#include <string>
#include <algorithm>

/**
 * @brief 最長共通部分列 (Longest Common Subsequence)
 *
 * 2つの文字列のLCSを求め、その文字列を復元する
 *
 * 典型用途:
 *   - 文字列比較
 *   - 差分検出
 *
 * 計算量:
 *   O(|s| * |t|)
 *
 * @param s, t 対象文字列
 *
 * 制約 / 注意:
 *   - 長さが大きいと O(N^2) で重い
 *
 * 使用例:
 *   auto res = kwm_t::string::lcs("abcde", "ace");
 *   // res = "ace"
 *
 * verified:
 *   https://atcoder.jp/contests/dp/submissions/74809521
 */
namespace kwm_t::string {

inline std::string lcs(const std::string& s, const std::string& t) {
    const int n = (int)s.size();
    const int m = (int)t.size();

    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

    // DP構築
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (s[i] == t[j]) {
                dp[i + 1][j + 1] = dp[i][j] + 1;
            } else {
                dp[i + 1][j + 1] = std::max(dp[i + 1][j], dp[i][j + 1]);
            }
        }
    }

    // 復元
    std::string res;
    int i = n, j = m;

    while (i > 0 && j > 0) {
        if (s[i - 1] == t[j - 1]) {
            res += s[i - 1];
            --i;
            --j;
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }

    std::reverse(res.begin(), res.end());
    return res;
}

} // namespace kwm_t::string

#endif // KWM_T_STRING_LCS_HPP
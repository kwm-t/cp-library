#ifndef KWM_T_MAX_MATCHING_HPP
#define KWM_T_MAX_MATCHING_HPP

#include <vector>

/**
 * @brief 木の最大マッチング
 *
 * 典型用途:
 *   - 木における最大マッチング
 *   - 完全マッチングの判定
 *
 * 計算量:
 *   - O(N)
 *
 * @param g 木の隣接リスト
 *
 * 戻り値:
 *   - match[v] = マッチ相手 or -1
 *
 * 使用例:
 *   auto match = tree_matching(g);
 *   int sz = matching_size(match);
 *   bool perfect = (sz * 2 == n);
 *
 * verified:
 *   - 典型問題（木DP）
 */
namespace kwm_t::tree {

/**
 * @brief 最大マッチング構築
 */
std::vector<int> tree_max_matching(const std::vector<std::vector<int>>& g) {
    int n = g.size();
    std::vector<int> match(n, -1);

    auto dfs = [&](auto&& self, int v, int p) -> void {
        for (int nv : g[v]) {
            if (nv == p) continue;
            self(self, nv, v);
        }
        for (int nv : g[v]) {
            if (nv == p) continue;
            if (match[v] == -1 && match[nv] == -1) {
                match[v] = nv;
                match[nv] = v;
            }
        }
    };

    dfs(dfs, 0, -1);
    return match;
}

/**
 * @brief マッチングサイズ
 */
int matching_size(const std::vector<int>& match) {
    int n = match.size();
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        if (match[i] != -1 && i < match[i]) cnt++;
    }
    return cnt;
}

/**
 * @brief 完全マッチングか判定
 */
bool is_perfect_matching(const std::vector<int>& match) {
    int n = match.size();
    return matching_size(match) * 2 == n;
}

} // namespace kwm_t::tree

#endif // KWM_T_MAX_MATCHING_HPP
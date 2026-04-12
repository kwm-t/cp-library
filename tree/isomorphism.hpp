#ifndef KWM_T_TREE_ISOMORPHISM_HPP
#define KWM_T_TREE_ISOMORPHISM_HPP

#include <vector>
#include <algorithm>
#include <random>
#include <cstdint>
#include "center.hpp"

namespace kwm_t::tree {

class TreeHash {
public:
    using ull = std::uint64_t;

    TreeHash(const std::vector<std::vector<int>>& g, ull seed = 0)
        : g(g), n(g.size()), rng(seed) {
        rand.resize(n);
        for (int i = 0; i < n; i++) {
            rand[i] = rng();
        }
    }

    /**
     * @brief 根付き木ハッシュ
     */
    ull rooted_hash(int root) {
        return dfs(root, -1).first;
    }

    /**
     * @brief 根なし木ハッシュ（中心考慮）
     */
    ull hash() {
        auto centers = tree_centers(g);

        if (centers.size() == 1) {
            return rooted_hash(centers[0]);
        } else {
            ull h1 = rooted_hash(centers[0]);
            ull h2 = rooted_hash(centers[1]);
            if (h1 > h2) std::swap(h1, h2);
            return combine(h1, h2);
        }
    }

private:
    int n;
    const std::vector<std::vector<int>>& g;
    std::vector<ull> rand;
    std::mt19937_64 rng;

    std::pair<ull, int> dfs(int v, int p) {
        std::vector<ull> child;
        int depth = 0;

        for (int nv : g[v]) {
            if (nv == p) continue;
            auto [h, d] = dfs(nv, v);
            child.push_back(h);
            depth = std::max(depth, d + 1);
        }

        std::sort(child.begin(), child.end());

        ull h = 1;
        for (auto c : child) {
            h = mix(h, c + rand[depth]);
        }

        return { h, depth };
    }

    static ull mix(ull a, ull b) {
        a ^= b + 0x9e3779b97f4a7c15ULL + (a << 6) + (a >> 2);
        return a;
    }

    static ull combine(ull a, ull b) {
        return mix(a, b);
    }
};

/**
 * @brief 根付き木ハッシュ
 */
inline std::uint64_t rooted_tree_hash(
    const std::vector<std::vector<int>>& g,
    int root,
    std::uint64_t seed = 0) {
    return TreeHash(g, seed).rooted_hash(root);
}

/**
 * @brief 根なし木ハッシュ
 */
inline std::uint64_t tree_hash(
    const std::vector<std::vector<int>>& g,
    std::uint64_t seed = 0) {
    return TreeHash(g, seed).hash();
}

/**
 * @brief 根付き木同型判定
 */
inline bool is_isomorphic_rooted(
    const std::vector<std::vector<int>>& g1, int r1,
    const std::vector<std::vector<int>>& g2, int r2,
    std::uint64_t seed = 0) {
    if (g1.size() != g2.size()) return false;
    return rooted_tree_hash(g1, r1, seed) ==
           rooted_tree_hash(g2, r2, seed);
}

/**
 * @brief 根なし木同型判定
 */
inline bool is_isomorphic(
    const std::vector<std::vector<int>>& g1,
    const std::vector<std::vector<int>>& g2,
    std::uint64_t seed = 0) {
    if (g1.size() != g2.size()) return false;
    return tree_hash(g1, seed) == tree_hash(g2, seed);
}

} // namespace kwm_t::tree

#endif // KWM_T_TREE_ISOMORPHISM_HPP
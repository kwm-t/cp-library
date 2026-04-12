#ifndef KWM_T_DATA_STRUCTURE_DISJOINT_SET_UNION_UNION_FIND_UNDO_HPP
#define KWM_T_DATA_STRUCTURE_DISJOINT_SET_UNION_UNION_FIND_UNDO_HPP

#include <vector>

/**
 * @brief Undo可能 Union-Find（Rollback DSU）
 *
 * 典型用途:
 *   - オフライン動的連結性
 *   - 分割統治 + Union-Find
 *
 * 計算量:
 *   - merge / undo: O(1)
 *   - leader: O(log N)（経路圧縮なし）
 *
 * 制約 / 注意:
 *   - 経路圧縮は使えない（undoのため）
 * verify
 *   - https://atcoder.jp/contests/joi2022yo2/submissions/74912321
 */
namespace kwm_t::data_structure::disjoint_set_union {

class UnionFindUndo {
public:
    explicit UnionFindUndo(int n)
        : n(n), par_or_size(n, -1) {}

    int leader(int v) const {
        while (par_or_size[v] >= 0) {
            v = par_or_size[v];
        }
        return v;
    }

    bool same(int u, int v) const {
        return leader(u) == leader(v);
    }

    bool merge(int u, int v) {
        u = leader(u);
        v = leader(v);

        history.emplace_back(u, par_or_size[u]);
        history.emplace_back(v, par_or_size[v]);

        if (u == v) return false;

        if (par_or_size[u] > par_or_size[v]) std::swap(u, v);

        par_or_size[u] += par_or_size[v];
        par_or_size[v] = u;
        return true;
    }

    int size(int v) const {
        return -par_or_size[leader(v)];
    }

    /**
     * @brief 直前のmergeを取り消す
     */
    void undo() {
        for (int i = 0; i < 2; ++i) {
            auto [v, val] = history.back();
            history.pop_back();
            par_or_size[v] = val;
        }
    }

    /**
     * @brief 現在状態のスナップショットを取得
     */
    int snapshot() const {
        return history.size();
    }

    /**
     * @brief 指定スナップショットまで巻き戻し
     */
    void rollback(int snap) {
        while ((int)history.size() > snap) {
            undo();
        }
    }

private:
    int n;
    std::vector<int> par_or_size;
    std::vector<std::pair<int, int>> history;
};

} // namespace kwm_t::data_structure::disjoint_set_union

#endif // KWM_T_DATA_STRUCTURE_DISJOINT_SET_UNION_UNION_FIND_UNDO_HPP
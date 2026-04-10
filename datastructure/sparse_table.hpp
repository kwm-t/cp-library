#ifndef KWM_T_DATA_STRUCTURE_SPARSE_TABLE_HPP
#define KWM_T_DATA_STRUCTURE_SPARSE_TABLE_HPP

#include <vector>
#include <algorithm>
#include <cstddef>

namespace kwm_t::data_structure {

/**
 * @brief Sparse Table (idempotent semigroup query structure)
 *
 * 典型用途:
 *   - RMQ (min/max)
 *   - gcd
 *   - bitwise and/or
 *
 * 計算量:
 *   build: O(N log N)
 *   query: O(1)
 *
 * 制約:
 *   - op は冪等・結合則を満たす必要あり
 *
 * 区間:
 *   [l, r) 半開区間
 * verified
 *  https://judge.yosupo.jp/submission/365493
 */
template <class T, T(*op)(T, T)>
class SparseTable {
private:
    int n = 0;
    int LOG = 0;
    std::vector<std::vector<T>> table;
    std::vector<int> log_table;

    void build(const std::vector<T>& v) {
        n = (int)v.size();
        LOG = (n > 1) ? (32 - __builtin_clz(n)) : 1;

        table.assign(LOG, std::vector<T>(n));
        log_table.assign(n + 1, 0);

        for (int i = 0; i < n; i++) {
            table[0][i] = v[i];
        }

        for (int k = 1; k < LOG; k++) {
            for (int i = 0; i + (1 << k) <= n; i++) {
                table[k][i] = op(table[k - 1][i],
                                 table[k - 1][i + (1 << (k - 1))]);
            }
        }

        for (int i = 2; i <= n; i++) {
            log_table[i] = log_table[i >> 1] + 1;
        }
    }
public:
    SparseTable() = default;

    explicit SparseTable(const std::vector<T>& v) {
        build(v);
    }

    // query on [l, r)
    T prod(int l, int r) const {
        int len = r - l;
        int k = log_table[len];
        return op(table[k][l], table[k][r - (1 << k)]);
    }
};

} // namespace kwm_t::data_structure

#endif // KWM_T_DATA_STRUCTURE_SPARSE_TABLE_HPP
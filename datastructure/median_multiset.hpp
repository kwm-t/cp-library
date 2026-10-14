/**
 * @file median_multiset.hpp
 * @brief 中央値を管理する multiset
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/datastructure/median_multiset.hpp
 */
#ifndef KWM_T_DATA_STRUCTURE_MEDIAN_MULTISET_HPP
#define KWM_T_DATA_STRUCTURE_MEDIAN_MULTISET_HPP

#include <set>

/**
 * @brief 動的中央値管理 (multiset)
 *
 * lower median を返す
 *
 * 計算量:
 *   add / del : O(log N)
 *   get       : O(1)
 * 
 * verified
 *   - https://atcoder.jp/contests/awc0055/submissions/75205292
 */

namespace kwm_t::data_structure {

template<typename T>
struct Median {
private:
    std::multiset<T> x, y;

    // 不変条件を満たすように調整
    void rebalance() {
        // サイズ調整
        while (x.size() > y.size() + 1) {
            y.insert(*x.rbegin());
            x.erase(std::prev(x.end()));
        }
        while (x.size() < y.size()) {
            x.insert(*y.begin());
            y.erase(y.begin());
        }

        // 順序調整（念のため）
        if (!x.empty() && !y.empty() && *x.rbegin() > *y.begin()) {
            T a = *x.rbegin();
            T b = *y.begin();
            x.erase(std::prev(x.end()));
            y.erase(y.begin());
            x.insert(b);
            y.insert(a);
        }
    }

public:
    void add(const T& val) {
        if (x.empty() || val <= *x.rbegin()) {
            x.insert(val);
        } else {
            y.insert(val);
        }
        rebalance();
    }

    // 存在は保証されている
    void del(const T& val) {
        if (auto it = x.find(val); it != x.end()) {
            x.erase(it);
        } else {
            it = y.find(val);
            y.erase(it);
        }
        rebalance();
    }

    T get() const {
        return *x.rbegin();
    }

    int size() const {
        return (int)(x.size() + y.size());
    }

    bool empty() const {
        return x.empty();
    }
};

} // namespace kwm_t::data_structure

#endif
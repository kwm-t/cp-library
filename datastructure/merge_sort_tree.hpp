/**
 * @file merge_sort_tree.hpp
 * @brief Merge Sort Tree
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/datastructure/merge_sort_tree.hpp
 */
#ifndef KWM_T_DATA_STRUCTURE_MERGE_SORT_TREE_HPP
#define KWM_T_DATA_STRUCTURE_MERGE_SORT_TREE_HPP

#include <vector>
#include <algorithm>
#include <cassert>

namespace kwm_t::data_structure {

/**
 * @brief Merge Sort Tree
 *
 * 典型用途:
 *   - 区間内の要素に対する順序統計 / 条件付き集計
 *   - k-th / count / prefix sum系クエリ
 *
 * 計算量:
 *   - build: O(N log N)
 *   - query: O(log^2 N)
 *
 * 注意:
 *   - 内部でソート済みベクタを保持
 *   - prefix sum により条件付き和を高速化
 * verified
 *   https://atcoder.jp/contests/abc339/submissions/75110500
 */
template <typename T>
class MergeSortTree {
public:
    MergeSortTree(const std::vector<T> &v, const T &INF) {
        init(v, INF);
    }

    T query(const int &l, const int &r, const T &threshold) {
        return queryHelper(0, size, 0, l, r, threshold);
    }

private:
    int log = 1;
    int size = 2;

    std::vector<std::vector<T>> node;
    std::vector<std::vector<T>> sumNode;

    void init(std::vector<T> v, const T &INF) {
        while (size < (int)v.size()) {
            log++;
            size *= 2;
        }
        while ((int)v.size() < size) {
            v.push_back(INF);
        }

        node.resize(size * 2);

        for (int i = 0; i < size; ++i) {
            node[i + size - 1].push_back(v[i]);
        }

        for (int i = size - 2; i >= 0; --i) {
            node[i] = merge(node[i * 2 + 1], node[i * 2 + 2]);
        }

        sumNode.resize(node.size());

        for (int i = 0; i < (int)node.size(); ++i) {
            sumNode[i].resize(node[i].size() + 1);
            for (int j = 0; j < (int)node[i].size(); ++j) {
                sumNode[i][j + 1] = sumNode[i][j] + node[i][j];
            }
        }
    }

    std::vector<T> merge(const std::vector<T> &l, const std::vector<T> &r) {
        std::vector<T> res;
        res.reserve(l.size() + r.size());

        int li = 0, ri = 0;

        while (li < (int)l.size() || ri < (int)r.size()) {
            if (li == (int)l.size()) {
                res.push_back(r[ri++]);
            } else if (ri == (int)r.size()) {
                res.push_back(l[li++]);
            } else if (l[li] < r[ri]) {
                res.push_back(l[li++]);
            } else {
                res.push_back(r[ri++]);
            }
        }

        return res;
    }

    T queryHelper(const int &l, const int &r, const int &index,
                  const int &targetL, const int &targetR,
                  const T &threshold) {
        if (l == r) return 0;
        if (targetR <= l) return 0;
        if (r <= targetL) return 0;

        if (targetL <= l && r <= targetR) {
            int ok = -1;
            int ng = (int)node[index].size();

            while (abs(ok - ng) > 1) {
                int mid = (ng + ok) / 2;
                if (node[index][mid] < threshold) ok = mid;
                else ng = mid;
            }

            return sumNode[index][ok + 1] - sumNode[index][0];
        }

        T res = 0;
        res += queryHelper(l, (l + r) / 2, 2 * index + 1,
                           targetL, targetR, threshold);
        res += queryHelper((l + r) / 2, r, 2 * index + 2,
                           targetL, targetR, threshold);

        return res;
    }
};

} // namespace kwm_t::data_structure

#endif // KWM_T_DATA_STRUCTURE_MERGE_SORT_TREE_HPP
/**
 * @file best_theorem.hpp
 * @brief 有向グラフの BEST 定理
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/graph/best_theorem.hpp
 */
#ifndef KWM_T_GRAPH_BEST_THEOREM_HPP
#define KWM_T_GRAPH_BEST_THEOREM_HPP

#include <vector>
#include <cassert>

namespace kwm_t::graph {

/**
 * @brief BEST Theorem（有向グラフのオイラー路 / オイラー閉路の数え上げ）
 *
 * 有向多重グラフにおいて、
 * - オイラー閉路（cycle）
 * - オイラー路（walk）
 *
 * の総数を数える。
 *
 * distinctEdge = false の場合、同一多重辺は区別しない。
 *
 * 典型用途:
 *   - オイラー路 / オイラー閉路の個数
 *   - 有向グラフの経路列挙数
 *
 * 計算量:
 *   O(N^3)（行列木定理）
 *
 * @tparam Mint
 *   modint など（inv() が定義されていること）
 *
 * @param edge
 *   隣接行列（多重辺可）
 *
 * @param fact, ifact
 *   階乗 / 逆階乗テーブル
 *
 * 制約 / 注意:
 *   - 孤立頂点があると壊れる（事前に除去 or 座圧）
 *   - execute() はグラフを破壊しない
 *
 * 使用例:
 *   BestTheorem<mint> bt(edge, fact, ifact);
 *   auto ans = bt.execute();
 *
 * verified:
 *   - https://atcoder.jp/contests/abc336/submissions/75096428
 *   - https://judge.yosupo.jp/submission/367220
 */
template <class Mint>
class BestTheorem {
public:
    BestTheorem(const std::vector<std::vector<int>>& edge,
                const std::vector<Mint>& fact,
                const std::vector<Mint>& ifact)
        : n(edge.size()), edge(edge), fact(fact), ifact(ifact) {}

    /**
     * @brief 実行
     *
     * @param startPoint
     *   cycle の場合の開始点（-1なら全て考慮）
     *
     * @param cycle
     *   オイラー閉路を許可
     *
     * @param walk
     *   オイラー路を許可
     *
     * @param distinctEdge
     *   辺を区別するか
     */
    Mint execute(int startPoint = -1,
                 bool cycle = true,
                 bool walk = true,
                 bool distinctEdge = false) {

        std::vector<int> in(n), out(n);

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                in[j] += edge[i][j];
                out[i] += edge[i][j];
            }
        }

        Mint val = 1;

        // 多重辺の重複除去
        if (!distinctEdge) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    val *= ifact[edge[i][j]];
                }
            }
        }

        int sv = -1, tv = -1, cnt = 0, tot = 0;

        for (int i = 0; i < n; ++i) {
            tot += out[i];
            if (in[i] == out[i]) continue;

            int diff = in[i] - out[i];
            if (diff >= 2 || diff <= -2) return Mint(0);

            if (diff == 1) tv = i, cnt++;
            if (diff == -1) sv = i;
        }

        if (cnt >= 2) return Mint(0);

        auto edge2 = edge;
        auto in2 = in;
        auto out2 = out;

        // cycle
        if (sv == -1) {
            if (!cycle) return Mint(0);
            if (startPoint == -1) val *= tot;
            else val *= out[startPoint];
        }
        // walk
        else {
            if (!walk) return Mint(0);
            // tv → sv を追加して cycle に変換
            out2[tv]++;
            in2[sv]++;
            edge2[tv][sv]++;
        }

        val *= count_spanning_tree(edge2);

        for (int i = 0; i < n; ++i) {
            val *= fact[out2[i] - 1];
        }

        return val;
    }

private:
    int n;
    std::vector<std::vector<int>> edge;
    const std::vector<Mint>& fact;
    const std::vector<Mint>& ifact;

    // 行列木定理
    Mint count_spanning_tree(const std::vector<std::vector<int>>& g) {
        std::vector<std::vector<Mint>> mat(n, std::vector<Mint>(n));

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                mat[i][j] -= g[i][j];
                mat[i][i] += g[i][j];
            }
        }

        int sz = n - 1;
        std::vector<std::vector<Mint>> a(sz, std::vector<Mint>(sz));

        for (int i = 1; i < n; ++i) {
            for (int j = 1; j < n; ++j) {
                a[i - 1][j - 1] = mat[i][j];
            }
        }

        Mint ret = 1;

        for (int i = 0; i < sz; ++i) {
            int pivot = -1;
            for (int j = i; j < sz; ++j) {
                if (a[j][i] != Mint(0)) {
                    pivot = j;
                    break;
                }
            }

            if (pivot == -1) return Mint(0);

            if (i != pivot) {
                std::swap(a[i], a[pivot]);
                ret = -ret;
            }

            ret *= a[i][i];
            Mint inv = a[i][i].inv();

            for (int j = i + 1; j < sz; ++j) {
                Mint factor = a[j][i] * inv;
                for (int k = i; k < sz; ++k) {
                    a[j][k] -= factor * a[i][k];
                }
            }
        }

        return ret;
    }
};

} // namespace kwm_t::graph

#endif // KWM_T_GRAPH_BEST_THEOREM_HPP
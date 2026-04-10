#ifndef KWM_T_GRAPH_K_MIN_CUT_HPP
#define KWM_T_GRAPH_K_MIN_CUT_HPP

#include <vector>
#include <limits>
#include <cassert>
#include <atcoder/maxflow>

namespace kwm_t::graph {

/**
 * @brief 多値変数の離散凸最適化（Project Selection Problem / Monge → Min-Cut）
 *
 * 各変数 x_i ∈ {0,1,...,sz[i]-1} を取り、
 * 以下の形のコストの総和を最小化する：
 *
 *   Σ f_i(x_i) + Σ f_{i,j}(x_i, x_j)
 *
 * ただし f_{i,j} は Monge 条件（離散凸）を満たす必要がある：
 *
 *   f(d,e) + f(d+1,e+1) ≤ f(d,e+1) + f(d+1,e)
 *
 * このとき、問題を最小カットに帰着して多項式時間で解く。
 *
 * ---
 *
 * 典型用途:
 *   - 数直線上の配置 + 二乗距離コスト
 *   - |x_i - x_j|^2 型の最適化
 *   - 画像処理のラベリング（グラフカット）
 *   - 多値ラベルの凸コスト最小化
 *   - Project Selection Problem（拡張）
 *
 * ---
 *
 * 計算量:
 *   ノード数: Σ(sz[i] - 1) + 2
 *   辺数:
 *     - unary: O(Σ sz[i])
 *     - pair:  O(Σ sz[i] * sz[j])
 *
 *   全体: O(E * sqrt(V)) （Dinic）
 *
 *   ※ sz[i] が小さい（例: ≤ 5）と高速
 *
 * ---
 *
 * @tparam Cap フローの容量型（int / long long 推奨）
 *
 * ---
 *
 * @param sizes 各変数の取りうる値の個数
 *
 * ---
 *
 * 制約 / 注意:
 *   - f_{i,j} は Monge 条件を満たす必要がある（assertあり）
 *   - コストはオーバーフローしないよう注意（Capに収まること）
 *   - sz[i] >= 1
 *   - pair は非対称でもOKだが、Monge条件は順序に依存
 *   - 同じ変数ペアに複数回 add_pair 可能（加算される）
 *   - solve() は1回のみ呼び出しを想定（flowを消費するため）
 *
 * ---
 *
 * 使用例:
 *   vector<int> sizes(n, m);
 *   KMinCut<long long> mc(sizes);
 *
 *   // unary
 *   mc.add_unary(i, [&](int d) {
 *       return cost[i][d];
 *   });
 *
 *   // pair（例: 二乗距離）
 *   mc.add_pair(i, j, [&](int x, int y) {
 *       ll d = pos[i][x] - pos[j][y];
 *       return w * d * d;
 *   });
 *
 *   ll ans = mc.solve();
 *   auto sol = mc.restore(); // 各 x_i
 *
 * ---
 *
 * verified:
 *   - https://mojacoder.app/users/_kanpurin_/problems/project_selection_problem003/submissions/530039b5-b5da-4959-9f90-79593ac5c97e
 */
template<class Cap>
class KMinCut {
private:
    int n;
    std::vector<int> sz, base;
    int S, T;
    atcoder::mf_graph<Cap> g;
    std::vector<Cap> unary;
    Cap offset = 0;
    Cap INF;

public:
    KMinCut(const std::vector<int>& sizes)
        : n(sizes.size()), sz(sizes) {

        INF = std::numeric_limits<Cap>::max() / 4;

        int ptr = 0;
        base.resize(n);
        for (int i = 0; i < n; i++) {
            base[i] = ptr;
            ptr += sz[i] - 1;
        }

        unary.assign(ptr, 0);
        S = ptr++;
        T = ptr++;

        g = atcoder::mf_graph<Cap>(ptr);
    }

    template<class F>
    void add_unary(int i, F f) {
        assert(0 <= i && i < n);
        int b = base[i];

        offset += f(0);
        for (int d = 0; d < sz[i] - 1; d++) {
            unary[b + d] += f(d + 1) - f(d);
        }
    }

    template<class F>
    void add_pair(int i, int j, F f) {
        assert(0 <= i && i < n);
        assert(0 <= j && j < n);

        int bi = base[i], bj = base[j];

        offset += f(0, 0);

        for (int d = 0; d < sz[i] - 1; d++) {
            unary[bi + d] += f(d + 1, 0) - f(d, 0);
        }
        for (int e = 0; e < sz[j] - 1; e++) {
            unary[bj + e] += f(0, e + 1) - f(0, e);
        }

        for (int d = 0; d < sz[i] - 1; d++) {
            for (int e = 0; e < sz[j] - 1; e++) {
                Cap c = f(d, e) - f(d+1, e) - f(d, e+1) + f(d+1, e+1);
                assert(c <= 0);
                if (c == 0) continue;

                unary[bj + e] += c;
                g.add_edge(bi + d, bj + e, -c);
            }
        }
    }

    Cap solve() {
        for (int i = 0; i < n; i++) {
            int b = base[i];

            for (int d = 0; d < sz[i] - 1; d++) {
                Cap w = unary[b + d];
                if (w > 0) {
                    g.add_edge(S, b + d, w);
                } else if (w < 0) {
                    offset += w;
                    g.add_edge(b + d, T, -w);
                }
            }

            for (int d = 0; d < sz[i] - 2; d++) {
                g.add_edge(b + d, b + d + 1, INF);
            }
        }

        return offset + g.flow(S, T);
    }

    std::vector<int> restore() {
        std::vector<bool> cut = g.min_cut(S);
        std::vector<int> res(n);

        for (int i = 0; i < n; i++) {
            int b = base[i];
            int d = 0;
            while (d < sz[i] - 1 && cut[b + d]) d++;
            res[i] = d;
        }
        return res;
    }
};

} // namespace kwm_t::graph

#endif // KWM_T_GRAPH_K_MIN_CUT_HPP
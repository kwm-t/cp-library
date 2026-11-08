
/**
 * @file min_weight_perfect_matching.hpp
 * @brief Tutte 行列を用いた最小重み完全マッチング
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/graph/matching/min_weight_perfect_matching.hpp
 */
#ifndef KWM_T_GRAPH_MATCHING_MIN_WEIGHT_PERFECT_MATCHING_HPP
#define KWM_T_GRAPH_MATCHING_MIN_WEIGHT_PERFECT_MATCHING_HPP

#include <algorithm>
#include <cassert>
#include <vector>

#include "../../math/modint/lagrange.hpp"
#include "../../math/modint/random_nonzero.hpp"
#include "../../math/matrix/matrix_field.hpp"

namespace kwm_t::graph::matching {

/**
 * @brief 最小重み完全マッチングを求める
 *
 * Tutte 行列の行列式と多項式補間を利用する乱択アルゴリズム。
 *
 * 典型用途:
 *  重み付き無向グラフの最小重み完全マッチングを求める。
 *
 * 計算量:
 *  O(repeat * n^4 * W)
 *  ただし n は頂点数、W は辺の最大重み。
 *
 * @tparam Mint 有限体上の modint 型
 * @tparam Random 乱数生成器の型
 * @param weight 重み行列
 *        weight[i][j] >= 0 のとき辺 (i, j) が存在し、その重みを表す。
 *        weight[i][j] == -1 のとき辺が存在しない。
 * @param rnd 乱数生成器
 * @param repeat 乱択を繰り返す回数
 * @return 最小重み完全マッチングの重みの総和。
 *         完全マッチングが存在しない場合は -1。
 *
 * 制約 / 注意:
 *  - weight は n x n の対称行列であること。
 *  - 対角成分は使用しない。
 *  - 辺の重みは非負整数であること。
 *  - n は偶数であること。奇数の場合は -1 を返す。
 *  - Mint は素数位数の有限体上の modint 型であること。
 *  - 多項式補間で使用する 0, 1, ..., n * W が相異なること。
 *    すなわち n * W が Mint の法より小さいこと。
 *  - repeat は 1 以上であること。
 *  - 乱択アルゴリズムであり、確率的な誤判定の可能性がある。
 *
 * 使用例:
 *  int ans = kwm_t::graph::matching::min_weight_perfect_matching<mint>(
 *      weight, rnd
 *  );
 *
 * verified:
 *  https://atcoder.jp/contests/abc412/submissions/80705679
 */
template <typename Mint, typename Random>
int min_weight_perfect_matching(
    const std::vector<std::vector<int>>& weight,
    Random& rnd,
    int repeat = 2
) {
    const int n = static_cast<int>(weight.size());

    if (n == 0) return 0;
    if (n % 2 == 1) return -1;

    assert(repeat >= 1);

    int max_weight = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (weight[i][j] == -1) continue;
            max_weight = std::max(max_weight, weight[i][j]);
        }
    }

    // det の次数は高々 n * max_weight
    const int lim = n * max_weight;

    int ret = lim + 1;

    for (int rep = 0; rep < repeat; ++rep) {
        kwm_t::math::matrix::MatrixField<Mint> base_mat(n);

        // Tutte 行列の構築
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (weight[i][j] == -1) continue;

                base_mat.a[i][j] =
                    kwm_t::math::modint::random_nonzero<Mint>(rnd);
                base_mat.a[j][i] = -base_mat.a[i][j];
            }
        }

        // det(y) を y = 0, 1, ..., lim で評価
        std::vector<Mint> dets(lim + 1);

        for (int y = 0; y <= lim; ++y) {
            auto mat = base_mat;

            // y^k を前計算
            std::vector<Mint> pow_y(max_weight + 1);
            pow_y[0] = Mint(1);

            for (int k = 1; k <= max_weight; ++k) {
                pow_y[k] = pow_y[k - 1] * Mint(y);
            }

            for (int i = 0; i < n; ++i) {
                for (int j = i + 1; j < n; ++j) {
                    if (weight[i][j] == -1) continue;

                    const int w = weight[i][j];
                    mat.a[i][j] *= pow_y[w];
                    mat.a[j][i] *= pow_y[w];
                }
            }

            dets[y] = mat.det();
        }

        // det(y) の係数を復元
        const auto coeff =
            kwm_t::math::modint::lagrange_coefficients(dets);

        // 最小の非ゼロ係数の次数を求める
        int val = lim + 1;

        for (int i = 0; i < static_cast<int>(coeff.size()); ++i) {
            if (coeff[i] != Mint(0)) {
                val = i;
                break;
            }
        }

        ret = std::min(ret, val);
    }

    if (ret == lim + 1) return -1;

    // det(Tutte) = Pf(Tutte)^2 より、最小次数は答えの2倍
    return ret / 2;
}

} // namespace kwm_t::graph::matching

#endif // KWM_T_GRAPH_MATCHING_MIN_WEIGHT_PERFECT_MATCHING_HPP

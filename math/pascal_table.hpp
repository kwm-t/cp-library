/**
 * @file pascal_table.hpp
 * @brief パスカルの三角形
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/pascal_table.hpp
 */
#ifndef KWM_T_MATH_PASCAL_TABLE_HPP
#define KWM_T_MATH_PASCAL_TABLE_HPP

#include <vector>

/**
 * @brief パスカルの三角形による組み合わせテーブル構築
 *
 * n 以下のすべての C(i, j) を O(n^2) で前計算する。
 * mod を使わない（逆元不要）環境で利用可能。
 *
 * 典型用途:
 *   - long long / int での組み合わせ
 *   - mod を取らない問題
 *   - 逆元が存在しない環境
 *
 * 計算量:
 *   構築 O(n^2)
 *   クエリ O(1)
 *
 * @tparam T 数値型 (int, long long, 任意型)
 *
 * @param n 最大値
 * @return res[i][j] = C(i, j)
 *
 * 制約 / 注意:
 *   - オーバーフローに注意（特に long long）
 *   - n ≈ 5000 くらいが現実的上限（メモリ的にも）
 *
 * 使用例:
 *   auto comb = kwm_t::math::combinatorics::pascal_table<long long>(1000);
 *   cout << comb[10][3]; // 120
 *
 * verified:
 *   https://yukicoder.me/submissions/1156632
 */
namespace kwm_t::math {

    template <typename T>
    std::vector<std::vector<T>> pascal_table(int n) {
        std::vector<std::vector<T>> res(n + 1, std::vector<T>(n + 1));

        for (int i = 0; i <= n; ++i) {
            res[i][0] = (T)1;
            for (int j = 1; j <= i; ++j) {
                res[i][j] = res[i - 1][j - 1] + res[i - 1][j];
            }
        }
        return res;
    }

} // namespace kwm_t::math

#endif // KWM_T_MATH_PASCAL_TABLE_HPP
/**
 * @file integer_partition.hpp
 * @brief 整数分割
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/integer_partition.hpp
 */
#ifndef KWM_T_MATH_INTEGER_PARTITION_HPP
#define KWM_T_MATH_INTEGER_PARTITION_HPP

#include <vector>
#include <algorithm>

/**
 * @brief 整数 n の分割（非増加列）を列挙する
 *
 * n を正の整数の和で表す全ての分割を列挙する。
 * 各分割は「非増加列（大きい順）」で返される。
 *
 * 例: n = 4
 *   {4}
 *   {3,1}
 *   {2,2}
 *   {2,1,1}
 *   {1,1,1,1}
 *
 * 典型用途:
 *   - 分割全列挙
 *   - 分割DPの検証
 *   - 制約付き列挙（長さ制限付きなど）
 *
 * 計算量:
 *   O(分割数) ≒ exp(π√(2n/3))
 *
 * @param n 対象の整数 (n >= 0)
 * @param max_size 分割の最大長（要素数制限）
 *
 * @return std::vector<std::vector<int>>
 *
 * 制約 / 注意:
 *   - 出力サイズは非常に大きくなりうる
 *   - n <= 30 程度までが現実的
 *
 * 使用例:
 *   auto res = integer_partition(5);
 *
 * verified:
 *   - 自作全探索
 */
namespace kwm_t::math {

    namespace internal {
        inline void dfs_partition(
            int n,
            int max_size,
            int& sum,
            std::vector<int>& current,
            std::vector<std::vector<int>>& result
        ) {
            if (sum > n) return;

            if (sum == n) {
                result.push_back(current);
                return;
            }

            if ((int)current.size() == max_size) return;

            int mx = n - sum;
            if (!current.empty()) {
                mx = std::min(mx, current.back());
            }

            for (int x = mx; x >= 1; --x) {
                current.push_back(x);
                sum += x;
                dfs_partition(n, max_size, sum, current, result);
                sum -= x;
                current.pop_back();
            }
        }
    }

    /**
     * @brief 分割列挙（長さ制限あり）
     */
    inline std::vector<std::vector<int>>
    integer_partition(int n, int max_size) {
        std::vector<std::vector<int>> result;
        std::vector<int> current;
        int sum = 0;

        internal::dfs_partition(n, max_size, sum, current, result);

        return result;
    }

    /**
     * @brief 分割列挙（制限なし）
     */
    inline std::vector<std::vector<int>>
    integer_partition(int n) {
        return integer_partition(n, n);
    }

} // namespace kwm_t::math

#endif // KWM_T_MATH_INTEGER_PARTITION_HPP
#ifndef KWM_T_MATH_LINEAR_ALGEBRA_GAUSS_JORDAN_HPP
#define KWM_T_MATH_LINEAR_ALGEBRA_GAUSS_JORDAN_HPP

#include <vector>
#include <utility>

namespace kwm_t::math {

/**
 * @brief Gauss-Jordan elimination（掃き出し法）
 *
 * Ax = b を解く / ランク / kernel を求める
 *
 * 要件:
 *   - T は体（+,-,*,/ が使える）
 *   - 0 判定ができる（== 0）
 *
 * verified:
 *   https://judge.yosupo.jp/submission/367294
 */
template <class T>
class GaussJordan {
public:
    GaussJordan(std::vector<std::vector<T>> A,
                std::vector<T> b)
        : A(std::move(A)), b(std::move(b)) {}

    /**
     * @brief 掃き出し（内部状態を書き換える）
     * @return rank
     */
    int sweep() {
        int n = A.size();
        int m = A[0].size();
        rank = 0;

        for (int col = 0; col < m; ++col) {
            int pivot = -1;
            for (int row = rank; row < n; ++row) {
                if (A[row][col] != T(0)) {
                    pivot = row;
                    break;
                }
            }
            if (pivot == -1) continue;

            std::swap(A[rank], A[pivot]);
            std::swap(b[rank], b[pivot]);

            T inv = T(1) / A[rank][col];

            for (int j = col; j < m; ++j) A[rank][j] *= inv;
            b[rank] *= inv;

            for (int row = 0; row < n; ++row) {
                if (row == rank) continue;
                if (A[row][col] == T(0)) continue;

                T factor = A[row][col];
                for (int j = col; j < m; ++j) {
                    A[row][j] -= factor * A[rank][j];
                }
                b[row] -= factor * b[rank];
            }

            ++rank;
        }
        return rank;
    }

    /**
     * @brief 解を1つ求める（存在すれば）
     */
    bool solve(std::vector<T>& x) {
        int n = A.size();
        int m = A[0].size();

        sweep();
        x.assign(m, T(0));

        for (int i = rank; i < n; ++i) {
            if (b[i] != T(0)) return false;
        }

        for (int i = 0; i < rank; ++i) {
            for (int j = 0; j < m; ++j) {
                if (A[i][j] != T(0)) {
                    x[j] = b[i];
                    break;
                }
            }
        }
        return true;
    }

    /**
     * @brief kernel（解空間の基底）
     */
    std::vector<std::vector<T>> kernel() {
        int n = A.size();
        int m = A[0].size();

        sweep();

        std::vector<int> pivot_col(m, -1);
        int row = 0;

        for (int col = 0; col < m && row < rank; ++col) {
            if (A[row][col] == T(1)) {
                pivot_col[col] = row;
                ++row;
            }
        }

        std::vector<std::vector<T>> res;

        for (int col = 0; col < m; ++col) {
            if (pivot_col[col] != -1) continue;

            std::vector<T> v(m, T(0));
            v[col] = T(1);

            for (int c = 0; c < m; ++c) {
                if (pivot_col[c] != -1) {
                    v[c] = -A[pivot_col[c]][col];
                }
            }
            res.push_back(v);
        }

        return res;
    }

    int rank = 0;

private:
    std::vector<std::vector<T>> A;
    std::vector<T> b;
};

} // namespace kwm_t::math

#endif
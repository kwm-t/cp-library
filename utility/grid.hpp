/**
 * @file grid.hpp
 * @brief グリッドの補助機能
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/utility/grid.hpp
 */
#ifndef KWM_T_UTILITY_GRID_HPP
#define KWM_T_UTILITY_GRID_HPP

#include <vector>
#include <string>
#include <algorithm>

/**
 * @brief グリッド操作ユーティリティ
 *
 * 提供機能:
 *   - rotate90 / 180 / 270
 *   - flip（上下・左右）
 *   - transpose（転置）
 *
 * 典型用途:
 *   - 盤面問題
 *   - 回転対称・一致判定
 *
 * 計算量:
 *   - すべて O(HW)
 *
 * 制約 / 注意:
 *   - 長方形配列であること
 *
 * verified:
 */
namespace kwm_t::utility {

// ==================== rotate ====================

// 90度回転（時計回り）string版
inline std::vector<std::string> rotate90(const std::vector<std::string>& g) {
    int h = (int)g.size();
    int w = (int)g[0].size();

    std::vector<std::string> res(w, std::string(h, g[0][0]));
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            res[j][h - 1 - i] = g[i][j];
        }
    }
    return res;
}

// 90度回転（時計回り）汎用版
template<class T>
std::vector<std::vector<T>> rotate90(const std::vector<std::vector<T>>& g) {
    int h = (int)g.size();
    int w = (int)g[0].size();

    std::vector<std::vector<T>> res(w, std::vector<T>(h));
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            res[j][h - 1 - i] = g[i][j];
        }
    }
    return res;
}

// 180度回転
template<class Grid>
Grid rotate180(const Grid& g) {
    int h = (int)g.size();
    int w = (int)g[0].size();

    Grid res = g;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            res[h - 1 - i][w - 1 - j] = g[i][j];
        }
    }
    return res;
}

// 270度回転（= 90度反時計回り）
template<class Grid>
Grid rotate270(const Grid& g) {
    return rotate90(rotate180(g));
}

// ==================== flip ====================

// 上下反転
template<class Grid>
Grid flip_vertical(const Grid& g) {
    Grid res = g;
    std::reverse(res.begin(), res.end());
    return res;
}

// 左右反転
template<class Grid>
Grid flip_horizontal(const Grid& g) {
    Grid res = g;
    for (auto& row : res) {
        std::reverse(row.begin(), row.end());
    }
    return res;
}

// ==================== transpose ====================

// 転置（string版）
inline std::vector<std::string> transpose(const std::vector<std::string>& g) {
    int h = (int)g.size();
    int w = (int)g[0].size();

    std::vector<std::string> res(w, std::string(h, g[0][0]));
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            res[j][i] = g[i][j];
        }
    }
    return res;
}

// 転置（汎用版）
template<class T>
std::vector<std::vector<T>> transpose(const std::vector<std::vector<T>>& g) {
    int h = (int)g.size();
    int w = (int)g[0].size();

    std::vector<std::vector<T>> res(w, std::vector<T>(h));
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            res[j][i] = g[i][j];
        }
    }
    return res;
}

} // namespace kwm_t::utility

#endif // KWM_T_UTILITY_GRID_HPP
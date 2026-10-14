/**
 * @file platonic_solid_graph.hpp
 * @brief 正多面体のグラフ
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/platonic_solid_graph.hpp
 */
#ifndef KWM_T_MATH_PLATONIC_SOLID_GRAPH_HPP
#define KWM_T_MATH_PLATONIC_SOLID_GRAPH_HPP

#include <vector>

namespace kwm_t::math {

/**
 * @brief 正多面体の種類
 */
enum class PlatonicSolid {
    Tetrahedron,   // 正四面体 (4頂点)
    Cube,          // 立方体 (8頂点)
    Octahedron,    // 正八面体 (6頂点)
    Dodecahedron,  // 正十二面体 (20頂点)
    Icosahedron    // 正二十面体 (12頂点)
};

/**
 * @brief 正多面体の隣接リストを返す
 *
 * 各頂点に対する隣接頂点のリストを返す
 *
 * verified:
 *   - known platonic solid graphs
 */
inline std::vector<std::vector<int>>
platonic_solid_graph(PlatonicSolid type) {
    using G = std::vector<std::vector<int>>;

    switch (type) {
    case PlatonicSolid::Tetrahedron:
        return G{
            {1,2,3},
            {0,2,3},
            {0,1,3},
            {0,1,2}
        };

    case PlatonicSolid::Cube:
        return G{
            {1,3,4},
            {0,2,5},
            {1,3,6},
            {0,2,7},
            {0,5,7},
            {1,4,6},
            {2,5,7},
            {3,4,6}
        };

    case PlatonicSolid::Octahedron:
        return G{
            {1,2,3,4},
            {0,2,4,5},
            {0,1,3,5},
            {0,2,4,5},
            {0,1,3,5},
            {1,2,3,4}
        };

    case PlatonicSolid::Dodecahedron:
        return G{
            {1,4,5},
            {0,2,6},
            {1,3,7},
            {2,4,8},
            {0,3,9},
            {0,10,14},
            {1,11,15},
            {2,12,16},
            {3,13,17},
            {4,14,18},
            {5,11,19},
            {6,10,12},
            {7,11,13},
            {8,12,14},
            {5,9,13},
            {6,16,19},
            {7,15,17},
            {8,16,18},
            {9,17,19},
            {10,15,18}
        };

    case PlatonicSolid::Icosahedron:
        return G{
            {1,2,3,4,5},
            {0,5,2,6,10},
            {0,1,3,10,9},
            {0,2,4,9,8},
            {0,3,5,8,7},
            {0,4,1,7,6},
            {11,10,7,1,5},
            {11,6,8,5,4},
            {11,7,9,4,3},
            {11,8,10,3,2},
            {11,9,6,2,1},
            {6,7,8,9,10}
        };
    }

    return {};
}

} // namespace kwm_t::math

#endif
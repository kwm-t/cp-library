/**
 * @file edge.hpp
 * @brief グラフの辺
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/graph/edge.hpp
 */
#ifndef KWM_T_GRAPH_EDGE_HPP
#define KWM_T_GRAPH_EDGE_HPP

namespace kwm_t::graph {

/**
 * @brief グラフ辺
 *
 * @tparam Cost
 *   辺コスト型
 */
template<class Cost>
struct Edge {
	int from;
	int to;
	Cost cost;
	int index = -1;
};

} // namespace kwm_t::graph

#endif // KWM_T_GRAPH_EDGE_HPP
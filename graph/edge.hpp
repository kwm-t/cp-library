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
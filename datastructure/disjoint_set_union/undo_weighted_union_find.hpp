#ifndef KWM_T_DATA_STRUCTURE_DISJOINT_SET_UNION_UNDO_WEIGHTED_UNION_FIND_HPP
#define KWM_T_DATA_STRUCTURE_DISJOINT_SET_UNION_UNDO_WEIGHTED_UNION_FIND_HPP

#include <vector>
#include <utility>

/**
 * @brief Undo可能 ポテンシャル付きUnion-Find
 *
 * 典型用途:
 *   - Undo付き差分制約
 *   - オフライン動的連結性 + 重み
 *
 * 計算量
 *   - merge / undo : O(1)
 *   - leader / diff : O(log N)
 *
 * 注意
 *   - 経路圧縮は行わない
 * verify
 *  - https://atcoder.jp/contests/awc0100/submissions/76973907
 */

namespace kwm_t::data_structure::disjoint_set_union {

template <
	class T,
	T(*op)(T, T),
	T(*inv)(T),
	T(*e)()>
class UndoWeightedUnionFind {
public:
	explicit UndoWeightedUnionFind(int n)
		: n(n),
		par_or_size(n, -1),
		weight(n, e()),
		ng(n, false) {}

	int leader(int v) const {
		while (par_or_size[v] >= 0) v = par_or_size[v];
		return v;
	}

	T potential(int v) const {
		T res = e();
		while (par_or_size[v] >= 0) {
			res = op(res, weight[v]);
			v = par_or_size[v];
		}
		return res;
	}

	bool same(int u, int v) const {
		return leader(u) == leader(v);
	}

	bool merge(int u, int v, T w) {
		w = op(op(potential(u), w), inv(potential(v)));

		int ru = leader(u);
		int rv = leader(v);

		history.emplace_back(
			ru, par_or_size[ru], ng[ru],
			rv, par_or_size[rv], weight[rv], ng[rv]);

		if (ru == rv) {
			if (w != e()) ng[ru] = true;
			return w == e();
		}

		if (-par_or_size[ru] < -par_or_size[rv]) {
			std::swap(ru, rv);
			w = inv(w);
		}

		par_or_size[ru] += par_or_size[rv];
		par_or_size[rv] = ru;
		weight[rv] = w;

		nng[ru] = ng[ru] | ng[rv];

		return true;
	}

	int size(int v) const {
		return -par_or_size[leader(v)];
	}

	T diff(int u, int v) const {
		if (!same(u, v)) return e();
		return op(inv(potential(u)), potential(v));
	}

	bool is_invalid(int v) const {
		return ng[leader(v)];
	}

	void undo() {
		auto h = history.back();
		history.pop_back();

		par_or_size[h.ru] = h.par_ru;
		ng[h.ru] = h.ng_ru;

		par_or_size[h.rv] = h.par_rv;
		weight[h.rv] = h.weight_rv;
		ng[h.rv] = h.ng_rv;
	}

	int snapshot() const {
		return history.size();
	}

	void rollback(int snap) {
		while ((int)history.size() > snap) undo();
	}

private:
	struct History {
		int ru;
		int par_ru;
		bool ng_ru;

		int rv;
		int par_rv;
		T weight_rv;
		bool ng_rv;

		History(int ru, int par_ru, bool ng_ru, int rv, int par_rv, T weight_rv, bool ng_rv)
			: ru(ru), par_ru(par_ru), ng_ru(ng_ru), rv(rv), par_rv(par_rv), weight_rv(weight_rv), ng_rv(ng_rv) {}
	};

	int n;

	std::vector<int> par_or_size;
	std::vector<T> weight;
	std::vector<bool> ng;

	std::vector<History> history;
};

}  // namespace kwm_t::data_structure::disjoint_set_union
#endif
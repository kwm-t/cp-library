#ifndef KWM_T_DATA_STRUCTURE__DISJOINT_SET_UNION_PARTIALLY_PERSISTENT_UNION_FIND_HPP
#define KWM_T_DATA_STRUCTURE__DISJOINT_SET_UNION_PARTIALLY_PERSISTENT_UNION_FIND_HPP

#include <vector>
#include <utility>
#include <algorithm>
#include <cstdlib>

/**
 * @brief 部分永続Union-Find
 *
 * 典型用途:
 *   - 時刻付き連結判定
 *   - 2頂点がいつ連結になったか
 *   - 過去時点での連結成分サイズ取得
 *   - 過去時点での連結成分列挙
 *
 * 計算量:
 *   merge: O(log N)
 *   leader: O(log N)
 *   size: O(log N)
 *   merge_time: O(log N)
 *   group: O(|set| log N)
 *
 * 制約 / 注意:
 *   - パス圧縮は使えない
 *   - union by size 必須
 *   - 時刻は unite ごとに +1
 *
 * verified:
 *   - https://atcoder.jp/contests/code-thanks-festival-2017/submissions/75190727
 *   - https://atcoder.jp/contests/agc002/submissions/75190869
 *   - https://atcoder.jp/contests/past202004-open/submissions/75191053
 */
namespace kwm_t::data_structure::disjoint_set_union {

class PartiallyPersistentUnionFind {
private:
	static constexpr long long INF = (1LL << 60);

	int N;
	int cur_time;

	std::vector<int> par;
	std::vector<long long> merge_times;
	std::vector<std::vector<std::pair<long long, int>>> size_history;
	std::vector<std::vector<std::pair<long long, int>>> next_v;

	template <class F>
	int binary_search(int ok, int ng, F is_valid) const {
		while (std::abs(ok - ng) > 1) {
			int mid = (ok + ng) >> 1;
			if (is_valid(mid)) ok = mid;
			else ng = mid;
		}
		return ok;
	}

	int find_next(long long t, int x) const {
		const std::vector<std::pair<long long, int>>& vec = next_v[x];

		auto is_valid = [&](int i) {
			return vec[i].first <= t;
		};

		int idx = binary_search(0, (int)vec.size(), is_valid);
		return vec[idx].second;
	}

public:
	explicit PartiallyPersistentUnionFind(int n)
		: N(n), cur_time(0),
		par(n, -1),
		merge_times(n, INF),
		size_history(n),
		next_v(n)
	{
		for (int i = 0; i < n; i++) {
			size_history[i].push_back(std::make_pair(0LL, 1));
			next_v[i].push_back(std::make_pair(0LL, i));
		}
	}

	int merge(int x, int y) {
		int rx = leader(cur_time, x);
		int ry = leader(cur_time, y);

		cur_time++;

		if (rx == ry) return rx;

		if (par[rx] > par[ry]) std::swap(rx, ry);

		par[rx] += par[ry];
		par[ry] = rx;
		merge_times[ry] = cur_time;

		size_history[rx].push_back(
			std::make_pair((long long)cur_time, -par[rx])
		);

		int next_rx = next_v[rx].back().second;
		int next_ry = next_v[ry].back().second;

		next_v[rx].push_back(
			std::make_pair((long long)cur_time, next_ry)
		);
		next_v[ry].push_back(
			std::make_pair((long long)cur_time, next_rx)
		);

		return rx;
	}

	bool same(long long t, int x, int y) const {
		return leader(t, x) == leader(t, y);
	}

	int leader(long long t, int x) const {
		while (merge_times[x] <= t) {
			x = par[x];
		}
		return x;
	}

	int size(long long t, int x) const {
		int rx = leader(t, x);

		auto is_valid = [&](int i) {
			return size_history[rx][i].first <= t;
		};

		int idx = binary_search(0, (int)size_history[rx].size(), is_valid);
		return size_history[rx][idx].second;
	}

	int merge_time(int x, int y) const {
		if (!same(cur_time, x, y)) return -1;

		auto is_valid = [&](int t) {
			return same(t, x, y);
		};

		return binary_search(cur_time, -1, is_valid);
	}

	std::vector<int> group(long long t, int x) const {
		int rx = leader(t, x);

		std::vector<int> res;
		res.push_back(rx);

		int cur = find_next(t, rx);
		while (cur != rx) {
			res.push_back(cur);
			cur = find_next(t, cur);
		}

		return res;
	}
};

} // namespace kwm_t::data_structure::disjoint_set_union

#endif // KWM_T_DATA_STRUCTURE__DISJOINT_SET_UNION_PARTIALLY_PERSISTENT_UNION_FIND_HPP
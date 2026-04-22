#ifndef KWM_T_DATASTRUCTURE_INTERVAL_MAP_HPP
#define KWM_T_DATASTRUCTURE_INTERVAL_MAP_HPP

#include <set>
#include <vector>
#include <optional>
#include <cassert>

namespace kwm_t::data_structure {

template <typename T>
class IntervalMap {
private:
	struct Range {
		int l, r;
		T val;

		bool operator<(const Range& o) const {
			return l < o.l;
		}
	};

	std::set<Range> st;
	int INF;

	using iterator = typename std::set<Range>::iterator;

	bool isSentinel(const Range& x) const {
		return x.l == -INF || x.l == INF;
	}

	// --- split ---
	iterator split(int pos) {
		auto it = std::prev(st.lower_bound({ pos + 1, 0, T() }));

		if (isSentinel(*it)) return std::next(it);
		if (it->l == pos) return it;
		if (it->r <= pos) return std::next(it);

		Range cur = *it;
		st.erase(it);

		st.insert({ cur.l, pos, cur.val });
		return st.insert({ pos, cur.r, cur.val }).first;
	}

public:
	// --- 空構造 ---
	explicit IntervalMap(int limit)
		: INF(limit + 1)
	{
		st.insert({ -INF, -INF, T() });
		st.insert({ INF, INF, T() });
	}

	// --- 配列初期化 ---
	explicit IntervalMap(const std::vector<T>& data)
		: INF((int)data.size() + 1)
	{
		st.insert({ -INF, -INF, T() });
		st.insert({ INF, INF, T() });

		int n = (int)data.size();
		for (int i = 0; i < n;) {
			int j = i + 1;
			while (j < n && data[i] == data[j]) j++;
			st.insert({ i, j, data[i] });
			i = j;
		}
	}

	// --- 区間代入 ---
	std::vector<Range> assign(int l, int r, const T& val) {
		if (l >= r) return {};

		auto itr = split(r);
		auto itl = split(l);

		std::vector<Range> removed;

		for (auto it = itl; it != itr; ++it) {
			if (!isSentinel(*it)) removed.push_back(*it);
		}

		st.erase(itl, itr);

		int nl = l, nr = r;

		// 左マージ
		auto it = std::prev(st.lower_bound({ l + 1, 0, T() }));
		if (!isSentinel(*it) && it->r == l && it->val == val) {
			nl = it->l;
			st.erase(it);
		}

		// 右マージ
		it = st.lower_bound({ r, 0, T() });
		if (it != st.end() && !isSentinel(*it) && it->l == r && it->val == val) {
			nr = it->r;
			st.erase(it);
		}

		st.insert({ nl, nr, val });
		return removed;
	}

	// --- 区間削除 ---
	std::vector<Range> eraseRange(int l, int r) {
		if (l >= r) return {};

		auto itr = split(r);
		auto itl = split(l);

		std::vector<Range> removed;

		for (auto it = itl; it != itr; ++it) {
			if (!isSentinel(*it)) removed.push_back(*it);
		}

		st.erase(itl, itr);
		return removed;
	}

	// --- 被覆判定 ---
	bool isCovered(int l, int r) const {
		auto it = std::prev(st.lower_bound({ l + 1, 0, T() }));
		if (isSentinel(*it)) return false;
		return it->l <= l && r <= it->r;
	}

	bool isCovered(int x) const {
		return isCovered(x, x + 1);
	}

	// --- 値取得 ---
	std::optional<T> get(int x) const {
		auto it = std::prev(st.lower_bound({ x + 1, 0, T() }));
		if (isSentinel(*it)) return std::nullopt;
		if (!(it->l <= x && x < it->r)) return std::nullopt;
		return it->val;
	}

	// --- 区間取得 ---
	std::optional<Range> getRange(int x) const {
		auto it = std::prev(st.lower_bound({ x + 1, 0, T() }));
		if (isSentinel(*it)) return std::nullopt;
		if (!(it->l <= x && x < it->r)) return std::nullopt;
		return *it;
	}

	// --- 先頭 ---
	std::optional<Range> front() const {
		auto it = st.begin(); ++it;
		if (isSentinel(*it)) return std::nullopt;
		return *it;
	}

	// --- 末尾 ---
	std::optional<Range> back() const {
		auto it = st.end(); --it; --it;
		if (isSentinel(*it)) return std::nullopt;
		return *it;
	}

	// --- 区間数 ---
	int rangeCount() const {
		return (int)st.size() - 2;
	}
};

} // namespace kwm_t::data_structure

#endif
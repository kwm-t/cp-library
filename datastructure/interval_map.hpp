#ifndef KWM_T_DATASTRUCTURE_INTERVAL_MAP_HPP
#define KWM_T_DATASTRUCTURE_INTERVAL_MAP_HPP

#include <set>
#include <vector>
#include <optional>
#include <functional>
#include <cassert>
#include <tuple>
#include <utility>

/**
 * @brief 半開区間 [l, r) の集合を管理するデータ構造（Interval Map）
 *
 * 典型用途:
 *   - 区間の追加・削除・値の更新
 *   - 区間のマージ管理（重なり・隣接の統合）
 *   - 区間被覆判定
 *   - 区間被覆を利用した貪欲・シミュレーション問題
 *
 * 計算量:
 *   - 各操作: O(k log N)
 *     (k は分割・削除・マージされる区間数)
 *
 * @tparam T
 *   - 区間に対応する値の型
 *
 * @param limit
 *   - 管理する座標の上限（番兵として使用）
 *
 * 制約 / 注意:
 *   - 半開区間 [l, r) を扱う
 *   - 内部で番兵（-INF, INF）を保持
 *   - 区間は常に「互いに素かつ同じ値の隣接区間がマージされた状態」を保つ
 *
 * verified:
 *   https://atcoder.jp/contests/awc0053/submissions/79682515
 *   https://yukicoder.me/submissions/1191292
 */
namespace kwm_t::data_structure {

template <typename T>
class IntervalMap {
public:
	struct Range {
		int l, r;
		T val;

		bool operator<(const Range& o) const {
			return l < o.l;
		}
	};

private:
	using Callback = std::function<void(int, int, const T&)>;

	Callback onInsert;
	Callback onErase;

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
		eraseRange(it);

		addRange(cur.l, pos, cur.val);
		return addRange(pos, cur.r, cur.val);
	}

	iterator eraseRange(iterator it) {
		onErase(it->l, it->r, it->val);
		return st.erase(it);
	}

	iterator addRange(int l, int r, const T& val) {
		onInsert(l, r, val);
		return st.insert({ l, r, val }).first;
	}

public:
	// --- 空構造 ---
	explicit IntervalMap(
		int limit,
		Callback onInsert = [](int, int, const T&) {},
		Callback onErase = [](int, int, const T&) {}
	)
		: onInsert(std::move(onInsert)),
		onErase(std::move(onErase)),
		INF(limit + 1)
	{
		st.insert({ -INF, -INF, T() });
		st.insert({ INF, INF, T() });
	}

	// --- 配列初期化 ---
	template <class Container>
	IntervalMap(
		const Container& data,
		Callback onInsert = [](int, int, const T&) {},
		Callback onErase = [](int, int, const T&) {}
	)
		: onInsert(std::move(onInsert)),
		onErase(std::move(onErase)),
		INF((int)data.size() + 1)
	{
		st.insert({ -INF, -INF, T() });
		st.insert({ INF, INF, T() });

		int n = (int)data.size();

		for (int i = 0; i < n;) {
			int j = i + 1;
			while (j < n && data[i] == data[j]) ++j;

			addRange(i, j, data[i]);

			i = j;
		}
	}

	// --- 区間代入 ---
	void insert(int l, int r, const T& val) {
		if (l >= r) return;

		auto itr = split(r);
		auto itl = split(l);

		// [l, r) に含まれる既存区間を削除
		auto it = itl;
		while (it != itr) {
			auto jt = std::next(it);
			eraseRange(it);
			it = jt;
		}

		int nl = l;
		int nr = r;

		// 左マージ
		it = std::prev(st.lower_bound({ l + 1, 0, T() }));
		if (!isSentinel(*it) && it->r == l && it->val == val) {
			nl = it->l;
			eraseRange(it);
		}

		// 右マージ
		it = st.lower_bound({ r, 0, T() });
		if (it != st.end() && !isSentinel(*it) &&
			it->l == r && it->val == val) {
			nr = it->r;
			eraseRange(it);
		}

		addRange(nl, nr, val);
	}

	void insertPoint(int x, const T& val) {
		insert(x, x + 1, val);
	}

	// --- 区間削除 ---
	void eraseRange(int l, int r) {
		if (l >= r) return;

		auto itr = split(r);
		auto itl = split(l);

		auto it = itl;
		while (it != itr) {
			auto jt = std::next(it);
			eraseRange(it);
			it = jt;
		}
	}

	void erasePoint(int x) {
		eraseRange(x, x + 1);
	}

	// --- 被覆判定 ---
	bool isCovered(int l, int r) const {
		if (l >= r) return true;

		auto it = std::prev(st.lower_bound({ l + 1, 0, T() }));
		if (isSentinel(*it)) return false;

		return it->l <= l && r <= it->r;
	}

	bool isCovered(int x) const {
		return isCovered(x, x + 1);
	}

	// --- 値取得 ---
	std::optional<T> getValue(int x) const {
		auto res = get(x);
		if (!res) return std::nullopt;
		return res->val;
	}

	// --- 区間・値取得 ---
	std::optional<Range> get(int x) const {
		auto it = std::prev(st.lower_bound({ x + 1, 0, T() }));

		if (isSentinel(*it)) return std::nullopt;
		if (!(it->l <= x && x < it->r)) return std::nullopt;

		return *it;
	}

	// --- 先頭 ---
	std::optional<Range> front() const {
		auto it = std::next(st.begin());

		if (isSentinel(*it)) return std::nullopt;

		return *it;
	}

	// --- 末尾 ---
	std::optional<Range> back() const {
		auto it = std::prev(st.end());
		it = std::prev(it);

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
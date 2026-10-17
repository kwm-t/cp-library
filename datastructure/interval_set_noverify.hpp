/**
 * @file interval_set_noverify.hpp
 * @brief 検証用処理を含まない Interval Set
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/datastructure/interval_set_noverify.hpp
 */
#ifndef KWM_T_DATA_STRUCTURE_INTERVAL_SET_HPP
#define KWM_T_DATA_STRUCTURE_INTERVAL_SET_HPP

#include <set>
#include <functional>
#include <utility>

/**
 * @brief 半開区間集合 [l, r) を管理するデータ構造
 *
 * 典型用途:
 *   - 区間の union 管理
 *   - mex 管理
 *   - 区間の被覆判定
 *
 * 計算量:
 *   - 各操作 O(log N)
 *
 * @tparam T
 *   - 順序比較可能
 *
 * 制約 / 注意:
 *   - 半開区間 [l, r)
 *   - 番兵として (-INF, -INF), (INF, INF) を内部に持つ
 *
 * verified(old):
 *   - https://atcoder.jp/contests/abc430/submissions/75172066
 *   - https://atcoder.jp/contests/awc0078/submissions/76170297
 * verified(new):
 *   - https://atcoder.jp/contests/awc0175/submissions/79896129
 */
namespace kwm_t::data_structure {

template<typename T>
class IntervalSet {
private:
	using Callback = std::function<void(T, T)>;
	using iterator = typename std::set<std::pair<T, T>>::iterator;

	std::set<std::pair<T, T>> ranges_;
	T TINF;
	Callback onInsert;
	Callback onErase;

	iterator eraseRange(iterator it) {
		onErase(it->first, it->second);
		return ranges_.erase(it);
	}

	void addRange(T l, T r) {
		onInsert(l, r);
		ranges_.emplace(l, r);
	}

public:
	explicit IntervalSet(
		T limit,
		Callback onInsert = [](T, T) {},
		Callback onErase = [](T, T) {}
	)
		: TINF(limit),
		onInsert(std::move(onInsert)),
		onErase(std::move(onErase))
	{
		ranges_.emplace(-TINF, -TINF);
		ranges_.emplace(TINF, TINF);
	}

	// [l,r) がカバーされているか
	bool isCovered(T l, T r) const noexcept {
		if (l >= r) return true;
		auto it = std::prev(ranges_.upper_bound({ l, TINF }));
		return (it->first <= l && r <= it->second);
	}
	bool isCovered(T x) const noexcept { return isCovered(x, x + 1); }

	// カバー区間取得
	std::pair<T, T> getCoveringRange(T l, T r) const noexcept {
		auto it = std::prev(ranges_.upper_bound({ l, TINF }));
		if (it->first <= l && r <= it->second) return *it;
		return { -TINF, -TINF };
	}
	std::pair<T, T> getCoveringRange(T x) const noexcept {
		return getCoveringRange(x, x + 1);
	}

	std::pair<T, T> front() const noexcept {
		return *std::next(ranges_.begin());
	}

	std::pair<T, T> back() const noexcept {
		return *std::prev(std::prev(ranges_.end()));
	}

	// --- 挿入 ---
	void insertRange(T l, T r) {
		if (l >= r) return;

		auto it = std::prev(ranges_.upper_bound({ l, TINF }));

		if (isCovered(l, r)) return;

		// 左マージ
		if (it->first <= l && l <= it->second) {
			l = it->first;
			it = eraseRange(it);
		}
		else {
			++it;
		}

		// 右方向マージ
		while (it->first <= r) {
			r = std::max(r, it->second);
			it = eraseRange(it);
		}

		addRange(l, r);
	}
	void insertPoint(T x) { insertRange(x, x + 1); }

	// --- 削除 ---
	void eraseRange(T l, T r) {
		if (l >= r) return;

		auto it = std::prev(ranges_.upper_bound({ l, TINF }));

		// 1区間内
		if (it->first <= l && r <= it->second) {
			auto [L, R] = *it;
			eraseRange(it);
			if (L < l) addRange(L, l);
			if (r < R) addRange(r, R);
			return;
		}

		// 左端
		if (it->first <= l && l < it->second) {
			auto [L, R] = *it;
			it = eraseRange(it);
			addRange(L, l);
		}
		else {
			++it;
		}

		// 中央
		while (it->second <= r) {
			it = eraseRange(it);
		}

		// 右端
		if (it->first < r && r < it->second) {
			auto [L, R] = *it;
			eraseRange(it);
			addRange(r, R);
		}
	}
	void erasePoint(T x) { eraseRange(x, x + 1); }

	int rangeCount() const noexcept {
		return (int)ranges_.size() - 2;
	}

	// mex
	T mex(T x = 0) const noexcept {
		if (!isCovered(x)) return x;
		auto it = std::prev(ranges_.upper_bound({ x, TINF }));
		return it->second;
	}

	void debug() const noexcept {
		for (auto [l, r] : ranges_) {
			if (l != -TINF && l != TINF) {
				std::cout << "[" << l << "," << r << ")\n";
			}
		}
	}
};


} // namespace kwm_t::data_structure

#endif // KWM_T_DATA_STRUCTURE_INTERVAL_SET_HPP
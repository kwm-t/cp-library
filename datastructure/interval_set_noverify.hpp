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
 * verified:
 *   - https://atcoder.jp/contests/abc430/submissions/75172066
 *   - https://atcoder.jp/contests/awc0078/submissions/76170297
 */
namespace kwm_t::data_structure {

template<typename T>
class IntervalSet {
public:
	explicit IntervalSet(T limit)
		: TINF(limit)
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
		auto it = std::next(ranges_.begin());
		return *it;
	}

	std::pair<T, T> back() const noexcept {
		auto it = std::prev(std::prev(ranges_.end()));
		return *it;
	}

	// --- 挿入 ---
	void insertRange(
		T l, T r,
		const std::function<void(T, T)>& onInsert = [](T, T) {},
		const std::function<void(T, T)>& onErase = [](T, T) {}
	) {
		if (l >= r) return;

		auto it = std::prev(ranges_.upper_bound({ l, TINF }));

		if (isCovered(l, r)) return;

		// 左マージ
		if (it->first <= l && l <= it->second) {
			l = it->first;
			eraseRange(it, onErase);
		}
		else {
			++it;
		}

		// 右方向マージ
		while (it->first <= r) {
			r = std::max(r, it->second);
			eraseRange(it, onErase);
		}

		addRange(l, r, onInsert);
	}

	void insertPoint(
		T x,
		const std::function<void(T, T)>& onInsert = [](T, T) {},
		const std::function<void(T, T)>& onErase = [](T, T) {}
	) {
		insertRange(x, x + 1, onInsert, onErase);
	}

	// --- 削除 ---
	void eraseRange(
		T l, T r,
		const std::function<void(T, T)>& onInsert = [](T, T) {},
		const std::function<void(T, T)>& onErase = [](T, T) {}
	) {
		if (l >= r) return;

		auto it = std::prev(ranges_.upper_bound({ l, TINF }));

		// 1区間内
		if (it->first <= l && r <= it->second) {
			auto[L, R] = *it;
			eraseRange(it, onErase);
			if (L < l) addRange(L, l, onInsert);
			if (r < R) addRange(r, R, onInsert);
			return;
		}

		// 左端
		if (it->first <= l && l < it->second) {
			auto[L, R] = *it;
			eraseRange(it, onErase);
			addRange(L, l, onInsert);
		}
		else {
			++it;
		}

		// 中央
		while (it->second <= r) {
			eraseRange(it, onErase);
		}

		// 右端
		if (it->first < r && r < it->second) {
			auto[L, R] = *it;
			eraseRange(it, onErase);
			addRange(r, R, onInsert);
		}
	}

	void erasePoint(
		T x,
		const std::function<void(T, T)>& onInsert = [](T, T) {},
		const std::function<void(T, T)>& onErase = [](T, T) {}
	) {
		eraseRange(x, x + 1, onInsert, onErase);
	}

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
		for (auto[l, r] : ranges_) {
			if (l != -TINF && l != TINF) {
				std::cout << "[" << l << "," << r << ")\n";
			}
		}
	}

private:
	std::set<std::pair<T, T>> ranges_;
	T TINF;

	void eraseRange(typename std::set<std::pair<T, T>>::iterator& it,
		const std::function<void(T, T)>& onErase) {
		onErase(it->first, it->second);
		it = ranges_.erase(it);
	}

	void addRange(T l, T r,
		const std::function<void(T, T)>& onInsert) {
		onInsert(l, r);
		ranges_.emplace(l, r);
	}
};


} // namespace kwm_t::data_structure

#endif // KWM_T_DATA_STRUCTURE_INTERVAL_SET_HPP
/**
 * @file interval_set.hpp
 * @brief 区間集合を管理する Interval Set
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/datastructure/interval_set.hpp
 */
#ifndef KWM_T_DATA_STRUCTURE_INTERVAL_SET_HPP
#define KWM_T_DATA_STRUCTURE_INTERVAL_SET_HPP

#include <set>
#include <functional>
#include <utility>
#include <vector>

 /**
  * @brief 閉区間集合 [l, r] を管理するデータ構造
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
  *   - 閉区間 [l, r]
  *   - 番兵として (-INF, -INF), (INF, INF) を内部に持つ
  *
  * verified:
  *  https://atcoder.jp/contests/awc0078/submissions/76168909
  */
namespace kwm_t::data_structure {

template<typename T>
class IntervalSet {
public:
	// --- コンストラクタ ---
	explicit IntervalSet(T limit)
		: TINF(limit + 1)
	{
		ranges_.emplace(-TINF, -TINF);
		ranges_.emplace(TINF, TINF);
	}

	// --- 公開API ---

	// 区間がすでにカバーされているか
	bool isCovered(T l, T r) const noexcept {
		auto it = std::prev(ranges_.lower_bound({ l + 1, l + 1 }));
		return (it->first <= l && r <= it->second);
	}
	bool isCovered(T x) const noexcept { return isCovered(x, x); }

	// カバーしている区間を返す（存在しなければ {-TINF, -TINF}）
	std::pair<T, T> getCoveringRange(T l, T r) const noexcept {
		auto it = std::prev(ranges_.lower_bound({ l + 1, l + 1 }));
		if (it->first <= l && r <= it->second) return *it;
		return { -TINF, -TINF };
	}
	std::pair<T, T> getCoveringRange(T x) const noexcept { return getCoveringRange(x, x); }

	// 先頭区間を取得
	std::pair<T, T> front() const noexcept {
		auto it = ranges_.begin();
		++it;
		return *it;
	}

	// 末尾区間を取得
	std::pair<T, T> back() const noexcept {
		auto it = ranges_.end();
		--it; --it;
		return *it;
	}

	// 区間挿入（マージあり）
	void insertRange(
		T l, T r,
		const std::function<void(T, T)>& onInsert = [](T, T) {},
		const std::function<void(T, T)>& onErase = [](T, T) {}
	) {
		auto it = std::prev(ranges_.lower_bound({ l + 1, l + 1 }));

		if (isCovered(l, r)) return; // 既に含まれている

		// 左端が既存区間に隣接 or 内包されている場合はマージ
		if (it->first <= l && l <= it->second + 1) {
			l = it->first;
			eraseRange(it, onErase);
		}
		else {
			++it;
		}

		// 右端方向にマージ
		while (r > it->second) eraseRange(it, onErase);

		if (it->first - 1 <= r && r <= it->second) {
			r = it->second;
			eraseRange(it, onErase);
		}

		addRange(l, r, onInsert);
	}

	// 1点挿入
	void insertPoint(
		T x,
		const std::function<void(T, T)>& onInsert = [](T, T) {},
		const std::function<void(T, T)>& onErase = [](T, T) {}
	) {
		insertRange(x, x, onInsert, onErase);
	}

	// 区間削除
	void eraseRange(
		T l, T r,
		const std::function<void(T, T)>& onInsert = [](T, T) {},
		const std::function<void(T, T)>& onErase = [](T, T) {}
	) {
		auto it = std::prev(ranges_.lower_bound({ l + 1, l + 1 }));

		// カバーされている1区間のみのケース
		if (it->first <= l && r <= it->second) {
			onErase(it->first, it->second);
			auto old = *it;
			ranges_.erase(it);
			if (old.first < l) addRange(old.first, l - 1, onInsert);
			if (r < old.second) addRange(r + 1, old.second, onInsert);
			return;
		}

		// 左端処理
		if (it->first <= l && l <= it->second) {
			if (it->first < l) addRange(it->first, l - 1, onInsert);
			eraseRange(it, onErase);
		}
		else {
			++it;
		}

		// 中央部削除
		while (it->second <= r) eraseRange(it, onErase);

		// 右端処理
		if (it->first <= r && r <= it->second) {
			if (r < it->second) addRange(r + 1, it->second, onInsert);
			eraseRange(it, onErase);
		}
	}

	// 1点削除
	void erasePoint(
		T x,
		const std::function<void(int, int)>& onInsert = [](int, int) {},
		const std::function<void(int, int)>& onErase = [](int, int) {}
	) {
		eraseRange(x, x, onInsert, onErase);
	}

	// 登録取得
	std::vector<std::pair<T, T>> getRanges() const {
		std::vector<std::pair<T, T>> res;
		res.reserve(rangeCount());
		for (auto it = std::next(ranges_.begin()); it != std::prev(ranges_.end()); ++it) {
			res.push_back(*it);
		}
		return res;
	}

	// 登録区間数
	int rangeCount() const noexcept { return static_cast<int>(ranges_.size()) - 2; }

	// Mex（未カバー最小値）
	T mex(T x = 0) const noexcept {
		if (!isCovered(x)) return x;
		auto it = std::prev(ranges_.lower_bound({ x + 1, x + 1 }));
		return it->second + 1;
	}

	// デバッグ表示
	void debug() const noexcept {
		for (auto [l, r] : ranges_)
			if (std::abs(l) != TINF)
				std::cout << l << " " << r << "\n";
	}

private:
	// --- 内部状態 ---
	std::set<std::pair<T, T>> ranges_;
	T TINF;

	// --- 内部ヘルパー ---
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
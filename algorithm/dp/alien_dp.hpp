/**
 * @file alien_dp.hpp
 * @brief Alien DP
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/algorithm/dp/alien_dp.hpp
 */
#ifndef KWM_T_ALGORITHM_ALIEN_DP_HPP
#define KWM_T_ALGORITHM_ALIEN_DP_HPP

#include <algorithm>
#include <cassert>

namespace kwm_t::algorithm::dp {

/**
 * @brief Alien DP における最適値と、そのときの個数を表す構造体。
 *
 * @tparam T 最適値の型。
 *
 * @param value
 *	ペナルティを含めた最適値。
 *
 * @param count
 *	その最適値を達成する選択個数。
 */
template <typename T>
struct AlienDPResult {
	T value;
	long long count;
};

/**
 * @brief Alien DP の最小化モード。
 *
 * solve(lambda) が
 *
 *	min_k { f(k) + lambda * k }
 *
 * を返す場合に使用する。
 */
struct AlienDPMinimize {
	/**
	 * @brief 2つの状態のうち、Alien DP において優れた方を返す。
	 *
	 * 最適値が同じ場合は count が小さい方を選ぶ。
	 *
	 * @tparam T 最適値の型。
	 *
	 * @param lhs 比較対象。
	 * @param rhs 比較対象。
	 * @return lhs が優れているか。
	 */
	template <typename T>
	static bool better(
		const AlienDPResult<T>& lhs,
		const AlienDPResult<T>& rhs
	) {
		if (lhs.value != rhs.value) {
			return lhs.value < rhs.value;
		}
		return lhs.count < rhs.count;
	}

	/**
	 * @brief ペナルティを取り除いて元の目的関数の値を復元する。
	 *
	 * solve(lambda) = min_k { f(k) + lambda * k } のとき、
	 *
	 *	solve(lambda) - lambda * target
	 *
	 * は f(target) 以下の値になる。
	 *
	 * @tparam T 最適値の型。
	 *
	 * @param res solve(lambda) の結果。
	 * @param lambda ペナルティ。
	 * @param target 求めたい個数。
	 * @return 復元した値。
	 */
	template <typename T>
	static T restore(
		const AlienDPResult<T>& res,
		T lambda,
		long long target
	) {
		return res.value - lambda * static_cast<T>(target);
	}

	/**
	 * @brief 2つの復元値から最終的な値を選ぶ。
	 *
	 * 復元値は f(target) 以下の下界なので、
	 * より大きい方を採用する。
	 *
	 * @tparam T 最適値の型。
	 *
	 * @param lhs 復元値。
	 * @param rhs 復元値。
	 * @return 最終的な値。
	 */
	template <typename T>
	static T select(T lhs, T rhs) {
		return std::max(lhs, rhs);
	}
};

/**
 * @brief Alien DP の最大化モード。
 *
 * solve(lambda) が
 *
 *	max_k { f(k) - lambda * k }
 *
 * を返す場合に使用する。
 */
struct AlienDPMaximize {
	/**
	 * @brief 2つの状態のうち、Alien DP において優れた方を返す。
	 *
	 * 最適値が同じ場合は count が大きい方を選ぶ。
	 *
	 * @tparam T 最適値の型。
	 *
	 * @param lhs 比較対象。
	 * @param rhs 比較対象。
	 * @return lhs が優れているか。
	 */
	template <typename T>
	static bool better(
		const AlienDPResult<T>& lhs,
		const AlienDPResult<T>& rhs
	) {
		if (lhs.value != rhs.value) {
			return lhs.value > rhs.value;
		}
		return lhs.count > rhs.count;
	}

	/**
	 * @brief ペナルティを取り除いて元の目的関数の値を復元する。
	 *
	 * solve(lambda) = max_k { f(k) - lambda * k } のとき、
	 *
	 *	solve(lambda) + lambda * target
	 *
	 * は f(target) 以上の値になる。
	 *
	 * @tparam T 最適値の型。
	 *
	 * @param res solve(lambda) の結果。
	 * @param lambda ペナルティ。
	 * @param target 求めたい個数。
	 * @return 復元した値。
	 */
	template <typename T>
	static T restore(
		const AlienDPResult<T>& res,
		T lambda,
		long long target
	) {
		return res.value + lambda * static_cast<T>(target);
	}

	/**
	 * @brief 2つの復元値から最終的な値を選ぶ。
	 *
	 * 復元値は f(target) 以上の上界なので、
	 * より小さい方を採用する。
	 *
	 * @tparam T 最適値の型。
	 *
	 * @param lhs 復元値。
	 * @param rhs 復元値。
	 * @return 最終的な値。
	 */
	template <typename T>
	static T select(T lhs, T rhs) {
		return std::min(lhs, rhs);
	}
};

/**
 * @brief Alien Trick によって指定個数の最適値を求める。
 *
 * solve(lambda) は、lambda に対して最適化した
 * AlienDPResult<T> を返す必要がある。
 *
 * 最小化の場合:
 *
 *	solve(lambda) = min_k { f(k) + lambda * k }
 *
 * 最大化の場合:
 *
 *	solve(lambda) = max_k { f(k) - lambda * k }
 *
 * とする。
 *
 * lambda_left では count >= target、
 * lambda_right では count <= target となる必要がある。
 *
 * @tparam Mode AlienDPMinimize または AlienDPMaximize。
 * @tparam T ペナルティおよび最適値の型。
 * @tparam F solve(lambda) の関数型。
 *
 * @param target 求めたい選択個数。
 * @param lambda_left 左端のペナルティ。
 * @param lambda_right 右端のペナルティ。
 * @param solve 指定したペナルティに対する最適化を行う関数。
 *
 * @return target 個選択したときの最適値。
 *
 * 制約 / 注意:
 * - lambda_left < lambda_right であること。
 * - solve(lambda_left).count >= target であること。
 * - solve(lambda_right).count <= target であること。
 * - lambda は整数であることを想定する。
 *
 * 使用例:
 *
 *	auto solve = [&](long long lambda)
 *		-> AlienDPResult<long long> {
 *		// ...
 *	};
 *
 *	long long ans = alien_dp<AlienDPMinimize>(
 *		target,
 *		-2000000001LL,
 *		2000000001LL,
 *		solve
 *	);
 *
 * verified:
 *  AlienDPMinimize
 *	https://atcoder.jp/contests/abc462/submissions/79685461
 *  AlienDPMaximize
 *  https://atcoder.jp/contests/abc218/submissions/79685258
 */
template <typename Mode, typename T, typename F>
T alien_dp(
	long long target,
	T lambda_left,
	T lambda_right,
	F solve
) {
	AlienDPResult<T> left = solve(lambda_left);
	AlienDPResult<T> right = solve(lambda_right);

	assert(right.count <= target && target <= left.count);

	while (lambda_left + 1 < lambda_right) {
		const T lambda =
			lambda_left + (lambda_right - lambda_left) / 2;

		AlienDPResult<T> mid = solve(lambda);

		if (mid.count > target) {
			lambda_left = lambda;
			left = mid;
		}
		else {
			lambda_right = lambda;
			right = mid;
		}
	}

	const T value_left =
		Mode::restore(left, lambda_left, target);

	const T value_right =
		Mode::restore(right, lambda_right, target);

	return Mode::select(value_left, value_right);
}

}  // namespace kwm_t::algorithm::dp

#endif  // KWM_T_ALGORITHM_ALIEN_DP_HPP
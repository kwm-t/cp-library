/**
 * @file slope_trick.hpp
 * @brief Slope Trick
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/datastructure/slope_trick.hpp
 */
#ifndef KWM_T_DATA_STRUCTURE_SLOPE_TRICK_HPP
#define KWM_T_DATA_STRUCTURE_SLOPE_TRICK_HPP

#include <queue>
#include <vector>
#include <algorithm>
#include <limits>

/**
 * @brief Slope Trick (最小値付き凸関数管理)
 *
 * 扱う関数:
 *   f(x) = min + Σ max(0, x - ai) + Σ max(0, bi - x)
 *
 * 計算量:
 *   各操作 O(log N)
 *
 * @tparam T 数値型 (int / long long など)
 *
 * 制約:
 *   - 加減算・比較が可能
 *   - overflowに注意
 * verified
 * 	https://atcoder.jp/contests/abc127/submissions/74799943
 * 	https://atcoder.jp/contests/abc217/submissions/74800013
 *  https://atcoder.jp/contests/abc330/submissions/74800034
 *  https://atcoder.jp/contests/arc070/submissions/74800068
 *  https://atcoder.jp/contests/dwango2016-prelims/submissions/74800108
 *  https://atcoder.jp/contests/kupc2016/submissions/74800146
 *  https://atcoder.jp/contests/utpc2012/submissions/74800222
 *  https://yukicoder.me/submissions/1156981
 */

namespace kwm_t::data_structure {

template<typename T>
struct SlopeTrick {
private:
	std::priority_queue<T> L;
	std::priority_queue<T, std::vector<T>, std::greater<T>> R;

	T min_f = 0;
	T addL = 0, addR = 0;

	static constexpr T INF = std::numeric_limits<T>::max() / 2;

	T topL() const { return L.empty() ? -INF : L.top() + addL; }
	T topR() const { return R.empty() ? INF : R.top() + addR; }

	void pushL_raw(T x) { L.push(x - addL); }
	void pushR_raw(T x) { R.push(x - addR); }

public:
	SlopeTrick() = default;

	/// max(0, x - a)
	void pushR(T a) {
		if (!L.empty()) {
			T l0 = topL();
			if (l0 > a) {
				min_f += l0 - a;
				L.pop();
				pushL_raw(a);
				pushR_raw(l0);
				return;
			}
		}
		pushR_raw(a);
	}

	/// max(0, a - x)
	void pushL(T a) {
		if (!R.empty()) {
			T r0 = topR();
			if (r0 < a) {
				min_f += a - r0;
				R.pop();
				pushR_raw(a);
				pushL_raw(r0);
				return;
			}
		}
		pushL_raw(a);
	}

	/// |x - a|
	void push(T a) {
		pushL(a);
		pushR(a);
	}

	/// 定数加算
	void add_const(T c) {
		min_f += c;
	}

	/// 最小値
	T get_min() const {
		return min_f;
	}

	/// 傾き0の区間
	std::pair<T, T> get_argmin_range() const {
		return { topL(), topR() };
	}

	/// x += shift
	void shift(T shift) {
		addL += shift;
		addR += shift;
	}

	// g(x) = min f(y), y ∈ [x - r, x - l]
	void shift(T l, T r) {
		addL += l;
		addR += r;
	}

	/// 左側累積min
	void prefix_min() {
		R = decltype(R)();
	}

	/// 右側累積min
	void suffix_min() {
		L = decltype(L)();
	}

	/// merge（destructive）
	void merge(SlopeTrick& other) {
		if (size() < other.size()) {
			swap(other);
		}

		while (!other.L.empty()) {
			pushL(other.topL());
			other.L.pop();
		}
		while (!other.R.empty()) {
			pushR(other.topR());
			other.R.pop();
		}
		min_f += other.min_f;
	}

	int size() const {
		return (int)(L.size() + R.size());
	}

	void swap(SlopeTrick& other) {
		std::swap(L, other.L);
		std::swap(R, other.R);
		std::swap(min_f, other.min_f);
		std::swap(addL, other.addL);
		std::swap(addR, other.addR);
	}

	// f(x) を計算（非破壊, O(N)）
	T query(T x) const {
		T res = min_f;

		auto tmpL = L;
		auto tmpR = R;

		while (!tmpL.empty()) {
			res += std::max((T)0, (tmpL.top() + addL) - x);
			tmpL.pop();
		}
		while (!tmpR.empty()) {
			res += std::max((T)0, x - (tmpR.top() + addR));
			tmpR.pop();
		}
		return res;
	}

	// 破壊的評価（定数倍が良い）
	T query_destructive(T x) {
		T res = min_f;

		while (!L.empty()) {
			res += std::max((T)0, topL() - x);
			L.pop();
		}
		while (!R.empty()) {
			res += std::max((T)0, x - topR());
			R.pop();
		}
		return res;
	}
};

} // namespace kwm_t::data_structure

#endif
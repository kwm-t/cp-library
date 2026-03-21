#ifndef KWM_T_UTILITY_CUMULATIVE_SUM_HPP
#define KWM_T_UTILITY_CUMULATIVE_SUM_HPP

#include <vector>
#include <cassert>

/**
 * @brief 累積和 (Prefix Sum)
 *
 * 配列 a に対して累積和を構築し、区間和を O(1) で取得する。
 *
 * 典型用途:
 *   - 区間和クエリ
 *   - 累積条件の判定（二分探索など）
 *   - imos法の補助
 *
 * 計算量:
 *   - 構築: O(N)
 *   - クエリ: O(1)
 *
 * @tparam T 加算・減算が可能な型
 *
 * 制約 / 注意:
 *   - T は +, - が定義されていること
 *   - オーバーフローに注意
 *
 * 使用例:
 *   vector<int> a = {1,2,3,4};
 *   utility::CumulativeSum<int> cs(a);
 *   cs.sum(1,3); // = 2+3
 *
 * verified:
 */
namespace kwm_t::utility {

template <class T>
struct CumulativeSum {
private:
	std::vector<T> s; // s[i] = a[0] + ... + a[i-1]

public:
	CumulativeSum() = default;

	explicit CumulativeSum(const std::vector<T>& a) {
		build(a);
	}

	void build(const std::vector<T>& a) {
		int n = (int)a.size();
		s.assign(n + 1, T{});
		for (int i = 0; i < n; i++) {
			s[i + 1] = s[i] + a[i];
		}
	}

	int size() const {
		return (int)s.size() - 1;
	}

	// [l, r)
	T sum(int l, int r) const {
		assert(0 <= l && l <= r && r <= size());
		return s[r] - s[l];
	}

	// [0, r)
	T prefix(int r) const {
		return sum(0, r);
	}

	// [l, n)
	T suffix(int l) const {
		return sum(l, size());
	}
};

} // namespace kwm_t::utility

#endif // KWM_T_UTILITY_CUMULATIVE_SUM_HPP
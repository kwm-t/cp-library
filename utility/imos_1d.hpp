/**
 * @file imos_1d.hpp
 * @brief 一次元 Imos 法
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/utility/imos_1d.hpp
 */
#ifndef KWM_T_UTILITY_IMOS_1D_HPP
#define KWM_T_UTILITY_IMOS_1D_HPP

#include <vector>
#include <cassert>

namespace kwm_t::utility {

/**
 * @brief 1次元いもす法（Difference Array / Imos）
 *
 * 典型用途:
 *   - 区間加算
 *   - 大量区間更新
 *   - 重なり数計算
 *
 * 計算量:
 *   - add: O(1)
 *   - build: O(N)
 *   - get: O(1)
 *
 * @tparam T
 *   加算・減算が定義されている型
 *
 * 制約 / 注意:
 *   - add(l, r, val) は
 *       半開区間 [l, r)
 *     に val を加算する
 *   - 0-indexed を前提とする
 *   - build() 後に実際の値が得られる
 *
 * 使用例:
 *   kwm_t::utility::Imos1D<int> imos(10);
 *
 *   // [2, 5) に +3
 *   imos.add(2, 5, 3);
 *
 *   // [0, 3) に +1
 *   imos.add(0, 3, 1);
 *
 *   imos.build();
 *
 *   int v = imos.get(2);
 *
 * verified:
 *   - 
 */
template <class T>
struct Imos1D {
private:
	std::vector<T> d;
	bool built = false;

public:
	Imos1D() = default;

	explicit Imos1D(int n) {
		init(n);
	}

	void init(int n) {
		assert(n >= 0);

		d.assign(n + 1, T{});
		built = false;
	}

	int size() const {
		return (int)d.size() - 1;
	}

	/**
	 * @brief 区間 [l,r) に val を加算
	 */
	void add(int l, int r, T val) {
		assert(!built);

		assert(0 <= l && l <= r && r <= size());

		d[l] += val;
		d[r] -= val;
	}

	/**
	 * @brief 累積和を取って復元
	 */
	void build() {
		assert(!built);

		int n = size();

		for (int i = 0; i < n; i++)
			d[i + 1] += d[i];

		built = true;
	}

	/**
	 * @brief build 後の値を取得
	 */
	T get(int i) const {
		assert(built);

		assert(0 <= i && i < size());

		return d[i];
	}

	/**
	 * @brief build 後の配列を取得
	 */
	const std::vector<T>& data() const {
		assert(built);
		return d;
	}
};

} // namespace kwm_t::utility

#endif // KWM_T_UTILITY_IMOS_1D_HPP
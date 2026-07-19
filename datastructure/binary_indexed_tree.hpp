#ifndef KWM_T_DATA_STRUCT_BINARY_INDEXED_TREE_HPP
#define KWM_T_DATA_STRUCT_BINARY_INDEXED_TREE_HPP

#include <cassert>
#include <bit>
#include <vector>

namespace kwm_t::data_structure {

/**
 * @brief Binary Indexed Tree (Fenwick Tree)
 *
 * 可換群（加算・減算可能な型）に対する一点更新・累積和データ構造。
 *
 * 典型用途:
 * - 数列の一点加算・区間和取得
 * - 座標圧縮と組み合わせた転倒数
 * - order statistics (lower_bound / upper_bound)
 * - 累積個数管理
 *
 * 計算量:
 * - 構築: O(n)
 * - apply: O(log n)
 * - prod: O(log n)
 * - lower_bound / upper_bound: O(log n)
 *
 * @tparam T
 * 加算・減算可能な型。
 *
 * 必要な演算:
 * @code
 * T();
 * T& T::operator+=(const T&);
 * T& T::operator-=(const T&);
 * friend T operator-(T, const T&);
 * bool operator<(const T&, const T&);
 * bool operator<=(const T&, const T&);
 * @endcode
 *
 * 例えば以下のような型が利用可能。
 *
 * @code
 * struct Data {
 *     long long cnt, sum;
 *
 *     Data() : cnt(0), sum(0) {}
 *     Data(long long c, long long s) : cnt(c), sum(s) {}
 *
 *     Data& operator+=(const Data& x) {
 *         cnt += x.cnt;
 *         sum += x.sum;
 *         return *this;
 *     }
 *
 *     Data& operator-=(const Data& x) {
 *         cnt -= x.cnt;
 *         sum -= x.sum;
 *         return *this;
 *     }
 *
 *     friend Data operator-(Data l, const Data& r) {
 *         return l -= r;
 *     }
 *
 *     friend bool operator<(const Data& a, const Data& b) {
 *         return a.cnt < b.cnt;
 *     }
 *
 *     friend bool operator<=(const Data& a, const Data& b) {
 *         return a.cnt <= b.cnt;
 *     }
 * };
 * @endcode
 *
 * 制約 / 注意:
 * - 添字は 0-indexed。
 * - lower_bound / upper_bound は prefix 和が単調増加であることを仮定する。
 * - lower_bound(x):
 *     最小の i で prod(i + 1) >= x となる i を返す。
 * - upper_bound(x):
 *     最小の i で prod(i + 1) > x となる i を返す。
 *
 * 使用例:
 * @code
 * BinaryIndexedTree<long long> bit(n);
 * bit.apply(i, x);
 * auto s = bit.prod(l, r);
 * @endcode
 *
 * verified:
 * - https://atcoder.jp/contests/abc467/submissions/77660923
 */
template <class T>
class BinaryIndexedTree {
private:
	int n = 0;
	std::vector<T> data;

public:
	BinaryIndexedTree() = default;

	explicit BinaryIndexedTree(int n)
		: n(n), data(n + 1, T()) {}

	explicit BinaryIndexedTree(const std::vector<T>& v)
		: BinaryIndexedTree(static_cast<int>(v.size())) {
		build(v);
	}

	/**
	 * @brief O(n) 構築
	 */
	void build(const std::vector<T>& v) {
		assert(n == static_cast<int>(v.size()));
		for (int i = 1; i <= n; i++) data[i] = v[i - 1];
		for (int i = 1; i <= n; i++) {
			int j = i + (i & -i);
			if (j <= n) data[j] += data[i];
		}
	}

	/**
	 * @brief a[k] += x
	 */
	void apply(int k, const T& x) {
		for (++k; k <= n; k += k & -k) data[k] += x;
	}

	/**
	 * @brief [0,r) の総和
	 */
	T prod(int r) const {
		T ret = T();
		for (; r > 0; r -= r & -r) ret += data[r];
		return ret;
	}

	/**
	 * @brief [l,r) の総和
	 */
	T prod(int l, int r) const {
		return prod(r) - prod(l);
	}

	/**
	 * @brief 最小の i で prod(i+1) >= x
	 *
	 * prefix 和が単調増加であることを仮定。
	 */
	int lower_bound(T x) const {
		int i = 0;
		for (int k = 1 << std::bit_width(static_cast<unsigned>(n)); k > 0; k >>= 1) {
			if (i + k <= n && data[i + k] < x) {
				x -= data[i + k];
				i += k;
			}
		}
		return i;
	}

	/**
	 * @brief 最小の i で prod(i+1) > x
	 *
	 * prefix 和が単調増加であることを仮定。
	 */
	int upper_bound(T x) const {
		int i = 0;
		for (int k = 1 << std::bit_width(static_cast<unsigned>(n)); k > 0; k >>= 1) {
			if (i + k <= n && data[i + k] <= x) {
				x -= data[i + k];
				i += k;
			}
		}
		return i;
	}

	int size() const {
		return n;
	}
};

} // namespace kwm_t::data_struct

#endif // KWM_T_DATA_STRUCT_BINARY_INDEXED_TREE_HPP
#ifndef KWM_T_DATASTRUCTURE_SQRT_DECOMP_HPP
#define KWM_T_DATASTRUCTURE_SQRT_DECOMP_HPP

#include <algorithm>
#include <cmath>
#include <vector>

/**
 * @brief 平方分割（区間取得）
 *
 * 区間クエリ（非更新 or 点更新）を平方分割で処理する。
 *
 * @tparam T 値型
 * @tparam op モノイド演算 (T, T) -> T
 * @tparam e 単位元
 *
 * 制約:
 *   - op は結合則を満たす
 *   - e は op の単位元
 *
 * 計算量:
 *   - 構築: O(N)
 *   - クエリ: O(sqrt(N))
 *   - 点更新: O(sqrt(N))
 */
namespace kwm_t::datastructure {

template <class T, T (*op)(T, T), T e>
class SqrtDecomposition {
private:
	int n;
	int block_size;
	int block_cnt;

	std::vector<T> data;
	std::vector<T> block;

public:
	SqrtDecomposition(int n_)
		: n(n_) {
		block_size = std::max(1, (int)std::sqrt(n));
		block_cnt = (n + block_size - 1) / block_size;

		data.assign(n, e);
		block.assign(block_cnt, e);
	}

	SqrtDecomposition(const std::vector<T>& v)
		: SqrtDecomposition((int)v.size()) {
		for (int i = 0; i < n; ++i) {
			data[i] = v[i];
			block[i / block_size] = op(block[i / block_size], v[i]);
		}
	}

	void set(int p, T x) {
		int b = p / block_size;

		data[p] = x;

		block[b] = e;
		int l = b * block_size;
		int r = std::min(n, l + block_size);

		for (int i = l; i < r; ++i) {
			block[b] = op(block[b], data[i]);
		}
	}

	T prod(int l, int r) const {
		T ret = e;

		int bl = l / block_size;
		int br = r / block_size;

		if (bl == br) {
			for (int i = l; i < r; ++i) {
				ret = op(ret, data[i]);
			}
			return ret;
		}

		int lend = (bl + 1) * block_size;
		for (int i = l; i < lend; ++i) {
			ret = op(ret, data[i]);
		}

		for (int b = bl + 1; b < br; ++b) {
			ret = op(ret, block[b]);
		}

		int rbegin = br * block_size;
		for (int i = rbegin; i < r; ++i) {
			ret = op(ret, data[i]);
		}

		return ret;
	}

	T all_prod() const {
		return prod(0, n);
	}
};

} // namespace kwm_t::datastructure

#endif // KWM_T_DATASTRUCTURE_SQRT_DECOMP_HPP
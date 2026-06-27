#ifndef KWM_T_DATASTRUCTURE_SQRT_DECOMP_HPP
#define KWM_T_DATASTRUCTURE_SQRT_DECOMP_HPP

#include <vector>

/**
 * @brief 平方分割（区間取得）
 *
 * 区間クエリ（非更新 or 点更新）を sqrt 分割で処理
 *
 * 典型用途:
 *   - Range Sum / Min / Max
 *   - 更新が軽めの問題
 *
 * 計算量:
 *   - 構築: O(N)
 *   - クエリ: O(√N)
 *   - 点更新: O(√N)
 *
 * @tparam T 値型
 * @tparam Op モノイド演算 (T, T) -> T
 *
 * 制約 / 注意:
 *   - op は結合則を満たす
 *   - e は単位元
 *
 * verified:
 */
namespace kwm_t::datastructure {

template <class T, class Op>
class SqrtDecomposition {
private:
	int n;
	int block_size;
	int block_cnt;

	std::vector<T> data;
	std::vector<T> block;

	Op op;
	T e;

public:
	SqrtDecomposition(int n_, Op op_, T e_)
		: n(n_), op(op_), e(e_) {
		block_size = std::max(1, (int)std::sqrt(n));
		block_cnt = (n + block_size - 1) / block_size;
		data.assign(n, e);
		block.assign(block_cnt, e);
	}

	SqrtDecomposition(const std::vector<T>& v, Op op_, T e_)
		: SqrtDecomposition((int)v.size(), op_, e_) {
		for (int i = 0; i < n; ++i) {
			data[i] = v[i];
			block[i / block_size] = op(block[i / block_size], v[i]);
		}
	}

	// 点更新
	void set(int i, T x) {
		int b = i / block_size;

		// 再計算
		data[i] = x;
		block[b] = e;
		int l = b * block_size;
		int r = std::min(n, l + block_size);
		for (int j = l; j < r; ++j) {
			block[b] = op(block[b], data[j]);
		}
	}

	// 区間取得 [l, r)
	T prod(int l, int r) const {
		T ret = e;

		int bl = l / block_size;
		int br = r / block_size;

		if (bl == br) {
			for (int i = l; i < r; ++i)
				ret = op(ret, data[i]);
			return ret;
		}

		int lend = (bl + 1) * block_size;
		for (int i = l; i < lend; ++i)
			ret = op(ret, data[i]);

		for (int b = bl + 1; b < br; ++b)
			ret = op(ret, block[b]);

		int rbegin = br * block_size;
		for (int i = rbegin; i < r; ++i)
			ret = op(ret, data[i]);

		return ret;
	}
};

} // namespace kwm_t::datastructure

#endif // KWM_T_DATASTRUCTURE_SQRT_DECOMP_HPP
/**
 * @file wavelet_matrix.hpp
 * @brief Wavelet Matrix
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/datastructure/wavelet/wavelet_matrix.hpp
 */

#ifndef KWM_T_DATA_STRUCTURE_WAVELET_WAVELET_MATRIX_RECTANGLE_SUM_HPP
#define KWM_T_DATA_STRUCTURE_WAVELET_WAVELET_MATRIX_RECTANGLE_SUM_HPP

#include <vector>
#include <algorithm>
#include <numeric>
#include <tuple>
#include <cassert>

#include "../bit_vector/succinct_indexable_dictionary.hpp"
#include "../binary_indexed_tree.hpp"

namespace kwm_t::data_structure::wavelet {

using kwm_t::data_structure::bit_vector::SuccinctIndexableDictionary;

/**
 * @brief Wavelet Matrix Rectangle Sum
 * verified:
 * - https://judge.yosupo.jp/submission/365586
 * - https://atcoder.jp/contests/abc405/submissions/74813191
 * - https://atcoder.jp/contests/abc467/submissions/77660923
 */
template <typename T, int MAXLOG, typename D>
struct WaveletMatrixRectangleSum {
	size_t length;
	SuccinctIndexableDictionary matrix[MAXLOG];
	kwm_t::data_structure::BinaryIndexedTree<D> ds[MAXLOG];
	vector<T> v;
	int mid[MAXLOG];

	WaveletMatrixRectangleSum() = default;

	// constructor
	WaveletMatrixRectangleSum(const std::vector<T>& v, const std::vector<D>& d)
		: length(v.size()), v(v) {

		assert(v.size() == d.size());

		std::vector<int> l(length), r(length), ord(length);
		std::iota(ord.begin(), ord.end(), 0);

		for (int level = MAXLOG - 1; level >= 0; level--) {
			matrix[level] = SuccinctIndexableDictionary(length + 1);

			int left = 0, right = 0;

			for (int i = 0; i < (int)length; i++) {
				if ((v[ord[i]] >> level) & 1) {
					matrix[level].set(i);
					r[right++] = ord[i];
				}
				else {
					l[left++] = ord[i];
				}
			}

			mid[level] = left;
			matrix[level].build();

			ord.swap(l);
			for (int i = 0; i < right; i++) {
				ord[left + i] = r[i];
			}

			vector<D> dd(length);
			for (int i = 0; i < (int)length; i++) {
				dd[i] = d[ord[i]];
			}
			ds[level] = BinaryIndexedTree<D>(dd);
		}
	}

	// 区間遷移
	std::pair<int, int> succ(bool f, int l, int r, int level) const {
		return {
			matrix[level].rank(f, l) + mid[level] * f,
			matrix[level].rank(f, r) + mid[level] * f
		};
	}

	// [l,r) × (-inf, upper) の総和
	D rect_sum(int l, int r, T upper) const {
		D ret = D();
		for (int level = MAXLOG - 1; level >= 0; level--) {
			bool f = (upper >> level) & 1;
			if (f) {
				auto nxt = succ(false, l, r, level);
				ret += ds[level].prod(nxt.first, nxt.second);
				l = l - nxt.first + mid[level];
				r = r - nxt.second + mid[level];
			}
			else {
				std::tie(l, r) = succ(f, l, r, level);
			}
		}
		return ret;
	}

	// [l,r) × [lower, upper) の総和
	D rect_sum(int l, int r, T lower, T upper) const {
		return rect_sum(l, r, upper) - rect_sum(l, r, lower);
	}

	// 点加算
	void point_add(int k, const D& x) {
		auto& y = v[k];
		for (int level = MAXLOG - 1; level >= 0; level--) {
			bool f = ((y >> level) & 1);
			k = matrix[level].rank(f, k) + mid[level] * f;
			ds[level].apply(k, x);
		}
	}
	
	// 点加算
	void point_add(int k, const D& x) {
		auto& y = v[k];
		for (int level = MAXLOG - 1; level >= 0; level--) {
			bool f = ((y >> level) & 1);
			k = matrix[level].rank(f, k) + mid[level] * f;
			ds[level].apply(k, x);
		}
	}

	// 二分探索
	template <class F>
	int max_right(int l, int r, F f) const {
		int value = 0;
		for (int level = MAXLOG - 1; level >= 0; --level) {
			auto right = succ(true, l, r, level);
			D d = ds[level].prod(right.first, right.second);

			if (f(d)) {
				value |= 1 << level;
				std::tie(l, r) = right;
			}
			else {
				std::tie(l, r) = succ(false, l, r, level);
			}
		}
		return value;
	}
};

} // namespace kwm_t::data_structure::wavelet

#endif
#ifndef KWM_T_DATA_STRUCTURE_WAVELET_WAVELET_MATRIX_RECTANGLE_SUM_HPP
#define KWM_T_DATA_STRUCTURE_WAVELET_WAVELET_MATRIX_RECTANGLE_SUM_HPP

#include <vector>
#include <algorithm>
#include <numeric>
#include <tuple>
#include <cassert>

#include "../bit_vector/succinct_indexable_dictionary.hpp"

namespace kwm_t::data_structure::wavelet {

using kwm_t::data_structure::bit_vector::SuccinctIndexableDictionary;

// Wavelet Matrix Rectangle Sum
// verified
// https://judge.yosupo.jp/submission/365586
// https://atcoder.jp/contests/abc405/submissions/74813191
template <typename T, int MAXLOG, typename D>
struct WaveletMatrixRectangleSum {
	size_t length;
	SuccinctIndexableDictionary matrix[MAXLOG];
	std::vector<D> ds[MAXLOG];
	int mid[MAXLOG];

	WaveletMatrixRectangleSum() = default;

	// constructor
	WaveletMatrixRectangleSum(const std::vector<T>& v, const std::vector<D>& d)
		: length(v.size()) {

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
				} else {
					l[left++] = ord[i];
				}
			}

			mid[level] = left;
			matrix[level].build();

			ord.swap(l);
			for (int i = 0; i < right; i++) {
				ord[left + i] = r[i];
			}

			// prefix sum (このレベルの順序に対応)
			ds[level].resize(length + 1);
			ds[level][0] = D();
			for (int i = 0; i < (int)length; i++) {
				ds[level][i + 1] = ds[level][i] + d[ord[i]];
			}
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
		D ret = 0;
		for (int level = MAXLOG - 1; level >= 0; level--) {
			bool f = (upper >> level) & 1;
			if (f) {
				ret += ds[level][matrix[level].rank(false, r)]
					- ds[level][matrix[level].rank(false, l)];
			}
			std::tie(l, r) = succ(f, l, r, level);
		}
		return ret;
	}

	// [l,r) × [lower, upper) の総和
	D rect_sum(int l, int r, T lower, T upper) const {
		return rect_sum(l, r, upper) - rect_sum(l, r, lower);
	}
};

} // namespace

#endif
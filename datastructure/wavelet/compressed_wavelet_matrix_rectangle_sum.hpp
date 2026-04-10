#ifndef KWM_T_DATA_STRUCTURE_WAVELET_COMPRESSED_WAVELET_MATRIX_RECTANGLE_SUM_HPP
#define KWM_T_DATA_STRUCTURE_WAVELET_COMPRESSED_WAVELET_MATRIX_RECTANGLE_SUM_HPP

#include <vector>
#include <algorithm>

#include "wavelet_matrix_rectangle_sum.hpp"

namespace kwm_t::data_structure::wavelet {

// 座標圧縮付き Rectangle Sum
// verified
// https://judge.yosupo.jp/submission/365586
// https://atcoder.jp/contests/abc405/submissions/74813191
template <typename T, int MAXLOG, typename D>
struct CompressedWaveletMatrixRectangleSum {
	WaveletMatrixRectangleSum<int, MAXLOG, D> mat;
	std::vector<T> ys;

	// constructor
	CompressedWaveletMatrixRectangleSum(const std::vector<T>& v, const std::vector<D>& d)
		: ys(v) {

		std::sort(ys.begin(), ys.end());
		ys.erase(std::unique(ys.begin(), ys.end()), ys.end());

		std::vector<int> t(v.size());
		for (int i = 0; i < (int)v.size(); i++) {
			t[i] = get(v[i]);
		}

		mat = WaveletMatrixRectangleSum<int, MAXLOG, D>(t, d);
	}

	// 圧縮値取得
	int get(const T& x) const {
		return std::lower_bound(ys.begin(), ys.end(), x) - ys.begin();
	}

	// (-inf, upper)
	D rect_sum(int l, int r, T upper) const {
		return mat.rect_sum(l, r, get(upper));
	}

	// [lower, upper)
	D rect_sum(int l, int r, T lower, T upper) const {
		return mat.rect_sum(l, r, get(lower), get(upper));
	}
};

} // namespace

#endif
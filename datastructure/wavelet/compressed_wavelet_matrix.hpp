#ifndef KWM_T_DATA_STRUCTURE_WAVELET_COMPRESSED_WAVELET_MATRIX_HPP
#define KWM_T_DATA_STRUCTURE_WAVELET_COMPRESSED_WAVELET_MATRIX_HPP

#include <vector>
#include <algorithm>
#include "wavelet_matrix.hpp"

namespace kwm_t::data_structure::wavelet {

// 座標圧縮付きWavelet Matrix
// verified
// https://judge.yosupo.jp/submission/365567
template< typename T, int MAXLOG >
struct CompressedWaveletMatrix {
	WaveletMatrix<int, MAXLOG> mat;
	std::vector<T> ys;

	// constructor
	CompressedWaveletMatrix(const std::vector<T>& v) : ys(v) {
		std::sort(ys.begin(), ys.end());
		ys.erase(std::unique(ys.begin(), ys.end()), ys.end());

		std::vector<int> t(v.size());
		for (int i = 0; i < (int)v.size(); i++) {
			t[i] = get(v[i]);
		}
		mat = WaveletMatrix<int, MAXLOG>(t);
	}

	// 圧縮値取得
	int get(const T& x) const {
		return std::lower_bound(ys.begin(), ys.end(), x) - ys.begin();
	}

	// v[k]取得
	T access(int k) const {
		return ys[mat.access(k)];
	}

	T operator[](const int& k) const {
		return access(k);
	}

	// [0,r) に含まれるxの数
	int rank(const T& x, int r) const {
		auto pos = get(x);
		if (pos == (int)ys.size() || ys[pos] != x) return 0;
		return mat.rank(pos, r);
	}

	// [l,r) のk番目に小さい
	T kth_smallest(int l, int r, int k) const {
		return ys[mat.kth_smallest(l, r, k)];
	}

	// [l,r) のk番目に大きい
	T kth_largest(int l, int r, int k) const {
		return ys[mat.kth_largest(l, r, k)];
	}

	// upper未満の個数
	int range_freq(int l, int r, T upper) const {
		return mat.range_freq(l, r, get(upper));
	}

	// 区間の個数
	int range_freq(int l, int r, T lower, T upper) const {
		return mat.range_freq(l, r, get(lower), get(upper));
	}

	// upper未満最大
	T prev_value(int l, int r, T upper) const {
		auto ret = mat.prev_value(l, r, get(upper));
		return ret == -1 ? T(-1) : ys[ret];
	}

	// lower以上最小
	T next_value(int l, int r, T lower) const {
		auto ret = mat.next_value(l, r, get(lower));
		return ret == -1 ? T(-1) : ys[ret];
	}
};

} // namespace

#endif
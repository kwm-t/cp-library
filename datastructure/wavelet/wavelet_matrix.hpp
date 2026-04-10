#ifndef KWM_T_DATA_STRUCTURE_WAVELET_WAVELET_MATRIX_HPP
#define KWM_T_DATA_STRUCTURE_WAVELET_WAVELET_MATRIX_HPP

#include <vector>
#include <tuple>
#include <cassert>
#include "../bit_vector/succinct_indexable_dictionary.hpp"

namespace kwm_t::data_structure::wavelet {

using kwm_t::data_structure::bit_vector::SuccinctIndexableDictionary;

// Wavelet Matrix
// verified
// https://judge.yosupo.jp/submission/365567
template< typename T, int MAXLOG >
struct WaveletMatrix {
	int length;
	std::vector<SuccinctIndexableDictionary> matrix;
	std::vector<int> mid;

	WaveletMatrix() = default;

	// constructor
	WaveletMatrix(std::vector<T> v) : length(v.size()) {
		matrix.resize(MAXLOG);
		mid.resize(MAXLOG);

		std::vector<T> l(length), r(length);

		for (int i = MAXLOG - 1; i >= 0; --i) {
			matrix[i] = SuccinctIndexableDictionary(length + 1);
			int left = 0, right = 0;

			for (int j = 0; j < length; ++j) {
				if ((v[j] >> i) & 1) {
					matrix[i].set(j);
					r[right++] = v[j];
				}
				else {
					l[left++] = v[j];
				}
			}

			mid[i] = left;
			matrix[i].build();

			for (int j = 0; j < right; ++j) {
				l[left + j] = r[j];
			}
			v.swap(l);
		}
	}

	// v[k]を取得
	T access(int k) const {
		T ret = 0;
		for (int i = MAXLOG - 1; i >= 0; --i) {
			bool f = matrix[i][k];
			if (f) ret |= T(1) << i;
			k = matrix[i].rank(f, k) + mid[i] * f;
		}
		return ret;
	}

	T operator[](const int& k) const {
		return access(k);
	}

	// 区間遷移
	std::pair<int,int> succ(bool f, int l, int r, int level) const {
		return {
			matrix[level].rank(f, l) + mid[level] * f,
			matrix[level].rank(f, r) + mid[level] * f
		};
	}

	// [0,r) に含まれるxの数
	int rank(const T& x, int r) const {
		int l = 0;
		for (int i = MAXLOG - 1; i >= 0; --i) {
			std::tie(l, r) = succ((x >> i) & 1, l, r, i);
		}
		return r - l;
	}

	// [l,r) のk番目に小さい値
	T kth_smallest(int l, int r, int k) const {
		assert(0 <= k && k < r - l);
		T ret = 0;

		for (int i = MAXLOG - 1; i >= 0; --i) {
			int cnt = matrix[i].rank(false, r) - matrix[i].rank(false, l);
			bool f = (cnt <= k);
			if (f) {
				ret |= T(1) << i;
				k -= cnt;
			}
			std::tie(l, r) = succ(f, l, r, i);
		}
		return ret;
	}

	// [l,r) のk番目に大きい値
	T kth_largest(int l, int r, int k) const {
		return kth_smallest(l, r, r - l - k - 1);
	}

	// [l,r) の v[i] < upper の個数
	int range_freq(int l, int r, T upper) const {
		int ret = 0;
		for (int i = MAXLOG - 1; i >= 0; --i) {
			bool f = (upper >> i) & 1;
			if (f) {
				ret += matrix[i].rank(false, r) - matrix[i].rank(false, l);
			}
			std::tie(l, r) = succ(f, l, r, i);
		}
		return ret;
	}

	// [l,r) の lower <= v[i] < upper の個数
	int range_freq(int l, int r, T lower, T upper) const {
		return range_freq(l, r, upper) - range_freq(l, r, lower);
	}

	// upper未満の最大値
	T prev_value(int l, int r, T upper) const {
		int cnt = range_freq(l, r, upper);
		return cnt == 0 ? T(-1) : kth_smallest(l, r, cnt - 1);
	}

	// lower以上の最小値
	T next_value(int l, int r, T lower) const {
		int cnt = range_freq(l, r, lower);
		return cnt == r - l ? T(-1) : kth_smallest(l, r, cnt);
	}
};

} // namespace

#endif
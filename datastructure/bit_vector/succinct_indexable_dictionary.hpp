#ifndef KWM_T_DATA_STRUCTURE_BIT_VECTOR_SUCCINCT_INDEXABLE_DICTIONARY_HPP
#define KWM_T_DATA_STRUCTURE_BIT_VECTOR_SUCCINCT_INDEXABLE_DICTIONARY_HPP

#include <vector>

namespace kwm_t::data_structure::bit_vector {

// 完備辞書（bit vector + rank + select）
// verified
// https://judge.yosupo.jp/submission/365567
struct SuccinctIndexableDictionary {
	int length, blocks;
	std::vector<unsigned> bit, sum;

	SuccinctIndexableDictionary() = default;

	// constructor（長さnで初期化）
	SuccinctIndexableDictionary(int _length)
		: length(_length), blocks((length + 31) >> 5) {
		bit.assign(blocks + 1, 0U);
		sum.assign(blocks + 1, 0U);
	}

	// k番目にbitをsetする
	void set(int k) {
		bit[k >> 5] |= 1U << (k & 31);
	}

	// set後に呼ぶ（prefix sum構築）
	void build() {
		sum[0] = 0U;
		for (int i = 0; i < blocks; ++i) {
			sum[i + 1] = sum[i] + __builtin_popcount(bit[i]);
		}
	}

	// k番目のbitを取得
	bool operator[](int k) const {
		return (bit[k >> 5] >> (k & 31)) & 1;
	}

	// [0,k) の1の個数
	int rank(int k) const {
		return sum[k >> 5] +
			__builtin_popcount(bit[k >> 5] & ((1U << (k & 31)) - 1));
	}

	// [0,k) のvalの個数
	int rank(bool val, int k) const {
		return val ? rank(k) : k - rank(k);
	}

	// select: n番目の1の位置（0-indexed）
	int select(int n) const {
		if (rank(length) <= n) return -1;
		int ng = length + 1, ok = 0;
		while (ng - ok > 1) {
			int mid = (ng + ok) >> 1;
			if (rank(mid) <= n) ok = mid;
			else ng = mid;
		}
		return ok;
	}
};

} // namespace

#endif
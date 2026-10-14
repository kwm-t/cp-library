/**
 * @file dynamic_bitset.hpp
 * @brief 動的ビット集合
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/datastructure/dynamic_bitset.hpp
 */
#ifndef KWM_T_DATA_STRUCTURE_DYNAMIC_BITSET_HPP
#define KWM_T_DATA_STRUCTURE_DYNAMIC_BITSET_HPP

#include <vector>
#include <cstddef>
#include <iostream>
#include <algorithm>

namespace kwm_t::data_structure {

/**
 * @brief 動的bitset（簡易版）
 *
 * 典型用途:
 *   - ビット集合管理
 *   - XOR/AND/OR集合演算
 *   - 軽量set代替
 *
 * 計算量:
 *   - set/get: O(1)
 *   - bitwise: O(N / 64)
 *   - count: O(N / 64)
 */
class DynamicBitset {
public:
	using ull = unsigned long long;

private:
	static constexpr size_t LG = 6;
	static constexpr size_t W = 1ULL << LG;     // 64
	static constexpr size_t MASK = W - 1;

	size_t n;
	std::vector<ull> bit;

public:
	DynamicBitset() : DynamicBitset(0) {}

	explicit DynamicBitset(size_t n)
		: n(n), bit((n + W - 1) / W, 0) {}

	// bit取得
	bool get(size_t pos) const {
		size_t i = pos >> LG;
		size_t j = pos & MASK;
		return (bit[i] >> j) & 1ULL;
	}

	// bitセット
	void set(size_t pos, bool val = true) {
		size_t i = pos >> LG;
		size_t j = pos & MASK;

		if (val) {
			bit[i] |= (1ULL << j);
		} else {
			bit[i] &= ~(1ULL << j);
		}
	}

	void reset(size_t pos) {
		set(pos, false);
	}

	// popcount
	size_t count() const {
		size_t ret = 0;
		for (ull x : bit) {
			ret += __builtin_popcountll(x);
		}
		return ret;
	}

	// debug
	void debug() const {
		for (size_t i = 0; i < bit.size(); i++) {
			std::cerr << i << " " << bit[i] << "\n";
		}
	}

	// OR
	DynamicBitset& operator|=(const DynamicBitset& rhs) {
		ensure(rhs.bit.size());
		for (size_t i = 0; i < rhs.bit.size(); i++) {
			bit[i] |= rhs.bit[i];
		}
		return *this;
	}

	// AND
	DynamicBitset& operator&=(const DynamicBitset& rhs) {
		ensure(rhs.bit.size());
		for (size_t i = 0; i < rhs.bit.size(); i++) {
			bit[i] &= rhs.bit[i];
		}
		return *this;
	}

	// XOR
	DynamicBitset& operator^=(const DynamicBitset& rhs) {
		ensure(rhs.bit.size());
		for (size_t i = 0; i < rhs.bit.size(); i++) {
			bit[i] ^= rhs.bit[i];
		}
		return *this;
	}

private:
	void ensure(size_t sz) {
		if (bit.size() < sz) {
			bit.resize(sz, 0);
		}
	}
};

} // namespace kwm_t::data_structure

#endif // KWM_T_DATA_STRUCTURE_DYNAMIC_BITSET_HPP
#ifndef KWM_T_MATH_CONVOLUTION_INTERNAL_WALSH_HADAMARD_HPP
#define KWM_T_MATH_CONVOLUTION_INTERNAL_WALSH_HADAMARD_HPP

#include <vector>
#include <cassert>

/**
 * @brief Walsh-Hadamard Transform（XOR / AND / OR）
 *
 * 典型用途:
 *   - XOR / AND / OR 畳み込み
 *
 * 計算量:
 *   O(n log n)
 *
 * 制約 / 注意:
 *   - サイズは 2^k
 *
 * verified:
 */
namespace kwm_t::math::convolution::internal {
template <typename T, typename Op>
void pair_transform(std::vector<T>& f, Op op) {
	int n = (int)f.size();
	assert((n & (n - 1)) == 0);

	for (int len = 1; len < n; len <<= 1) {
		for (int i = 0; i < n; i += len << 1) {
			for (int j = 0; j < len; ++j) {
				op(f[i + j], f[i + j + len]);
			}
		}
	}
}

// XOR
template <typename T>
void xor_transform(std::vector<T>& f) {
	pair_transform(f, [](T& x, T& y) {
		T a = x;
		T b = y;
		x = a + b;
		y = a - b;
	});
}

template <typename T>
void xor_transform_inv(std::vector<T>& f) {
	xor_transform(f);
	T inv_n = T(1) / T(f.size());
	for (auto& x : f) x *= inv_n;
}

// OR
template <typename T>
void or_transform(std::vector<T>& f) {
	pair_transform(f, [](T& x, T& y) {
		y += x;
	});
}

template <typename T>
void or_transform_inv(std::vector<T>& f) {
	pair_transform(f, [](T& x, T& y) {
		y -= x;
	});
}

// AND
template <typename T>
void and_transform(std::vector<T>& f) {
	pair_transform(f, [](T& x, T& y) {
		x += y;
	});
}

template <typename T>
void and_transform_inv(std::vector<T>& f) {
	pair_transform(f, [](T& x, T& y) {
		x -= y;
	});
}

}

#endif // KWM_T_MATH_CONVOLUTION_INTERNAL_WALSH_HADAMARD_HPP
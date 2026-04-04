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

// XOR
template <typename T>
void xor_transform(std::vector<T>& f) {
	int n = (int)f.size();
	assert((n & (n - 1)) == 0);

	for (int len = 1; len < n; len <<= 1) {
		for (int i = 0; i < n; i += len << 1) {
			for (int j = 0; j < len; ++j) {
				T x = f[i + j];
				T y = f[i + j + len];
				f[i + j] = x + y;
				f[i + j + len] = x - y;
			}
		}
	}
}

template <typename T>
void xor_transform_inv(std::vector<T>& f) {
	int n = (int)f.size();
	xor_transform(f);
	T inv_n = T(1) / T(n);
	for (auto& x : f) x *= inv_n;
}

// OR
template <typename T>
void or_transform(std::vector<T>& f) {
	int n = (int)f.size();
	assert((n & (n - 1)) == 0);

	for (int len = 1; len < n; len <<= 1) {
		for (int i = 0; i < n; i += len << 1) {
			for (int j = 0; j < len; ++j) {
				f[i + j + len] += f[i + j];
			}
		}
	}
}

template <typename T>
void or_transform_inv(std::vector<T>& f) {
	int n = (int)f.size();

	for (int len = 1; len < n; len <<= 1) {
		for (int i = 0; i < n; i += len << 1) {
			for (int j = 0; j < len; ++j) {
				f[i + j + len] -= f[i + j];
			}
		}
	}
}

// AND
template <typename T>
void and_transform(std::vector<T>& f) {
	int n = (int)f.size();
	assert((n & (n - 1)) == 0);

	for (int len = 1; len < n; len <<= 1) {
		for (int i = 0; i < n; i += len << 1) {
			for (int j = 0; j < len; ++j) {
				f[i + j] += f[i + j + len];
			}
		}
	}
}

template <typename T>
void and_transform_inv(std::vector<T>& f) {
	int n = (int)f.size();

	for (int len = 1; len < n; len <<= 1) {
		for (int i = 0; i < n; i += len << 1) {
			for (int j = 0; j < len; ++j) {
				f[i + j] -= f[i + j + len];
			}
		}
	}
}

}

#endif // KWM_T_MATH_CONVOLUTION_INTERNAL_WALSH_HADAMARD_HPP
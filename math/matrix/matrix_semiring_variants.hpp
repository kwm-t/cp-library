/**
 * @file matrix_semiring_variants.hpp
 * @brief 半環上の行列演算のバリエーション
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/matrix/matrix_semiring_variants.hpp
 */
#ifndef KWM_T_MATH_MATRIX_SEMIRING_VARIANTS_HPP
#define KWM_T_MATH_MATRIX_SEMIRING_VARIANTS_HPP

// #include
#include <algorithm>
#include <limits>

/**
 * @brief 半環（Semiring）定義集
 *
 * MatrixSemiring と組み合わせて使用する。
 *
 * 提供:
 *   - MaxPlus（最長路 / tropical）
 *   - MinPlus（最短路）
 *   - Bool（到達可能性）
 *   - Xor（bit演算）
 *
 * 使用例:
 *   using M = MatrixSemiring<semiring::MaxPlus, N>;
 *
 * verified:
 *   - 未検証
 */
namespace kwm_t::math::matrix {

	// ================= Max-Plus (Tropical) =================
	struct MaxPlus {
		using T = long long;
		static T add(T a, T b) { return std::max(a, b); }
		static T mul(T a, T b) { return a + b; }
		static T zero() { return std::numeric_limits<T>::lowest(); }
		static T one() { return 0; }
	};

	// ================= Min-Plus =================
	struct MinPlus {
		using T = long long;
		static T add(T a, T b) { return std::min(a, b); }
		static T mul(T a, T b) { return a + b; }
		static T zero() { return std::numeric_limits<T>::max(); }
		static T one() { return 0; }
	};

	// ================= Bool =================
	struct Bool {
		using T = bool;
		static T add(T a, T b) { return a || b; }
		static T mul(T a, T b) { return a && b; }
		static T zero() { return false; }
		static T one() { return true; }
	};

	// ================= XOR =================
	struct Xor {
		using T = int;
		static T add(T a, T b) { return a ^ b; }
		static T mul(T a, T b) { return a & b; }
		static T zero() { return 0; }
		static T one() { return ~0; }
	};

} // namespace kwm_t::math::matrix

#endif // KWM_T_MATH_SEMIRING_VARIANTS_HPP
/**
 * @file fracmint.hpp
 * @brief 分数を扱う modint 補助機能
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/modint/fracmint.hpp
 */
#ifndef KMW_T_MATH_MODINT_FRACMINT_HPP
#define KMW_T_MATH_MODINT_FRACMINT_HPP

#include <iostream>
#include <cassert>

/**
 * @brief modint上の有理数（デバッグ用途）
 *
 * 典型用途:
 *   - 分数のまま計算を追いたいとき
 *   - 割り算のバグ検出
 *
 * @tparam Mint modint型
 *
 * 注意:
 *   - 約分はしない（mod世界では意味が薄い）
 *   - 分母0は未定義（assertで落とす）
 */

namespace kmw_t::math::modint {

	template <class Mint>
	struct FracMint {
		Mint num, den;

		// --- constructor ---
		FracMint() : num(0), den(1) {}
		FracMint(long long x) : num(x), den(1) {}
		FracMint(long long x, long long y) : num(x), den(y) {
			assert(y != 0);
		}
		FracMint(Mint x, Mint y = Mint(1)) : num(x), den(y) {
			assert(y != Mint(0));
		}

		// --- basic ops ---
		FracMint& operator+=(const FracMint& r) {
			num = num * r.den + den * r.num;
			den = den * r.den;
			return *this;
		}

		FracMint& operator-=(const FracMint& r) {
			num = num * r.den - den * r.num;
			den = den * r.den;
			return *this;
		}

		FracMint& operator*=(const FracMint& r) {
			num *= r.num;
			den *= r.den;
			return *this;
		}

		FracMint& operator/=(const FracMint& r) {
			assert(r.num != Mint(0));
			num *= r.den;
			den *= r.num;
			return *this;
		}

		// Mint == FracMint
		friend bool operator==(const Mint& l, const FracMint& r) {
			return l * r.den == r.num;
		}

		// FracMint == Mint
		friend bool operator==(const FracMint& l, const Mint& r) {
			return l.num == r * l.den;
		}

		// !=
		friend bool operator!=(const Mint& l, const FracMint& r) {
			return !(l == r);
		}

		friend bool operator!=(const FracMint& l, const Mint& r) {
			return !(l == r);
		}

		// --- binary ops ---
		FracMint operator+(const FracMint& r) const { return FracMint(*this) += r; }
		FracMint operator-(const FracMint& r) const { return FracMint(*this) -= r; }
		FracMint operator*(const FracMint& r) const { return FracMint(*this) *= r; }
		FracMint operator/(const FracMint& r) const { return FracMint(*this) /= r; }

		// --- compare ---
		bool operator==(const FracMint& r) const {
			return num * r.den == r.num * den;
		}
		bool operator!=(const FracMint& r) const {
			return !(*this == r);
		}

		// --- increment / decrement ---
		FracMint& operator++() { return *this += 1; }
		FracMint& operator--() { return *this -= 1; }

		FracMint operator++(int) {
			FracMint tmp = *this;
			++(*this);
			return tmp;
		}

		FracMint operator--(int) {
			FracMint tmp = *this;
			--(*this);
			return tmp;
		}

		// --- value ---
		Mint val() const {
			return num * den.inv();
		}

		// --- debug ---
		void print() const {
			std::cout << val().val() << '\n';
		}

		void debug() const {
			std::cout << num.val() << "/" << den.val() << '\n';
		}
	};

} // namespace kmw_t::math::modint

#endif // KMW_T_MATH_MODINT_FRACMINT_HPP
/**
 * @file linear_sieve.hpp
 * @brief 線形篩
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/math/linear_sieve.hpp
 */
#ifndef KWM_T_MATH_LINEAR_SIEVE_HPP
#define KWM_T_MATH_LINEAR_SIEVE_HPP

#include <vector>
#include <numeric>
#include <algorithm>

/**
 * @brief 線形篩 (Linear Sieve)
 *
 * 1 から n までの各整数について最小素因数を O(n) で求める。
 * 同時に素数列も取得可能。
 *
 * 典型用途:
 *   - 高速な素因数分解 (O(log n))
 *   - 素数列の列挙
 *   - 約数列挙の前処理
 *
 * 計算量:
 *   - 構築: O(n)
 *   - 素因数分解: O(log n)
 *
 * 制約 / 注意:
 *   - n は 1e7 程度までが現実的（メモリに依存）
 *   - min_prime_factor[x] は x >= 2 に対して有効
 *
 * 使用例:
 *   kwm_t::math::LinearSieve sieve(1000000);
 *   auto primes = sieve.primes();
 *   auto mpf = sieve.min_prime_factor();
 *
 * verified:
 *   - typical competitive programming usage
 */
namespace kwm_t::math {

class LinearSieve {
public:
	explicit LinearSieve(unsigned int n)
		: n_(n), min_prime_factor_(n + 1) {

		std::iota(min_prime_factor_.begin(), min_prime_factor_.end(), 0);
		prime_list_.reserve(n_ + 1);

		for (unsigned int d = 2; d <= n_; ++d) {
			if (min_prime_factor_[d] == d) {
				prime_list_.push_back(d);
			}
			unsigned int prime_max = std::min(min_prime_factor_[d], n_ / d);
			for (unsigned int prime : prime_list_) {
				if (prime > prime_max) break;
				min_prime_factor_[prime * d] = prime;
			}
		}
	}

	/**
	 * @brief 素数判定
	 */
	bool is_prime(int n) const {
		return min_prime_factor_[n] == n;
	}

	/**
	 * @brief 素数の個数を取得
	 */
	unsigned int prime_count() const {
		return prime_list_.size();
	}

	/**
	 * @brief 素数列を取得
	 */
	const std::vector<unsigned int>& primes() const {
		return prime_list_;
	}

	/**
	 * @brief 最小素因数テーブルを取得
	 */
	const std::vector<unsigned int>& min_prime_factor() const {
		return min_prime_factor_;
	}

private:
	unsigned int n_;
	std::vector<unsigned int> min_prime_factor_;
	std::vector<unsigned int> prime_list_;
};

} // namespace kwm_t::math

#endif // KWM_T_MATH_LINEAR_SIEVE_HPP
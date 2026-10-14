/**
 * @file lucy.hpp
 * @brief Lucy DP
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/algorithm/dp/lucy.hpp
 */
#ifndef KWM_T_ALGORITHM_DP_LUCY_HPP
#define KWM_T_ALGORITHM_DP_LUCY_HPP

#include <vector>
#include <unordered_map>
#include <cmath>

/**
 * @brief Lucy DP (Min_25系) による素数計数
 *
 * @details
 *  n 以下の素数の個数 π(n) または素数の総和を
 *  O(N^(3/4) / logN) 程度で計算するアルゴリズム。
 *
 * Typical Use:
 *  - n <= 1e12 程度の素数個数
 *  - n <= 1e12 程度の素数和
 *
 * Complexity:
 *  Time: O(N^(3/4) / logN)
 *  Memory: O(√N)
 *
 * Parameters:
 *  N : 上限
 *
 * Return:
 *  π(N) または ∑ prime ≤ N
 *
 * Notes:
 *  Lucy DP / Meissel-Lehmer 系の簡易実装
 *
 * Example:
 *  ```cpp
 *  long long cnt = lucyDp_count(1000000000000LL);
 *  long long sum = lucyDp_sum(1000000000000LL);
 *  ```
 *
 * Verified:
 *
 */

namespace kwm_t::algorithm::dp {
/**
 * @brief n 以下の素数の個数 π(n)
 */
inline long long lucyDp_count(const long long N) {
	const long long sqrtN = std::sqrt((long double)N);

	std::vector<long long> Q;
	Q.reserve(sqrtN * 2);

	for (long long i = 1; i <= sqrtN; ++i) Q.push_back(N / i);

	while (Q.back() != 1) Q.push_back(Q.back() - 1);

	std::unordered_map<long long, long long> mp;
	mp.reserve(Q.size() * 2);

	for (auto q : Q) mp[q] = q - 1;

	for (long long p = 2; p <= sqrtN; ++p) {
		if (mp[p] == mp[p - 1]) continue;
		auto pre = mp[p - 1];
		for (auto q : Q) {
			if (q < p * p) break;
			mp[q] -= mp[q / p] - pre;
		}
	}

	return mp[N];
}

/**
 * @brief n 以下の素数の総和
 */
inline long long lucyDp_sum(const long long N) {
	const long long sqrtN = std::sqrt((long double)N);

	std::vector<long long> Q;
	Q.reserve(sqrtN * 2);

	for (long long i = 1; i <= sqrtN; ++i) Q.push_back(N / i);
	while (Q.back() != 1) Q.push_back(Q.back() - 1);

	std::unordered_map<long long, long long> mp;
	mp.reserve(Q.size() * 2);

	for (auto q : Q) mp[q] = q * (q + 1) / 2 - 1;

	for (long long p = 2; p <= sqrtN; ++p) {
		if (mp[p] == mp[p - 1]) continue;
		auto pre = mp[p - 1];
		for (auto q : Q) {
			if (q < p * p) break;
			mp[q] -= p * (mp[q / p] - pre);
		}
	}

	return mp[N];
}

} // namespace kwm_t::algorithm::dp

#endif // KWM_T_ALGORITHM_DP_LUCY_HPP
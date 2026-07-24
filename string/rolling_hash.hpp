#ifndef KWM_T_STRING_ROLLING_HASH_HPP
#define KWM_T_STRING_ROLLING_HASH_HPP

#include <vector>
#include <utility>

/**
 * @brief Rolling Hash（静的・ダブルハッシュ）
 *
 * 文字列や配列に対して prefix hash を構築し、
 * 任意区間のハッシュを O(1) で取得できる
 *
 * 計算量:
 *   構築: O(N)
 *   クエリ: O(1)
 *
 * @tparam T 要素型（char, int など）
 *
 * @param s 入力列
 *
 * 使用例:
 *   RollingHash<std::string> rh(s);
 *   auto h = rh.get(l, r); // [l, r)
 *
 *   if (rh.get(l1, r1) == rh.get(l2, r2)) {
 *       // substring equal
 *   }
 *
 * 制約 / 注意:
 *   - 衝突の可能性は低いがゼロではない
 *   - T は整数として扱える必要あり
 *
 * verified:
 *   - https://atcoder.jp/contests/awc0120/submissions/77788393
 */
namespace kwm_t::string {

template <typename Container>
struct RollingHash {
private:

	static constexpr long long base1 = 1007;
	static constexpr long long base2 = 2009;
	static constexpr long long mod1 = 1000000007;
	static constexpr long long mod2 = 1000000009;

	std::vector<long long> hash1, hash2;
	std::vector<long long> power1, power2;

public:
	using Hash = std::pair<long long, long long>;
	RollingHash() = default;

	explicit RollingHash(const Container& s) {
		build(s);
	}

	void build(const Container& s) {
		int n = (int)s.size();

		hash1.assign(n + 1, 0);
		hash2.assign(n + 1, 0);
		power1.assign(n + 1, 1);
		power2.assign(n + 1, 1);

		for (int i = 0; i < n; ++i) {
			long long x = (long long)s[i];

			hash1[i + 1] = (hash1[i] * base1 + x) % mod1;
			hash2[i + 1] = (hash2[i] * base2 + x) % mod2;

			power1[i + 1] = (power1[i] * base1) % mod1;
			power2[i + 1] = (power2[i] * base2) % mod2;
		}
	}

	/**
	 * @brief 区間 [l, r) のハッシュを取得
	 */
	Hash get(int l, int r) const {
		long long res1 = hash1[r] - hash1[l] * power1[r - l] % mod1;
		if (res1 < 0) res1 += mod1;

		long long res2 = hash2[r] - hash2[l] * power2[r - l] % mod2;
		if (res2 < 0) res2 += mod2;
		return { res1, res2 };
	}

	/**
	 * @brief 長さを取得
	 */
	int size() const {
		return (int)hash1.size() - 1;
	}

	static Hash concat(Hash h1, Hash h2, int len2) {
		long long p1 = 1;
		long long p2 = 1;

		long long b1 = base1;
		long long b2 = base2;

		while (len2 > 0) {
			if (len2 & 1) {
				p1 = p1 * b1 % mod1;
				p2 = p2 * b2 % mod2;
			}

			b1 = b1 * b1 % mod1;
			b2 = b2 * b2 % mod2;
			len2 >>= 1;
		}

		return {
			(h1.first * p1 + h2.first) % mod1,
			(h1.second * p2 + h2.second) % mod2
		};
	}
};

} // namespace kwm_t::string

#endif // KWM_T_STRING_ROLLING_HASH_HPP
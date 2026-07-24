#ifndef KWM_T_STRING_ROLLING_HASH_HPP
#define KWM_T_STRING_ROLLING_HASH_HPP

#include <array>
#include <vector>

/**
 * @brief Rolling Hash（静的・多重ハッシュ）
 *
 * 文字列や配列に対して prefix hash を構築し、
 * 任意区間のハッシュを O(1) で取得できる。
 *
 * ハッシュ数はテンプレート引数で変更できる。
 *
 * 使用例:
 *   RollingHash<std::string> rh(s);      // ダブルハッシュ
 *   RollingHash<std::string, 1> rh(s);   // シングルハッシュ
 *   RollingHash<std::string, 2> rh(s);   // ダブルハッシュ
 *   RollingHash<std::string, 3> rh(s);   // トリプルハッシュ
 *
 *   auto h = rh.get(l, r); // [l, r)
 *
 *   if (rh.get(l1, r1) == rh.get(l2, r2)) {
 *       // substring equal
 *   }
 *
 * 計算量:
 *   構築: O(N * HashCount)
 *   クエリ: O(HashCount)
 *   連結: O(HashCount * log len)
 *
 * @tparam Container 要素型（char, int など）を持つコンテナ
 * @tparam HashCount ハッシュ数
 *
 * 制約 / 注意:
 *   - 衝突の可能性は低いがゼロではない
 *   - T は整数として扱える必要あり
 *
 * verified:
 *   - https://atcoder.jp/contests/awc0120/submissions/77788781
 */
namespace kwm_t::string {

template <typename Container, int HashCount = 2>
struct RollingHash {
private:
	using ll = long long;

	static constexpr std::array<ll, 3> bases = {
		1007,
		2009,
		1000003
	};

	static constexpr std::array<ll, 3> mods = {
		1000000007,
		1000000009,
		998244353
	};

	std::array<std::vector<ll>, HashCount> hashes;
	std::array<std::vector<ll>, HashCount> powers;

public:
	using Hash = std::array<ll, HashCount>;

	RollingHash() = default;

	explicit RollingHash(const Container& s) {
		build(s);
	}

	/**
	 * @brief prefix hash を構築する
	 *
	 * 計算量:
	 *   O(N * HashCount)
	 */
	void build(const Container& s) {
		const int n = (int)s.size();

		for (int k = 0; k < HashCount; ++k) {
			hashes[k].assign(n + 1, 0);
			powers[k].assign(n + 1, 1);
		}

		for (int i = 0; i < n; ++i) {
			const ll x = (ll)s[i];

			for (int k = 0; k < HashCount; ++k) {
				hashes[k][i + 1] =
					(hashes[k][i] * bases[k] + x) % mods[k];

				powers[k][i + 1] =
					powers[k][i] * bases[k] % mods[k];
			}
		}
	}

	/**
	 * @brief 区間 [l, r) のハッシュを取得する
	 *
	 * 計算量:
	 *   O(HashCount)
	 */
	Hash get(int l, int r) const {
		Hash res;

		for (int k = 0; k < HashCount; ++k) {
			res[k] =
				hashes[k][r]
				- hashes[k][l] * powers[k][r - l] % mods[k];

			if (res[k] < 0) {
				res[k] += mods[k];
			}
		}

		return res;
	}

	/**
	 * @brief 長さを取得する
	 */
	int size() const {
		return (int)hashes[0].size() - 1;
	}

	/**
	 * @brief Hash と Hash を連結させる
	 *
	 * h1 に長さ len2 の h2 を連結したハッシュを返す。
	 *
	 * hash(A + B)
	 * = hash(A) * base^|B| + hash(B)
	 *
	 * power 配列は使用せず、繰り返し二乗法で計算する。
	 *
	 * 計算量:
	 *   O(HashCount * log len2)
	 */
	static Hash concat(Hash h1, Hash h2, int len2) {
		Hash res;

		for (int k = 0; k < HashCount; ++k) {
			ll p = 1;
			ll base = bases[k];

			int len = len2;

			while (len > 0) {
				if (len & 1) {
					p = p * base % mods[k];
				}

				base = base * base % mods[k];
				len >>= 1;
			}

			res[k] =
				(h1[k] * p + h2[k]) % mods[k];
		}

		return res;
	}
};

} // namespace kwm_t::string

#endif // KWM_T_STRING_ROLLING_HASH_HPP
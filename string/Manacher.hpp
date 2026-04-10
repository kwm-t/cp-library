#ifndef KWM_T_STRING_MANACHER_HPP
#define KWM_T_STRING_MANACHER_HPP

#include <vector>
#include <string>
#include <algorithm>

/**
 * @brief Manacher法（回文半径）
 *
 * 各中心 i に対して最大の回文半径を求める（奇数長）
 *
 * 計算量:
 *   O(N)
 *
 * @tparam T 比較可能な型
 *
 * @param s 入力列
 * @return radius[i]: 中心iの回文半径
 *
 * verified:
 *  https://atcoder.jp/contests/abc349/submissions/74809646
 *  https://yukicoder.me/submissions/1157003
 *  https://mojacoder.app/users/dyktr_06/contests/mma-014/submissions/bc2fce82-fe18-4124-86e9-336f0b308c99
 */
namespace kwm_t::string {

// =========================
// core: 奇数長（唯一の実装）
// =========================
template <typename T>
std::vector<int> manacher(const std::vector<T>& s) {
	int n = (int)s.size();
	std::vector<int> radius(n);

	int i = 0, j = 0;
	while (i < n) {
		while (i - j >= 0 && i + j < n && s[i - j] == s[i + j]) ++j;
		radius[i] = j;

		int k = 1;
		while (i - k >= 0 && i + k < n && k + radius[i - k] < j) {
			radius[i + k] = radius[i - k];
			++k;
		}

		i += k;
		j -= k;
	}
	return radius;
}

// =========================
// wrapper: 偶数対応（dummy外部指定）
// =========================
template <typename T>
std::vector<int> manacher_all(const std::vector<T>& s, const T& dummy) {
	if (s.empty()) return {};

	std::vector<T> w(2 * (int)s.size() - 1, dummy);
	for (int i = 0; i < (int)s.size(); ++i) {
		w[2 * i] = s[i];
	}

	return manacher(w);
}

// =========================
// wrapper: string版（奇数）
// =========================
inline std::vector<int> manacher(const std::string& s) {
	std::vector<char> v(s.begin(), s.end());
	return manacher(v);
}

// =========================
// wrapper: string版（偶数対応）
// =========================
inline std::vector<int> manacher_all(const std::string& s, char dummy) {
	if (s.empty()) return {};

	std::vector<char> v(s.begin(), s.end());
	return manacher_all(v, dummy);
}

// =========================
// utility: 開始位置ごとの最大回文長（string専用）
// =========================
inline std::vector<int> longest_palindrome_from_each(const std::string& s, char dummy = '*') {
	if (s.empty()) return {};

	// 展開
	std::string t;
	t.reserve(2 * s.size() - 1);
	for (int i = 0; i < (int)s.size(); ++i) {
		if (i) t += dummy;
		t += s[i];
	}

	// Manacher
	auto r = manacher(t);

	std::vector<int> res(s.size());

	for (int i = 0; i < (int)r.size(); ++i) {
		int len, l;

		if (i % 2 == 0) {
			int k = (r[i] + 1) / 2;
			l = i / 2 + 1 - k;
			len = 2 * k - 1;
		}
		else {
			int k = r[i] / 2;
			l = (i + 1) / 2 - k;
			len = 2 * k;
		}

		if (0 <= l && l < (int)res.size()) {
			res[l] = std::max(res[l], len);
		}
	}

	// 伝播
	for (int i = 1; i < (int)res.size(); ++i) {
		res[i] = std::max(res[i], res[i - 1] - 2);
	}

	return res;
}

} // namespace kwm_t::string

#endif // KWM_T_STRING_MANACHER_HPP
/**
 * @file contain.hpp
 * @brief 文字列の包含判定
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/string/contain.hpp
 */
#ifndef KWM_T_STRING_CONTAIN_HPP
#define KWM_T_STRING_CONTAIN_HPP

#include <string>
#include <vector>
#include <atcoder/all>

namespace kwm_t::string {
/**
 * @brief 文字列 s が文字列 t に含まれるか判定（Z-algorithm）
 *
 * 区切り文字を指定可能（デフォルト '#'）
 *
 * 典型用途:
 *   - substring 判定
 *
 * 計算量:
 *   O(|s| + |t|)
 *
 * @param s 探す側（パターン）
 * @param t 探される側（テキスト）
 * @param sep 区切り文字（デフォルト '#')
 *
 * 制約 / 注意:
 *   - sep は s, t に含まれないこと
 *   - AtCoder Library に依存
 *
 * verified:
 *   - AtCoder ABC系
 */

inline bool contain(const std::string& s, const std::string& t, char sep = '#') {
	const int n = (int)s.size();

	// s + sep + t
	std::string u;
	u.reserve(n + 1 + t.size());
	u += s; u += sep; u += t;
	std::vector<int> z = atcoder::z_algorithm(u);

	for (int i = n + 1; i < (int)z.size(); ++i) {
		if (z[i] >= n) return true;
	}
	return false;
}

/**
 * @brief 文字列 s が文字列 t に部分列として含まれるか判定
 *
 * 「連続である必要はない」包含判定。
 * つまり s の各文字を順番を保ったまま t から取り出せるかを判定する。
 *
 * 例:
 *   - "ace" は "abcde" に部分列として含まれる → true
 *   - "aec" は "abcde" に部分列として含まれない → false
 *
 * 典型用途:
 *   - subsequence 判定
 *
 * 計算量:
 *   O(|t|)
 *
 * @param s 探す側（パターン）
 * @param t 探される側（テキスト）
 *
 * @return s が t に部分列として含まれるなら true
 *
 * 制約 / 注意:
 *   - 連続部分文字列ではなく、部分列判定
 *
 * verified:
 *   - https://atcoder.jp/contests/awc0057/submissions/75334826
 */
inline bool contain_subsequence(const std::string& s, const std::string& t) {
	std::size_t idx = 0;

	for (char c : t) {
		if (idx < s.size() && s[idx] == c) {
			++idx;
		}
	}

	return idx == s.size();
}

} // namespace kwm_t::string

#endif // KWM_T_STRING_CONTAIN_HPP
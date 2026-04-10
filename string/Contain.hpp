#ifndef KWM_T_STRING_CONTAIN_HPP
#define KWM_T_STRING_CONTAIN_HPP

#include <string>
#include <vector>
#include <atcoder/all>

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
namespace kwm_t::string {

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

} // namespace kwm_t::string

#endif // KWM_T_STRING_CONTAIN_HPP
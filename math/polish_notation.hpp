#ifndef KWM_T_MATH_POLISH_NOTATION_HPP
#define KWM_T_MATH_POLISH_NOTATION_HPP

#include <stack>
#include <string>
#include <vector>
#include <stdexcept>

namespace kwm_t::math {

/**
 * @brief 前置記法 / 逆ポーランド記法 の評価
 *
 * 典型用途:
 *   - 数式パーサ
 *   - 電卓
 *   - 構文木なしでの式評価
 *
 * 計算量:
 *   O(N)
 *
 * 制約 / 注意:
 *   - トークン列は空白分割済みであること
 *   - 整数除算は C++ の / に従う
 *   - 対応演算子: + - * /
 *
 * verified:
 *   - https://atcoder.jp/contests/awc0076/submissions/76130746
 */

inline bool is_operator(const std::string& s) {
	return s == "+" || s == "-" || s == "*" || s == "/";
}

inline long long apply_operator(
	const std::string& op,
	long long a,
	long long b
) {
	if (op == "+") return a + b;
	if (op == "-") return a - b;
	if (op == "*") return a * b;
	if (op == "/") return a / b;

	throw std::runtime_error("unknown operator");
}

/**
 * @brief 前置記法を評価
 *
 * @param s トークン列
 * @return long long 評価結果
 */
inline long long eval_prefix(
	const std::vector<std::string>& s
) {
	std::stack<long long> st;

	for (int i = (int)s.size() - 1; i >= 0; --i) {
		const std::string& t = s[i];

		if (is_operator(t)) {
			long long a = st.top(); st.pop();
			long long b = st.top(); st.pop();

			st.push(apply_operator(t, a, b));
		}
		else {
			st.push(std::stoll(t));
		}
	}

	return st.top();
}

/**
 * @brief 逆ポーランド記法を評価
 *
 * @param s トークン列
 * @return long long 評価結果
 */
inline long long eval_postfix(
	const std::vector<std::string>& s
) {
	std::stack<long long> st;

	for (const std::string& t : s) {
		if (is_operator(t)) {
			long long b = st.top(); st.pop();
			long long a = st.top(); st.pop();

			st.push(apply_operator(t, a, b));
		}
		else {
			st.push(std::stoll(t));
		}
	}

	return st.top();
}

} // namespace kwm_t::math

#endif // KWM_T_MATH_POLISH_NOTATION_HPP
#ifndef KWM_T_DEBUG_DEBUG_HPP
#define KWM_T_DEBUG_DEBUG_HPP

#include <bitset>
#include <iostream>
#include <iterator>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

/**
 * @brief デバッグ出力ユーティリティ
 *
 * 典型用途:
 *   - コンテナや組を見やすく標準エラー出力へ表示する
 *   - ローカル環境でのデバッグ補助
 *
 * 計算量:
 *   - 出力対象サイズに比例
 *
 * 制約 / 注意:
 *   - `debug(...)` マクロ経由での利用を想定
 *   - C++17 対応
 *   - 内部実装は `detail` 名前空間に配置
 *
 * 使用例:
 *   debug(a, vec, p);
 *
 * verified:
 */
namespace kwm_t::debug_detail {

// =========================================================
// iterable 判定
// =========================================================
template <typename, typename = void>
struct is_iterable : std::false_type {};

template <typename T>
struct is_iterable<
	T,
	std::void_t<
	decltype(std::begin(std::declval<T>())),
	decltype(std::end(std::declval<T>()))
	>
> : std::true_type {};

// =========================================================
// forward declarations
// =========================================================
template <typename A, typename B>
std::string to_string(const std::pair<A, B>& p);

template <typename A, typename B, typename C>
std::string to_string(const std::tuple<A, B, C>& p);

template <typename A, typename B, typename C, typename D>
std::string to_string(const std::tuple<A, B, C, D>& p);

// arithmetic types
template <typename T,
	typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
	std::string to_string(const T& x) {
	return std::to_string(x);
}

inline std::string to_string(const std::string& s) {
	return '"' + s + '"';
}

inline std::string to_string(const char* s) {
	return to_string(std::string(s));
}

inline std::string to_string(bool b) {
	return b ? "true" : "false";
}

inline std::string to_string(const std::vector<bool>& v) {
	bool first = true;
	std::string res = "{";
	for (std::size_t i = 0; i < v.size(); i++) {
		if (!first) {
			res += ", ";
		}
		first = false;
		res += to_string(v[i]);
	}
	res += "}";
	return res;
}

template <std::size_t N>
std::string to_string(const std::bitset<N>& v) {
	std::string res;
	for (std::size_t i = 0; i < N; i++) {
		res += static_cast<char>('0' + v[i]);
	}
	return res;
}

// iterable containers
template <typename A,
	typename std::enable_if<
	is_iterable<A>::value &&
	!std::is_same<A, std::string>::value,
	int
>::type = 0>
std::string to_string(const A& v) {
	bool first = true;
	std::string res = "{";
	for (const auto& x : v) {
		if (!first) {
			res += ", ";
		}
		first = false;
		res += to_string(x);
	}
	res += "}";
	return res;
}

template <typename A, typename B>
std::string to_string(const std::pair<A, B>& p) {
	return "(" + to_string(p.first) + ", " + to_string(p.second) + ")";
}

template <typename A, typename B, typename C>
std::string to_string(const std::tuple<A, B, C>& p) {
	return "("
		+ to_string(std::get<0>(p)) + ", "
		+ to_string(std::get<1>(p)) + ", "
		+ to_string(std::get<2>(p)) + ")";
}

template <typename A, typename B, typename C, typename D>
std::string to_string(const std::tuple<A, B, C, D>& p) {
	return "("
		+ to_string(std::get<0>(p)) + ", "
		+ to_string(std::get<1>(p)) + ", "
		+ to_string(std::get<2>(p)) + ", "
		+ to_string(std::get<3>(p)) + ")";
}

inline void debug_out() {
	std::cerr << '\n';
}

template <typename Head, typename... Tail>
void debug_out(const Head& h, const Tail&... t) {
	std::cerr << " " << to_string(h);
	debug_out(t...);
}

} // namespace kwm_t::debug_detail

#ifdef LOCAL
#define debug(...)                                      \
        do {                                                \
            std::cerr << "[" << #__VA_ARGS__ << "]:";       \
            ::kwm_t::debug_detail::debug_out(__VA_ARGS__);  \
        } while (false)
#else
#define debug(...) ((void)0)
#endif
#endif // KWM_T_DEBUG_DEBUG_HPP
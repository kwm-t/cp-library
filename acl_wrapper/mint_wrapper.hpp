/**
 * @file mint_wrapper.hpp
 * @brief AtCoder Library の mint ラッパー
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/acl_wrapper/mint_wrapper.hpp
 */
#ifndef KWM_T_ACLWRAPPER_MINT_WRAPPER_HPP
#define KWM_T_ACLWRAPPER_MINT_WRAPPER_HPP

#include <iostream>
#include <type_traits>
#include <atcoder/modint>

 // static_modint
template <int m, std::enable_if_t<(1 <= m)>* = nullptr>
std::istream& operator>>(std::istream& is, atcoder::static_modint<m>& x) {
	long long v;
	is >> v;
	x = v;
	return is;
}

template <int m, std::enable_if_t<(1 <= m)>* = nullptr>
std::ostream& operator<<(std::ostream& os, const atcoder::static_modint<m>& x) {
	return os << x.val();
}

// dynamic_modint
template <int id>
std::istream& operator>>(std::istream& is, atcoder::dynamic_modint<id>& x) {
	long long v;
	is >> v;
	x = v;
	return is;
}

template <int id>
std::ostream& operator<<(std::ostream& os, const atcoder::dynamic_modint<id>& x) {
	return os << x.val();
}

#endif // KWM_T_ACLWRAPPER_MINT_WRAPPER_HPP
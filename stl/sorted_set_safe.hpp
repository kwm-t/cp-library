#ifndef KWM_T_STL_SORTED_SET_SAFE_HPP
#define KWM_T_STL_SORTED_SET_SAFE_HPP

#include <set>
#include <iterator>
#include <cstddef>

namespace kwm_t::stl::sorted_set_safe {

// ===== 近傍探索 =====

// <= x（multiset の場合、x が複数あると「最後の x」）
template <class Set, class T>
inline typename Set::const_iterator it_prev_leq(const Set& s, const T& x) {
	auto it = s.upper_bound(x);
	if (it == s.begin()) return s.end();
	--it;
	return it;
}

// < x（multiset の場合、x 未満の最後）
template <class Set, class T>
inline typename Set::const_iterator it_prev_less(const Set& s, const T& x) {
	auto it = s.lower_bound(x);
	if (it == s.begin()) return s.end();
	--it;
	return it;
}

// >= x（multiset の場合、x が複数あれば最初の x）
template <class Set, class T>
inline typename Set::const_iterator it_next_geq(const Set& s, const T& x) {
	auto it = s.lower_bound(x);
	if (it == s.end()) return s.end();
	return it;
}

// > x
template <class Set, class T>
inline typename Set::const_iterator it_next_greater(const Set& s, const T& x) {
	auto it = s.upper_bound(x);
	if (it == s.end()) return s.end();
	return it;
}

// ===== 基本ユーティリティ =====

template <class Set, class T>
inline bool contains(const Set& s, const T& x) {
	auto it = s.lower_bound(x);
	return it != s.end() && *it == x;
}

// set: 唯一の x / multiset: x のどれか 1 個
template <class Set, class T>
inline typename Set::const_iterator it_find(const Set& s, const T& x) {
	return s.find(x);
}

template <class Set>
inline typename Set::const_iterator it_min(const Set& s) {
	if (s.empty()) return s.end();
	return s.begin();
}

template <class Set>
inline typename Set::const_iterator it_max(const Set& s) {
	if (s.empty()) return s.end();
	return std::prev(s.end());
}

// ===== 区間 =====

template <class Set, class T>
inline bool exists_in_range(const Set& s, const T& l, const T& r) {
	auto it = s.lower_bound(l);
	return it != s.end() && *it < r;
}

// set / multiset ともに O(k)（k = 区間内の要素数）
template <class Set, class T>
inline std::size_t count_range(const Set& s, const T& l, const T& r) {
	auto L = s.lower_bound(l);
	auto R = s.lower_bound(r);
	return std::distance(L, R);
}

// ===== erase / pop =====

// set: 唯一の x / multiset: x のどれか 1 個だけ消す
template <class Set, class T>
inline bool erase_one(Set& s, const T& x) {
	auto it = s.find(x);
	if (it == s.end()) return false;
	s.erase(it);
	return true;
}

template <class Set>
inline bool pop_min(Set& s, typename Set::value_type& out) {
	if (s.empty()) return false;
	auto it = s.begin();
	out = *it;
	s.erase(it);
	return true;
}

template <class Set>
inline bool pop_max(Set& s, typename Set::value_type& out) {
	if (s.empty()) return false;
	auto it = std::prev(s.end());
	out = *it;
	s.erase(it);
	return true;
}

// ===== multiset 前提ユーティリティ =====

// multiset 前提：
// x が複数ある場合の「最初の x」
template <class Set, class T>
inline typename Set::const_iterator it_find_first(const Set& s, const T& x) {
	auto it = s.lower_bound(x);
	if (it == s.end() || *it != x) return s.end();
	return it;
}

// multiset 前提：
// x が複数ある場合の「最後の x」
template <class Set, class T>
inline typename Set::const_iterator it_find_last(const Set& s, const T& x) {
	auto it = s.upper_bound(x);
	if (it == s.begin()) return s.end();
	--it;
	if (*it != x) return s.end();
	return it;
}

// multiset 前提：
// x を全部消す（set でも 0 or 1 個消える）
template <class Set, class T>
inline std::size_t erase_all(Set& s, const T& x) {
	auto L = s.lower_bound(x);
	auto R = s.upper_bound(x);
	std::size_t cnt = std::distance(L, R);
	s.erase(L, R);
	return cnt;
}

// multiset 前提：
// x の最初の 1 個だけ消す
template <class Set, class T>
inline bool erase_first(Set& s, const T& x) {
	auto it = s.lower_bound(x);
	if (it == s.end() || *it != x) return false;
	s.erase(it);
	return true;
}

// multiset 前提：
// x の最後の 1 個だけ消す
template <class Set, class T>
inline bool erase_last(Set& s, const T& x) {
	auto it = s.upper_bound(x);
	if (it == s.begin()) return false;
	--it;
	if (*it != x) return false;
	s.erase(it);
	return true;
}

// multiset 前提：
// x の個数
template <class Set, class T>
inline std::size_t count_equal(const Set& s, const T& x) {
	auto L = s.lower_bound(x);
	auto R = s.upper_bound(x);
	return std::distance(L, R);
}

// ===== 最近傍 =====

// multiset の場合、同値が複数あればどれか 1 つを返す
template <class Set, class T>
inline typename Set::const_iterator it_closest(const Set& s, const T& x) {
	if (s.empty()) return s.end();

	auto r = s.lower_bound(x);
	if (r == s.begin()) return r;
	if (r == s.end()) return std::prev(r);

	auto l = std::prev(r);
	if (x - *l <= *r - x) return l;
	return r;
}

}
#endif // KWM_T_STL_SORTED_SET_SAFE_HPP
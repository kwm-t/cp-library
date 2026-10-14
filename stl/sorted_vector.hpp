/**
 * @file sorted_vector.hpp
 * @brief ソート済み vector
 * @author kwm_t
 * @see https://github.com/kwm-t/cp-library/blob/main/stl/sorted_vector.hpp
 */
#ifndef KWM_T_STL_SORTED_VECTOR_HPP
#define KWM_T_STL_SORTED_VECTOR_HPP

#include <vector>
#include <algorithm>
#include <iterator>
#include <cstddef>

namespace kwm_t::stl::sorted_vector {
// x と等しい要素の個数
template <class T>
std::size_t count_equal(const std::vector<T>& v, const T& x) {
	auto l = std::lower_bound(v.begin(), v.end(), x);
	auto r = std::upper_bound(v.begin(), v.end(), x);
	return std::distance(l, r);
}

// x 未満の個数
template <class T>
std::size_t count_less(const std::vector<T>& v, const T& x) {
	return std::distance(v.begin(), std::lower_bound(v.begin(), v.end(), x));
}

// x 以下の個数
template <class T>
std::size_t count_leq(const std::vector<T>& v, const T& x) {
	return std::distance(v.begin(), std::upper_bound(v.begin(), v.end(), x));
}

// x 以上の個数
template <class T>
std::size_t count_geq(const std::vector<T>& v, const T& x) {
	return std::distance(std::lower_bound(v.begin(), v.end(), x), v.end());
}

// x より大きい個数
template <class T>
std::size_t count_greater(const std::vector<T>& v, const T& x) {
	return std::distance(std::upper_bound(v.begin(), v.end(), x), v.end());
}

// [l, r) に含まれる個数
template <class T>
std::size_t count_range(const std::vector<T>& v, const T& l, const T& r) {
	if (l > r)return 0;
	auto L = std::lower_bound(v.begin(), v.end(), l);
	auto R = std::lower_bound(v.begin(), v.end(), r);
	return std::distance(L, R);
}

// 存在判定（1個以上あるか）
template <class T>
bool contains(const std::vector<T>& v, const T& x) {
	auto it = std::lower_bound(v.begin(), v.end(), x);
	return it != v.end() && *it == x;
}

// 最初の出現位置（なければ v.size()）
template <class T>
std::size_t first_index(const std::vector<T>& v, const T& x) {
	auto it = std::lower_bound(v.begin(), v.end(), x);
	return (it != v.end() && *it == x) ? std::size_t(it - v.begin()) : v.size();
}

// 最後の出現位置（なければ v.size()）
template <class T>
std::size_t last_index(const std::vector<T>& v, const T& x) {
	auto it = std::upper_bound(v.begin(), v.end(), x);
	if (it == v.begin()) return v.size();
	--it;
	return (*it == x) ? std::size_t(it - v.begin()) : v.size();

}

// x 未満のうち最大の要素
template <class T>
auto it_less(const std::vector<T>& v, const T& x) {
	auto it = std::lower_bound(v.begin(), v.end(), x);
	if (it == v.begin()) return v.end();
	--it;
	return it;
}

// x 以下のうち最大の要素
template <class T>
auto it_leq(const std::vector<T>& v, const T& x) {
	auto it = std::upper_bound(v.begin(), v.end(), x);
	if (it == v.begin()) return v.end();
	--it;
	return it;
}

// x 以上のうち最小の要素
template <class T>
auto it_geq(const std::vector<T>& v, const T& x) {
	return std::lower_bound(v.begin(), v.end(), x);
}

// x より大きい最小の要素
template <class T>
auto it_greater(const std::vector<T>& v, const T& x) {
	return std::upper_bound(v.begin(), v.end(), x);
}

// x の最初の出現位置（なければ v.end()）
template <class T>
auto it_first(const std::vector<T>& v, const T& x) {
	auto it = std::lower_bound(v.begin(), v.end(), x);
	if (it != v.end() && *it == x) return it;
	return v.end();
}

// x の最後の出現位置（なければ v.end()）
template <class T>
auto it_last(const std::vector<T>& v, const T& x) {
	auto it = std::upper_bound(v.begin(), v.end(), x);
	if (it == v.begin()) return v.end();
	--it;
	if (*it == x) return it;
	return v.end();
}
} // namespace kwm_t::stl::sorted_vector

#endif // KWM_T_STL_SORTED_VECTOR_HPP
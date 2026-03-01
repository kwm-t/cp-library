#pragma once
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstddef>

namespace stl {
	namespace sorted_vector {
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

	} // namespace sorted_vector
} // namespace stl
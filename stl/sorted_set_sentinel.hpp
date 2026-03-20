#ifndef KWM_T_STL_SORTED_SET_SENTINEL_HPP
#define KWM_T_STL_SORTED_SET_SENTINEL_HPP

#include <set>
#include <iterator>
#include <cstddef>

namespace stl {
	namespace sorted_set_sentinel {

		// ========= 前提 =========
		// ・先頭と末尾に番兵
		// ・常に size() >= 2
		// ・min_sentinel < 実データ < max_sentinel

		// ===== 近傍探索（safe と同名） =====

		// <= x（multiset の場合、x が複数あると「最後の x」）
		template <class Set, class T>
		inline typename Set::const_iterator it_prev_leq(const Set& s, const T& x) {
			return std::prev(s.upper_bound(x));
		}

		// < x（multiset の場合、x 未満の最後）
		template <class Set, class T>
		inline typename Set::const_iterator it_prev_less(const Set& s, const T& x) {
			return std::prev(s.lower_bound(x));
		}

		// >= x（multiset の場合、x が複数あれば最初の x）
		template <class Set, class T>
		inline typename Set::const_iterator it_next_geq(const Set& s, const T& x) {
			return s.lower_bound(x);
		}

		// > x
		template <class Set, class T>
		inline typename Set::const_iterator it_next_greater(const Set& s, const T& x) {
			return s.upper_bound(x);
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
			return std::next(s.begin());
		}

		template <class Set>
		inline typename Set::const_iterator it_max(const Set& s) {
			return std::prev(std::prev(s.end()));
		}

		// ===== 区間 =====

		template <class Set, class T>
		inline bool exists_in_range(const Set& s, const T& l, const T& r) {
			auto it = s.lower_bound(l);
			return *it < r;
		}

		// set / multiset ともに O(logN + k)
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
			if (s.size() <= 2) return false;
			auto it = it_min(s);
			out = *it;
			s.erase(it);
			return true;
		}

		template <class Set>
		inline bool pop_max(Set& s, typename Set::value_type& out) {
			if (s.size() <= 2) return false;
			auto it = it_max(s);
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
			if (*it != x) return s.end();   // 番兵前提で it==end() にはならない
			return it;
		}

		// multiset 前提：
		// x が複数ある場合の「最後の x」
		template <class Set, class T>
		inline typename Set::const_iterator it_find_last(const Set& s, const T& x) {
			auto it = s.upper_bound(x);
			--it;                           // 番兵前提で安全
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
			if (*it != x) return false;
			s.erase(it);
			return true;
		}

		// multiset 前提：
		// x の最後の 1 個だけ消す
		template <class Set, class T>
		inline bool erase_last(Set& s, const T& x) {
			auto it = s.upper_bound(x);
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
			auto r = s.lower_bound(x);
			auto l = std::prev(r);
			if (x - *l <= *r - x) return l;
			return r;
		}

		// ===== 番兵ユーティリティ =====

		template <class Set>
		inline bool empty_real(const Set& s) {
			return s.size() <= 2;
		}

		template <class Set>
		inline std::size_t size_real(const Set& s) {
			return s.size() <= 2 ? 0 : s.size() - 2;
		}
	}
}

#endif // KWM_T_STL_SORTED_SET_SENTINEL_HPP